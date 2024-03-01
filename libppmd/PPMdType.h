/****************************************************************************
 *  This file is part of PPMd project                                       *
 *  Written and distributed to public domain by Dmitry Shkarin 1997,        *
 *  1999-2001                                                               *
 *  Contents: compilation parameters and miscelaneous definitions           *
 *  Comments: system & compiler dependent file                              *
 ****************************************************************************/
#if !defined(_PPMDTYPE_H_)
#define _PPMDTYPE_H_

const int MAX_O=16;                         /* maximum allowed model order */

#define _WIN32_ENVIRONMENT_

#if defined(_WIN32_ENVIRONMENT_)
#include <wtypes.h>
#else /* _DOS32_ENVIRONMENT_ || _POSIX_ENVIRONMENT_ || _UNKNOWN_ENVIRONMENT_ */
typedef int   BOOL;
#define FALSE 0
#define TRUE  1
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   UINT;
#endif /* defined(_WIN32_ENVIRONMENT_)  */

#if !defined(_UNKNOWN_ENVIRONMENT_) && !defined(__GNUC__)
#define _FASTCALL __fastcall
#define _STDCALL  __stdcall
#else
#define _FASTCALL
#define _STDCALL
#endif /* !defined(_UNKNOWN_ENVIRONMENT_) && !defined(__GNUC__) */

#if defined(__GNUC__)
#define _PACK_ATTR __attribute__ ((packed))
#else
#define _PACK_ATTR
#endif /* defined(__GNUC__) */

template <class T>
inline void _PPMD_SWAP(T& t1,T& t2) { T tmp=t1; t1=t2; t2=tmp; }

#endif /* !defined(_PPMDTYPE_H_) */
