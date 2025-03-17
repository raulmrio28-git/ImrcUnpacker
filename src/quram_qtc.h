/*
** ===========================================================================
** File: quram_qtc.h
** Description: Quram QTC function externs.
** Copyright (c) 2025 raulmrio28-git and contributors.
** Format Copyright (C) 2010 Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 03/17/2025	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef _QURAM_QTC_H_
#define _QURAM_QTC_H_

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
#define QTCAPI DLLEXPORT
#else
#define QTCAPI DLLIMPORT
#endif

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

VOID	QTCAPI	Quram_GetTableSize
				(BYTE* pData, DWORD* pnUnpTextSize, DWORD* pnUnpMapSize);
LONG	QTCAPI	Quram_DecodeTextComp(BYTE* pIn, BYTE* pOutText, BYTE* pOutMap);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //TK15_H