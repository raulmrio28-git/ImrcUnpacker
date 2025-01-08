/*
** ===========================================================================
** File: quram_imrc.h
** Description: Quram IMRC function externs.
** Copyright (c) 2024-2025 raulmrio28-git and contributors.
** Format Copyright (C) 2008 Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 01/07/2025	raulmrio28-git	Add missing functions
** 08/02/2024	raulmrio28-git	Reorganization
** 07/28/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef _QURAM_IMRC_H_
#define _QURAM_IMRC_H_

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

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct {
	DWORD			nDecompSize;
	DWORD			nBlockSize;
} QuramDataDecomp_HdrInfo;

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

LONG	IMRCAPI	QuramDataDecompInit(BYTE* pData);
LONG	IMRCAPI	QuramDataDecompress(BYTE* pBlock, LONG nBlockSize,
									WORD* pOutBlock);
LONG	IMRCAPI	QuramDataDecomp_GetHeaderInfo(QuramDataDecomp_HdrInfo* pInfo);
LONG	IMRCAPI	QuramDataDecomp_Buf(BYTE* pIn, WORD* pOut);
LONG	IMRCAPI	QuramDataDecomp_Buf_GetHeaderInfo
				(BYTE* pData, QuramDataDecomp_HdrInfo* pInfo);
VOID	IMRCAPI	QuramDataDecompShutdown();

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //TK15_H