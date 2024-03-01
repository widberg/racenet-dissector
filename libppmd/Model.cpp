/****************************************************************************
 *  This file is part of PPMd project                                       *
 *  Written and distributed to public domain by Dmitry Shkarin 1997,        *
 *  1999-2001                                                               *
 *  Contents: PPMII model description and encoding/decoding routines        *
 ****************************************************************************/
#include "PPMd.h"
#include "Coder.hpp"
#include "SubAlloc.hpp"

const int INT_BITS=7, PERIOD_BITS=7, TOT_BITS=INT_BITS+PERIOD_BITS,
        INTERVAL=1 << INT_BITS, BIN_SCALE=1 << TOT_BITS, MAX_FREQ=124;

#pragma pack(1)
static struct SEE2_CONTEXT { // SEE-contexts for PPM-contexts with masked symbols
    WORD Summ;
    BYTE Shift, Count;
    void init(int InitVal) { Summ=InitVal << (Shift=PERIOD_BITS-4); Count=4; }
    UINT getMean() {
        UINT RetVal=(Summ >> Shift);        Summ -= RetVal;
        return RetVal+(RetVal == 0);
    }
    void update() {
        if (Shift < PERIOD_BITS && --Count == 0) {
            Summ += Summ;                   Count=3 << Shift++;
        }
    }
} _PACK_ATTR SEE2Cont[25][16], DummySEE2Cont;
static struct PPM_CONTEXT {
    struct STATE { BYTE Symbol, Freq; PPM_CONTEXT* Successor; } _PACK_ATTR;
    WORD NumStats;
    union {
        struct {
            WORD SummFreq;                     // sizeof(WORD) > sizeof(BYTE)
            STATE * Stats;
        };
        STATE _oneState;
    };
    PPM_CONTEXT* Suffix;
    inline void encodeBinSymbol(int symbol);    // MaxOrder:
    inline void   encodeSymbol1(int symbol);    //  ABCD    context
    inline void   encodeSymbol2(int symbol);    //   BCD    suffix
    inline void           decodeBinSymbol();    //   BCDE   successor
    inline void             decodeSymbol1();    // other orders:
    inline void             decodeSymbol2();    //   BCD    context
    inline void           update1(STATE* p);    //    CD    suffix
    inline void           update2(STATE* p);    //   BCDE   successor
    void                          rescale();
    inline PPM_CONTEXT* createChild(STATE* pStats,STATE& FirstState);
    inline SEE2_CONTEXT* makeEscFreq2(int Diff);
    STATE& oneState() { return _oneState; }
} _PACK_ATTR * MinContext, * MaxContext;
#pragma pack()

static PPM_CONTEXT::STATE* FoundState;      // found next state transition
static int NumMasked, InitEsc, OrderFall, RunLength, InitRL, MaxOrder;
static BYTE CharMask[256], NS2Indx[256], NS2BSIndx[256], HB2Flag[256];
static BYTE EscCount, PrintCount, PrevSuccess, HiBitsFlag;
static WORD BinSumm[128][64];               // binary SEE-contexts

inline PPM_CONTEXT* PPM_CONTEXT::createChild(STATE* pStats,STATE& FirstState)
{
    PPM_CONTEXT* pc = (PPM_CONTEXT*) AllocContext();
    if ( pc ) {
        pc->NumStats=1;                     pc->oneState()=FirstState;
        pc->Suffix=this;                    pStats->Successor=pc;
    }
    return pc;
}
static void RestartModelRare()
{
    int i, k, m;
    memset(CharMask,0,sizeof(CharMask));
    InitSubAllocator();                     InitRL=-((MaxOrder < 12)?MaxOrder:12)-1;
    MinContext = MaxContext = (PPM_CONTEXT*) AllocContext();
    MinContext->Suffix=NULL;                OrderFall=MaxOrder;
    MinContext->SummFreq=(MinContext->NumStats=256)+1;
    FoundState = MinContext->Stats = (PPM_CONTEXT::STATE*) AllocUnits(256/2);
    for (RunLength=InitRL, PrevSuccess=i=0;i < 256;i++) {
        MinContext->Stats[i].Symbol=i;      MinContext->Stats[i].Freq=1;
        MinContext->Stats[i].Successor=NULL;
    }
static const WORD InitBinEsc[]={0x3CDD,0x1F3F,0x59BF,0x48F3,0x64A1,0x5ABC,0x6632,0x6051};
    for (i=0;i < 128;i++)
        for (k=0;k < 8;k++)
            for (m=0;m < 64;m += 8)
                    BinSumm[i][k+m]=BIN_SCALE-InitBinEsc[k]/(i+2);
    for (i=0;i < 25;i++)
            for (k=0;k < 16;k++)            SEE2Cont[i][k].init(5*i+10);
}
static void _FASTCALL StartModelRare(int MaxOrder)
{
    int i, k, m ,Step;
    EscCount=PrintCount=1;
    if (MaxOrder < 2) {
        memset(CharMask,0,sizeof(CharMask));
        OrderFall=::MaxOrder;               MinContext=MaxContext;
        while (MinContext->Suffix != NULL) {
            MinContext=MinContext->Suffix;  OrderFall--;
        }
        FoundState=MinContext->Stats;       MinContext=MaxContext;
    } else {
        ::MaxOrder=MaxOrder;                RestartModelRare();
        NS2BSIndx[0]=2*0;                   NS2BSIndx[1]=2*1;
        memset(NS2BSIndx+2,2*2,9);          memset(NS2BSIndx+11,2*3,256-11);
        for (i=0;i < 3;i++)                 NS2Indx[i]=i;
        for (m=i, k=Step=1;i < 256;i++) {
            NS2Indx[i]=m;
            if ( !--k ) { k = ++Step;       m++; }
        }
        memset(HB2Flag,0,0x40);             memset(HB2Flag+0x40,0x08,0x100-0x40);
        DummySEE2Cont.Shift=PERIOD_BITS;
    }
}
void PPM_CONTEXT::rescale()
{
    int OldNS=NumStats, i=NumStats-1, Adder, EscFreq;
    STATE* p1, * p;
    for (p=FoundState;p != Stats;p--)       _PPMD_SWAP(p[0],p[-1]);
    Stats->Freq += 4;                       SummFreq += 4;
    EscFreq=SummFreq-p->Freq;               Adder=(OrderFall != 0);
    SummFreq = (p->Freq=(p->Freq+Adder) >> 1);
    do {
        EscFreq -= (++p)->Freq;
        SummFreq += (p->Freq=(p->Freq+Adder) >> 1);
        if (p[0].Freq > p[-1].Freq) {
            STATE tmp=*(p1=p);
            do { p1[0]=p1[-1]; } while (--p1 != Stats && tmp.Freq > p1[-1].Freq);
            *p1=tmp;
        }
    } while ( --i );
    if (p->Freq == 0) {
        do { i++; } while ((--p)->Freq == 0);
        EscFreq += i;
        if ((NumStats -= i) == 1) {
            STATE tmp=*Stats;
            do { tmp.Freq-=(tmp.Freq >> 1); EscFreq>>=1; } while (EscFreq > 1);
            FreeUnits(Stats,(OldNS+1) >> 1);
            *(FoundState=&oneState())=tmp;  return;
        }
    }
    SummFreq += (EscFreq -= (EscFreq >> 1));
    int n0=(OldNS+1) >> 1, n1=(NumStats+1) >> 1;
    if (n0 != n1)
           Stats = (STATE*) ShrinkUnits(Stats,n0,n1);
    FoundState=Stats;
}
static inline PPM_CONTEXT* CreateSuccessors(BOOL Skip,PPM_CONTEXT::STATE* p1)
{
// static UpState declaration bypasses IntelC bug
static PPM_CONTEXT::STATE UpState;
    PPM_CONTEXT* pc=MinContext, * UpBranch=FoundState->Successor;
    PPM_CONTEXT::STATE * p, * ps[MAX_O], ** pps=ps;
    if ( !Skip ) {
        *pps++ = FoundState;
        if ( !pc->Suffix )                  goto NO_LOOP;
    }
    if ( p1 ) {
        p=p1;                               pc=pc->Suffix;
        goto LOOP_ENTRY;
    }
    do {
        pc=pc->Suffix;
        if (pc->NumStats != 1) {
            if ((p=pc->Stats)->Symbol != FoundState->Symbol)
                do { p++; } while (p->Symbol != FoundState->Symbol);
        } else                              p=&(pc->oneState());
LOOP_ENTRY:
        if (p->Successor != UpBranch) {
            pc=p->Successor;                break;
        }
        *pps++ = p;
    } while ( pc->Suffix );
NO_LOOP:
    if (pps == ps)                          return pc;
    UpState.Symbol=*(BYTE*) UpBranch;
    UpState.Successor=(PPM_CONTEXT*) (((BYTE*) UpBranch)+1);
    if (pc->NumStats != 1) {
        if ((p=pc->Stats)->Symbol != UpState.Symbol)
                do { p++; } while (p->Symbol != UpState.Symbol);
        UINT cf=p->Freq-1;
        UINT s0=pc->SummFreq-pc->NumStats-cf;
        UpState.Freq=1+((2*cf <= s0)?(5*cf > s0):((2*cf+3*s0-1)/(2*s0)));
    } else                                  UpState.Freq=pc->oneState().Freq;
    do {
        pc = pc->createChild(*--pps,UpState);
        if ( !pc )                          return NULL;
    } while (pps != ps);
    return pc;
}
static inline void UpdateModel()
{
    PPM_CONTEXT::STATE fs = *FoundState, * p = NULL;
    PPM_CONTEXT* pc, * Successor;
    UINT ns1, ns, cf, sf, s0;
    if (fs.Freq < MAX_FREQ/4 && (pc=MinContext->Suffix) != NULL) {
        if (pc->NumStats != 1) {
            if ((p=pc->Stats)->Symbol != fs.Symbol) {
                do { p++; } while (p->Symbol != fs.Symbol);
                if (p[0].Freq >= p[-1].Freq) {
                    _PPMD_SWAP(p[0],p[-1]); p--;
                }
            }
            if (p->Freq < MAX_FREQ-9) {
                p->Freq += 2;               pc->SummFreq += 2;
            }
        } else {
            p=&(pc->oneState());            p->Freq += (p->Freq < 32);
        }
    }
    if ( !OrderFall ) {
        MinContext=MaxContext=FoundState->Successor=CreateSuccessors(TRUE,p);
        if ( !MinContext )                  goto RESTART_MODEL;
        return;
    }
    *pText++ = fs.Symbol;                   Successor = (PPM_CONTEXT*) pText;
    if (pText >= UnitsStart)                goto RESTART_MODEL;
    if ( fs.Successor ) {
        if ((BYTE*) fs.Successor <= pText &&
                (fs.Successor=CreateSuccessors(FALSE,p)) == NULL)
                        goto RESTART_MODEL;
        if ( !--OrderFall ) {
            Successor=fs.Successor;         pText -= (MaxContext != MinContext);
        }
    } else {
        FoundState->Successor=Successor;    fs.Successor=MinContext;
    }
    s0=MinContext->SummFreq-(ns=MinContext->NumStats)-(fs.Freq-1);
    for (pc=MaxContext;pc != MinContext;pc=pc->Suffix) {
        if ((ns1=pc->NumStats) != 1) {
            if ((ns1 & 1) == 0) {
                pc->Stats=(PPM_CONTEXT::STATE*) ExpandUnits(pc->Stats,ns1 >> 1);
                if ( !pc->Stats )           goto RESTART_MODEL;
            }
            pc->SummFreq += (2*ns1 < ns)+2*((4*ns1 <= ns) &
                    (pc->SummFreq <= 8*ns1));
        } else {
            p=(PPM_CONTEXT::STATE*) AllocUnits(1);
            if ( !p )                       goto RESTART_MODEL;
            *p=pc->oneState();              pc->Stats=p;
            if (p->Freq < MAX_FREQ/4-1)     p->Freq += p->Freq;
            else                            p->Freq  = MAX_FREQ-4;
            pc->SummFreq=p->Freq+InitEsc+(ns > 3);
        }
        cf=2*fs.Freq*(pc->SummFreq+6);      sf=s0+pc->SummFreq;
        if (cf < 6*sf) {
            cf=1+(cf > sf)+(cf >= 4*sf);
            pc->SummFreq += 3;
        } else {
            cf=4+(cf >= 9*sf)+(cf >= 12*sf)+(cf >= 15*sf);
            pc->SummFreq += cf;
        }
        p=pc->Stats+ns1;                    p->Successor=Successor;
        p->Symbol = fs.Symbol;              p->Freq = cf;
        pc->NumStats=++ns1;
    }
    MaxContext=MinContext=fs.Successor;
    return;
RESTART_MODEL:
    RestartModelRare();
    EscCount=0;                             PrintCount=0xFF;
}
// Tabulated escapes for exponential symbol distribution
static const BYTE ExpEscape[16]={ 25,14, 9, 7, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2 };
#define GET_MEAN(SUMM,SHIFT,ROUND) ((SUMM+(1 << (SHIFT-ROUND))) >> (SHIFT))
inline void PPM_CONTEXT::encodeBinSymbol(int symbol)
{
    STATE& rs=oneState();                   HiBitsFlag=HB2Flag[FoundState->Symbol];
    WORD& bs=BinSumm[rs.Freq-1][PrevSuccess+NS2BSIndx[Suffix->NumStats-1]+
            HiBitsFlag+2*HB2Flag[rs.Symbol]+((RunLength >> 26) & 0x20)];
    if (rs.Symbol == symbol) {
        FoundState=&rs;                     rs.Freq += (rs.Freq < 128);
        SubRange.LowCount=0;                SubRange.HighCount=bs;
        bs += INTERVAL-GET_MEAN(bs,PERIOD_BITS,2);
        PrevSuccess=1;                      RunLength++;
    } else {
        SubRange.LowCount=bs;               bs -= GET_MEAN(bs,PERIOD_BITS,2);
        SubRange.HighCount=BIN_SCALE;       InitEsc=ExpEscape[bs >> 10];
        NumMasked=1;                        CharMask[rs.Symbol]=EscCount;
        PrevSuccess=0;                      FoundState=NULL;
    }
}
inline void PPM_CONTEXT::decodeBinSymbol()
{
    STATE& rs=oneState();                   HiBitsFlag=HB2Flag[FoundState->Symbol];
    WORD& bs=BinSumm[rs.Freq-1][PrevSuccess+NS2BSIndx[Suffix->NumStats-1]+
            HiBitsFlag+2*HB2Flag[rs.Symbol]+((RunLength >> 26) & 0x20)];
    if (ariGetCurrentShiftCount(TOT_BITS) < bs) {
        FoundState=&rs;                     rs.Freq += (rs.Freq < 128);
        SubRange.LowCount=0;                SubRange.HighCount=bs;
        bs += INTERVAL-GET_MEAN(bs,PERIOD_BITS,2);
        PrevSuccess=1;                      RunLength++;
    } else {
        SubRange.LowCount=bs;               bs -= GET_MEAN(bs,PERIOD_BITS,2);
        SubRange.HighCount=BIN_SCALE;       InitEsc=ExpEscape[bs >> 10];
        NumMasked=1;                        CharMask[rs.Symbol]=EscCount;
        PrevSuccess=0;                      FoundState=NULL;
    }
}
inline void PPM_CONTEXT::update1(STATE* p)
{
    (FoundState=p)->Freq += 4;              SummFreq += 4;
    if (p[0].Freq > p[-1].Freq) {
        _PPMD_SWAP(p[0],p[-1]);             FoundState=--p;
        if (p->Freq > MAX_FREQ)             rescale();
    }
}
inline void PPM_CONTEXT::encodeSymbol1(int symbol)
{
    SubRange.scale=SummFreq;
    STATE* p=Stats;
    if (p->Symbol == symbol) {
        PrevSuccess=(2*(SubRange.HighCount=p->Freq) > SubRange.scale);
        RunLength += PrevSuccess;
        (FoundState=p)->Freq += 4;          SummFreq += 4;
        if (p->Freq > MAX_FREQ)             rescale();
        SubRange.LowCount=0;                return;
    }
    PrevSuccess=0;
    int LoCnt=p->Freq, i=NumStats-1;
    while ((++p)->Symbol != symbol) {
        LoCnt += p->Freq;
        if (--i == 0) {
            HiBitsFlag=HB2Flag[FoundState->Symbol];
            SubRange.LowCount=LoCnt;        CharMask[p->Symbol]=EscCount;
            i=(NumMasked=NumStats)-1;       FoundState=NULL;
            do { CharMask[(--p)->Symbol]=EscCount; } while ( --i );
            SubRange.HighCount=SubRange.scale;
            return;
        }
    }
    SubRange.HighCount=(SubRange.LowCount=LoCnt)+p->Freq;
    update1(p);
}
inline void PPM_CONTEXT::decodeSymbol1()
{
    SubRange.scale=SummFreq;
    STATE* p=Stats;
    int i, count, HiCnt;
    if ((count=ariGetCurrentCount()) < (HiCnt=p->Freq)) {
        PrevSuccess=(2*(SubRange.HighCount=HiCnt) > SubRange.scale);
        RunLength += PrevSuccess;
        (FoundState=p)->Freq=(HiCnt += 4);  SummFreq += 4;
        if (HiCnt > MAX_FREQ)               rescale();
        SubRange.LowCount=0;                return;
    }
    PrevSuccess=0;                          i=NumStats-1;
    while ((HiCnt += (++p)->Freq) <= count)
        if (--i == 0) {
            HiBitsFlag=HB2Flag[FoundState->Symbol];
            SubRange.LowCount=HiCnt;        CharMask[p->Symbol]=EscCount;
            i=(NumMasked=NumStats)-1;       FoundState=NULL;
            do { CharMask[(--p)->Symbol]=EscCount; } while ( --i );
            SubRange.HighCount=SubRange.scale;
            return;
        }
    SubRange.LowCount=(SubRange.HighCount=HiCnt)-p->Freq;
    update1(p);
}
inline void PPM_CONTEXT::update2(STATE* p)
{
    (FoundState=p)->Freq += 4;              SummFreq += 4;
    if (p->Freq > MAX_FREQ)                 rescale();
    EscCount++;                             RunLength=InitRL;
}
inline SEE2_CONTEXT* PPM_CONTEXT::makeEscFreq2(int Diff)
{
    SEE2_CONTEXT* psee2c;
    if (NumStats != 256) {
        psee2c=SEE2Cont[NS2Indx[Diff-1]]+(Diff < Suffix->NumStats-NumStats)+
                2*(SummFreq < 11*NumStats)+4*(NumMasked > Diff)+HiBitsFlag;
        SubRange.scale=psee2c->getMean();
    } else {
        psee2c=&DummySEE2Cont;              SubRange.scale=1;
    }
    return psee2c;
}
inline void PPM_CONTEXT::encodeSymbol2(int symbol)
{
    int HiCnt, i=NumStats-NumMasked;
    SEE2_CONTEXT* psee2c=makeEscFreq2(i);
    STATE* p=Stats-1;                       HiCnt=0;
    do {
        do { p++; } while (CharMask[p->Symbol] == EscCount);
        HiCnt += p->Freq;
        if (p->Symbol == symbol)            goto SYMBOL_FOUND;
        CharMask[p->Symbol]=EscCount;
    } while ( --i );
    SubRange.HighCount=(SubRange.scale += (SubRange.LowCount=HiCnt));
    psee2c->Summ += SubRange.scale;         NumMasked = NumStats;
    return;
SYMBOL_FOUND:
    SubRange.LowCount = (SubRange.HighCount=HiCnt)-p->Freq;
    if ( --i ) {
        STATE* p1=p;
        do {
            do { p1++; } while (CharMask[p1->Symbol] == EscCount);
            HiCnt += p1->Freq;
        } while ( --i );
    }
    SubRange.scale += HiCnt;
    psee2c->update();                       update2(p);
}
inline void PPM_CONTEXT::decodeSymbol2()
{
    int count, HiCnt, i=NumStats-NumMasked;
    SEE2_CONTEXT* psee2c=makeEscFreq2(i);
    STATE* ps[256], ** pps=ps, * p=Stats-1;
    HiCnt=0;
    do {
        do { p++; } while (CharMask[p->Symbol] == EscCount);
        HiCnt += p->Freq;                   *pps++ = p;
    } while ( --i );
    SubRange.scale += HiCnt;                count=ariGetCurrentCount();
    p=*(pps=ps);
    if (count < HiCnt) {
        HiCnt=0;
        while ((HiCnt += p->Freq) <= count) p=*++pps;
        SubRange.LowCount = (SubRange.HighCount=HiCnt)-p->Freq;
        psee2c->update();                   update2(p);
    } else {
        SubRange.LowCount=HiCnt;            SubRange.HighCount=SubRange.scale;
        i=NumStats-NumMasked;               pps--;
        do { CharMask[(*++pps)->Symbol]=EscCount; } while ( --i );
        psee2c->Summ += SubRange.scale;     NumMasked = NumStats;
    }
}
inline void ClearMask()
{
    EscCount=1;
	memset(CharMask,0,sizeof(CharMask));
	++PrintCount;
}

void _STDCALL EncodeFile(std::ostream & EncodedFile, std::istream & DecodedFile,int MaxOrder)
{
    ariInitEncoder();                       StartModelRare(MaxOrder);
    for (int ns=MinContext->NumStats; ;ns=MinContext->NumStats) {
        int c;
        if ((c = DecodedFile.get()) == EOF) break;
        if (ns != 1) {
            MinContext->encodeSymbol1(c);   ariEncodeSymbol();
        } else {
            MinContext->encodeBinSymbol(c); ariShiftEncodeSymbol(TOT_BITS);
        }
        while ( !FoundState ) {
            ARI_ENC_NORMALIZE(EncodedFile);
            do {
                OrderFall++;                MinContext=MinContext->Suffix;
                if ( !MinContext )          goto STOP_ENCODING;
            } while (MinContext->NumStats == NumMasked);
            MinContext->encodeSymbol2(c);   ariEncodeSymbol();
        }
        if (!OrderFall && (BYTE*) FoundState->Successor > pText)
                MinContext=MaxContext=FoundState->Successor;
        else {
            UpdateModel();
            if (EscCount == 0)              
				ClearMask();
        }
        ARI_ENC_NORMALIZE(EncodedFile);
    }
STOP_ENCODING:
    ARI_FLUSH_ENCODER(EncodedFile);         
	//PrintInfo(DecodedFile,EncodedFile);
}
void _STDCALL DecodeFile(std::ostream & DecodedFile, std::istream & EncodedFile,int MaxOrder, std::size_t DecodedFileSize)
{
    ARI_INIT_DECODER(EncodedFile);          StartModelRare(MaxOrder);
    for (int ns=MinContext->NumStats; ;ns=MinContext->NumStats) {
        if (ns != 1)                        MinContext->decodeSymbol1();
        else                                MinContext->decodeBinSymbol();
        ariRemoveSubrange();
        while ( !FoundState ) {
            ARI_DEC_NORMALIZE(EncodedFile);
            do {
                OrderFall++;                MinContext=MinContext->Suffix;
                if ( !MinContext )          goto STOP_DECODING;
            } while (MinContext->NumStats == NumMasked);
            MinContext->decodeSymbol2();    ariRemoveSubrange();
        }
        DecodedFile.put(FoundState->Symbol);
        if (DecodedFile.tellp() >= DecodedFileSize) break;
        if (!OrderFall && (BYTE*) FoundState->Successor > pText)
                MinContext=MaxContext=FoundState->Successor;
        else {
            UpdateModel();
            if (EscCount == 0)              
				ClearMask();
        }
        ARI_DEC_NORMALIZE(EncodedFile);
    }
STOP_DECODING: ;
    //PrintInfo(DecodedFile,EncodedFile);
}
