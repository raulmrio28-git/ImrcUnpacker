/*
** ===========================================================================
** File: tkcommon.c
** Description: Common TK functions code.
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 05/16/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "tkcommon.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

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
**  Function(internal use only) Declarations
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Function(external use only) Declarations
**----------------------------------------------------------------------------
*/

VOID*	QuramFlashRead(VOID* pAddress, VOID* pDest, SIZE_T length)
{
	return QuramMemcpy(pDest, pAddress, length);
}

VOID*   QuramMemAlloc(SIZE_T size)
{
    return malloc(size);
}

VOID    QuramMemFree(VOID* pMem)
{
	free(pMem);
}

LONG	QuramMemcpy(VOID* pDest, const VOID* pSrc, SIZE_T length)
{
	return memcpy(pDest, pSrc, length);
}