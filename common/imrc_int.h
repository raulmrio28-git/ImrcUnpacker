/*
** ===========================================================================
** File: imrc_int.h
** Description: IMRC header file
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 05/16/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef IMRC_INT_H
#define IMRC_INT_H
/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "../common/typedef.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define BSWAP_WORD(value)			((value>>8) | ((value&0xff)<<8))
#define VERSION_TO_DOUBLE(nMajVer, nMinVer) (double)(nMajVer + ((double)nMinVer/10))
#define VERSION_TO_DOUBLE_WRD(value) (double)((value>>8) + ((double)(value&0xff)/10))
#define IMRC_MAGIC	'CRMI'

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct {
	LONG nCmdSize;
	LONG nDataSize;
} QuramDataDecomp_BlockInfo;

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

#endif //IMRC_INT_H