/****************************************************************************
 *  This file is part of PPMd project                                       *
 *  Written and distributed to public domain by Dmitry Shkarin 1997,        *
 *  1999-2001                                                               *
 *  Contents: interface to encoding/decoding routines                       *
 *  Comments: this file can be used as an interface to PPMd module          *
 *  (consisting of Model.cpp) from external program           				*
 ****************************************************************************/
#if !defined(_PPMD_H_)
#define _PPMD_H_

#include "PPMdType.h"
#include <iostream>

BOOL  _STDCALL StartSubAllocator(int SubAllocatorSize);
void  _STDCALL StopSubAllocator();          // it can be called once
DWORD _STDCALL GetUsedMemory();             // for information only

// (MaxOrder == 1) parameter value has special meaning, it does not restart
// model and can be used for solid mode archives;
// Call sequence:
//	StartSubAllocator(SubAllocatorSize);
//  EncodeFile(SolidArcFile,File1,MaxOrder);
//  EncodeFile(SolidArcFile,File2,1);
//  ...
//  EncodeFile(SolidArcFile,FileN,1);
//  StopSubAllocator();
void _STDCALL EncodeFile(std::ostream & EncodedFile, 
						 std::istream & DecodedFile,
						 int MaxOrder);
void _STDCALL DecodeFile(std::ostream & DecodedFile, 
						 std::istream & EncodedFile,
						 int MaxOrder, std::size_t DecodedFileSize);


#endif /* !defined(_PPMD_H_) */
