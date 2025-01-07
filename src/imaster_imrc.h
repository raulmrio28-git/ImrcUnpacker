/*
** ===========================================================================
** File: imaster_imrc.h
** Description: I-master IMRC function externs.
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 08/02/2024	raulmrio28-git	Reorganization
** 07/28/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef _IMASTER_IMRC_H_
#define _IMASTER_IMRC_H_

/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#include "../common/typedef.h"
#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#if defined(COMPILE_FOR_DLL)
#if defined(_MSC_VER)
//  Microsoft 
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLIMPORT
#else
//  do nothing and hope for the best?
#define DLLEXPORT
#define DLLIMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif
#else
#define DLLEXPORT
#define DLLIMPORT
#endif

#ifdef TK_EXPORTS
#define IMRCAPI DLLEXPORT
#else
#define IMRCAPI DLLIMPORT
#endif

#define REG_DIST_BITS				6
#define EXT_DIST_BITS				11

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct {
	DWORD			nDecompSize;
	DWORD			nBlockSize;
} ImasterDataDecomp_HdrInfo;

/*
**----------------------------------------------------------------------------
**  Variable Declarations
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Function(external use only) Declarations
**----------------------------------------------------------------------------
*/

LONG	IMRCAPI	ImasterDataDecompInit(BYTE* pData);
LONG	IMRCAPI	ImasterDataDecompress(BYTE* pBlock, LONG nOutSize,
									  WORD* pOutBlock);

LONG	IMRCAPI	ImasterDataDecomp_GetHeaderInfo
				(ImasterDataDecomp_HdrInfo* pInfo);
LONG	IMRCAPI	ImasterDataDecomp_Buf(BYTE* pIn, WORD* pOut);
LONG	IMRCAPI	ImasterDataDecomp_Buf_GetHeaderInfo
				(BYTE* pData, ImasterDataDecomp_HdrInfo* pInfo);
VOID	IMRCAPI	ImasterDataDecompShutdown();

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //_IMASTER_IMRC_H_