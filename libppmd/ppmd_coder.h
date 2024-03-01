#ifndef PPMD_CODER_HEADER_0F508BAB_5509_11d5_83F2_000102A39096
#define PPMD_CODER_HEADER_0F508BAB_5509_11d5_83F2_000102A39096

/** PPMD_Coder is a class to compress and uncompress a file using the very
sophisticated PPMD method. It is based on the public domain code by Dmitry 
Shkarin which is widely available. Please notice that only one file can
compressed, archiving several files can be done with the help of TAR.

There is no general rule which OrderSize to use. If you have highly repeatable 
data it's worth to use a high value but in general this should be avoided
because the overhead is slightly greater and would increase your output file
compared to smaller OrderSizes. Just play around with the values to see what
I mean.

The SubAllocatorSize is the size in MB which will be allocated dynamically
on the heap. Only use high values if you have enough memory.

You must include the "ppmd.lib" in your project!

Please send any bugs to:
Andreas Muegge <andreas.muegge@gmx.de> or <andreas@starke-muegge.de>

2001-30-05
*/

#include <iostream>
#include <wtypes.h>
#include <cstdio>
#include <string>

// functions implemented in the static library "ppmd.lib"
BOOL  __stdcall StartSubAllocator(int SubAllocatorSize);
void  __stdcall StopSubAllocator();
DWORD __stdcall GetUsedMemory();

void __stdcall EncodeFile(std::ostream & EncodedFile, 
						  std::istream & DecodedFile,
						  int MaxOrder);

void __stdcall DecodeFile(std::ostream & DecodedFile, 
						  std::istream & EncodedFile,
						  int MaxOrder, std::size_t DecodedFileSize);

class PPMD_Coder
{
public:
	/** Parameters are:
	szIn		- file to read in
	szOut		- file to write compressed/uncompressed data to. If this
				  parameter is NULL the extension "ppm" will be added 
				  (Compression) or the original filename stored in the 
				  archive will be used (Decompression)
	nSub		- Memory to use for compression in MBytes (1..256)
	nOrder		- Ordersize (2..16)
	*/
	PPMD_Coder(int nSub = 0x10000 * sizeof(void*) / 4, int nOrder = 5 ) 
		: m_nOrder(nOrder), m_nSubAllocatorSize(nSub)
	{}
	
	~PPMD_Coder() { }

	/** Play around with the order sizes. Higher values require more 
	overhead so you must find the right balance. Values between 4 and 6
	are good starting points. */
	int	OrderSize(int nOrder) 
	{
		m_nOrder = nOrder; 
		m_nOrder = max(nOrder, 2);	// make sure that nOrder > 0
		m_nOrder = min(nOrder, 16);	// make sure that nOrder <= 16
		return m_nOrder;
	}

	/** Specify how much memory (in MBytes) can be used. */
	int	SubAllocatorSize(int nSub)
	{
		m_nSubAllocatorSize = nSub;

		return m_nSubAllocatorSize;
	}

	/** Access functions for OrderSize and SubAllocatorSize */
	int OrderSize() { return m_nOrder; }
	int SubAllocatorSize() {return m_nSubAllocatorSize; }

	/** Compress the file, return FALSE if an error occured */
	bool	Compress(std::istream &Infile, std::ostream &Outfile);

	/** Uncompress file. */
	bool	Uncompress(std::istream &Infile, std::ostream &Outfile, std::size_t DecodedFileSize);

private:
	int				m_nOrder;
	int				m_nSubAllocatorSize;
};


//////////////////////////////////////////////////////////////////////////
bool PPMD_Coder::Compress(std::istream &Infile, std::ostream &Outfile) 
{
	if (!StartSubAllocator(m_nSubAllocatorSize))
	{
		return FALSE;
	}
	
	EncodeFile(Outfile, Infile, m_nOrder);
	
	StopSubAllocator();
	
	return TRUE;
}


bool PPMD_Coder::Uncompress(std::istream &Infile, std::ostream &Outfile, std::size_t DecodedFileSize)
{
	if (!StartSubAllocator(m_nSubAllocatorSize))
	{
		return FALSE;
	}
	
	DecodeFile(Outfile, Infile, m_nOrder, DecodedFileSize);
	
	StopSubAllocator();
	
	return TRUE;
}



#endif
