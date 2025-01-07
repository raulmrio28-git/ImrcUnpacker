/*
** ===========================================================================
** File: imrc_int.h
** Description: IMRC header file
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 08/02/2024	raulmrio28-git	Reorganization
** 07/28/2024	raulmrio28-git	TK 1.0 compatibility (block offsets instead of
**								sizes)						
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
#define B1(value)					(value&0xff)
#define B2(value)					((value>>8)&0xff)
#define B3(value)					((value>>16)&0xff)
#define B4(value)					((value>>24)&0xff)
#define VER2DOUBLE(dwVersion)		(B4(dwVersion)+(((B2(dwVersion)*10) \
									+B3(dwVersion))/100))
#define IMRC_MAGIC					'CRMI'
#define BKI_SIZE					2*sizeof(DWORD)

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct {
	LONG nCmdOffs;
	LONG nDataOffs;
} ImasterDataDecomp_BlockInfo;

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