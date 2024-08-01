/*
** ===========================================================================
** File: tk10.h
** Description: TK1.0 functions externs.
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 07/28/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef TK10_H_
#define TK10_H_

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

#ifdef TK_EXPORTS
#define TK_API DLLEXPORT
#else
#define TK_API DLLIMPORT
#endif

#define REG_DIST_BITS				6
#define EXT_DIST_BITS				11

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

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

BOOL	TK_API	Tk10_DataDecompInit(BYTE* pData, LONG nSize);
LONG	TK_API	Tk10_DataDecompress(BYTE* pBlock, LONG nBlockSize,
									BYTE* pOutBlock);
VOID	TK_API	Tk10_GetDecompInfo(LONG* pnBlockSize, LONG* pnOutSize);
VOID	TK_API	Tk10_DataDecompShutdown();

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //TK10_H