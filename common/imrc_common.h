/*
** ===========================================================================
** File: imrc_common.h
** Description: Common IMRC functions header.
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 08/02/2024	raulmrio28-git	Reorganization
** 07/31/2024	raulmrio28-git	64-bit fix for QuramFlashRead
** 05/16/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef TKCOMMON_H_
#define TKCOMMON_H_
/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#include "../common/typedef.h"
#include <stdio.h>
#include <stddef.h>

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define QURAM_SUCCESS			1
#define QURAM_FAIL				-1
#define IMASTER_SUCCESS			QURAM_SUCCESS
#define IMASTER_FAIL			QURAM_FAIL
#define ImasterFlashRead(d,a,l)	QuramFlashRead(a,d,l)
#define ImasterMemAlloc			QuramMemAlloc
#define ImasterMemFree			QuramMemFree
#define ImasterMemcpy			QuramMemcpy
#define AcMemcpyEx				QuramMemcpy

#define ALLOC_CHECK(val,sz,type,r)  val = (type ##)ImasterMemAlloc(sz); \
									if(!val) \
										return r;
#define FREE(val)					if(val) \
									{ \
										ImasterMemFree(val); \
										val = 0;\
									}	

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

VOID*	QuramFlashRead(VOID* pAddress, VOID* pDest, SIZE_T length);
VOID*	QuramMemAlloc(SIZE_T size);
VOID    QuramMemFree(VOID* pMem);
LONG	QuramMemcpy(VOID* pDest, CONST VOID* pSrc, SIZE_T length);

#endif //TKCOMMON_H