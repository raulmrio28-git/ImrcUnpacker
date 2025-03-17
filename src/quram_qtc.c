/*
** ===========================================================================
** File: quram_qtc.h
** Description: Code for Quram QTC handling.
** Copyright (c) 2025 raulmrio28-git and contributors.
** Format Copyright (C) 2010 Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 03/17/2025	raulmrio28-git	Initial version
** ===========================================================================
*/

/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#include "../common/imrc_common.h"
#include "../common/imrc_int.h"
#include "../common/bit_handle.h"
#include "quram_qtc.h"
#include <stdbool.h>
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/			

//main decompressor
#define MBK_OFFS					((curr_mbk << 1) + curr_mbk_wrd)

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

LOCAL	WORD						QuramDDC_diffTable[] =
{
	0x0001, 0x0003, 0x0100, 0x0002, 0x0008, 0x0007, 0x0006, 0x0300,
	0x0010, 0x0004, 0x0200, 0x0009, 0x0040, 0x0018, 0x0005, 0x0020,
	0x000C, 0x000E, 0x000F, 0x000A, 0x00C0, 0x0800, 0x0700, 0x0101,
	0x0400, 0x000B, 0x0030, 0x0011, 0x0080, 0x0600, 0x000D, 0x0012,
	0x001C, 0x0500, 0x001B, 0x001E, 0x0014, 0x001A, 0x0028, 0x0038,
	0x1000, 0x001F, 0x0019, 0x0016, 0x0060, 0x2000, 0x0013, 0x001D,
	0x0103, 0x0024, 0x0017, 0x0015, 0x0102, 0x01C0, 0x0F00, 0x003C,
	0x0301, 0x0C00, 0x1800, 0x0048, 0x0021, 0x0034, 0x0E00, 0x0202,
	0x002C, 0x0070, 0x0A00, 0x0303, 0x0036, 0x0201, 0x003F, 0x0D00,
	0x0180, 0x003E, 0x3000, 0x0900, 0x0078, 0x0022, 0x0050, 0x003A,
	0x0041, 0x0107, 0x0033, 0x0106, 0x0026, 0x002A, 0x00A0, 0x0023,
	0x0029, 0x0088, 0x0044, 0x003D, 0x00E0, 0x0032, 0x002E, 0x0039,
	0x0031, 0x002D, 0x00F0, 0x0140, 0x0B00, 0x003B, 0x0058, 0x4000,
	0x0037, 0x0035, 0x0068, 0x0302, 0x007C, 0x002F, 0x0027, 0x0064,
	0x0090, 0x0074, 0x0203, 0x0104, 0x006C, 0x1100, 0x03C0, 0x00FF,
	0x0025, 0xF000, 0x1F00, 0x0701, 0x0042, 0x007F, 0x002B, 0x0105,
	0x0054, 0x1C00, 0x004C, 0x0801, 0x0043, 0x6000, 0x005C, 0x007E,
	0x00E8, 0x0108, 0x00F8, 0xE000, 0x0206, 0x1E00, 0x0380, 0x0061,
	0x007A, 0x004E, 0x0601, 0x1001, 0x00C8, 0x8000, 0x1D00, 0x00D0,
	0x0072, 0x0049, 0x1600, 0x1A00, 0x0046, 0x7000, 0x010F, 0x0110,
	0x0076, 0x1200, 0x1400, 0x0404, 0x0606, 0x010E, 0x00FC, 0x1700,
	0x006E, 0x00FE, 0x1300, 0x0062, 0x0066, 0xC000, 0x0204, 0x0306,
	0x0063, 0x0707, 0x0280, 0x0602, 0x0055, 0x0047, 0x006A, 0x010C,
	0x0052, 0x0501, 0x00D8, 0x0307, 0x0073, 0x0109, 0x0808, 0x0401,
	0x004A, 0x2020, 0x005A, 0x0702, 0x00B0, 0x0045, 0x0207, 0x0304,
	0x0402, 0x005E, 0x010A, 0x0079, 0x3800, 0x00F4, 0x1500, 0x01E0,
	0x1B00, 0x0071, 0x1010, 0x00C1, 0x00E4, 0x0502, 0x0056, 0x007D,
	0x0081, 0x0077, 0x00CC, 0x0703, 0x010D, 0x0205, 0x0340, 0x5000,
	0x0082, 0x0067, 0xFF00, 0x0120, 0x0069, 0x0098, 0x00C3, 0x1900,
	0x0065, 0x007B, 0x0240, 0x0603, 0x00EC, 0x0059, 0x00FA, 0x0403,
	0x0075, 0x006F, 0x3100, 0x3300, 0x004F, 0x00B8, 0x006D, 0x0208,
	0x004D, 0x0111, 0x0051, 0x020E, 0x00DC, 0x00C4, 0x2100, 0x00A8
};

/*
**----------------------------------------------------------------------------
**  Function(internal use only) Declarations
**----------------------------------------------------------------------------
*/

LOCAL	DWORD	Quram_DataDeCompress(BYTE* pInBlock, BYTE* pOutData)
{
	BYTE*		pCmdBuf;
	BYTE*		pCurrOut;
	DWORD		nCurrBits;
	DWORD		bCmdBytes;
	DWORD		nOffs;
	DWORD		nUnpMblks, nTotalMblks;
	DWORD		nFQMBBits, nRQMBBits;
	DWORD 		nOutSize;
	DWORD 		curr_mbk;
	DWORD 		curr_mbk_wrd;
	DWORD 		nExtDistBits = 12;
	DWORD 		nStdDistBits = 6;
	pCurrOut = pOutData;
	nRQMBBits = nExtDistBits + 4;
	nFQMBBits = nExtDistBits + 6;
	nCurrBits = 0;
	bCmdBytes = 0;
	nUnpMblks = 0;
	pCmdBuf = pInBlock+2*sizeof(DWORD);
	nOffs = 4;
	nOutSize = pInBlock[0] | (pInBlock[1] << 8) | (pInBlock[2] << 16)
			 | (pInBlock[3] << 24);
	nTotalMblks = 4 * (nOutSize >> 4);
	pInBlock += (pInBlock[4] | (pInBlock[5] << 8) | (pInBlock[6] << 16)
			 | (pInBlock[7] << 24)) + 2*sizeof(DWORD);
	//starting quad-macro-block
	for (curr_mbk = 0; curr_mbk < 4; curr_mbk++)
	{
		if (curr_mbk)
		{
			BOOL sameoffs;
			CHK_BITS(nFQMBBits);
			EXTRACT_BIT(sameoffs);
			if (!sameoffs)
			{
				BOOL stdoffs;
				EXTRACT_BIT(stdoffs);
				EXTRACT_BITS(nOffs, (stdoffs
									 ? nStdDistBits : nExtDistBits));
			}
			for (curr_mbk_wrd=0;curr_mbk_wrd<2;curr_mbk_wrd++)
			{
				BOOL stdlz;
				EXTRACT_BIT(stdlz);
				if (stdlz)
					*(WORD*)pCurrOut = *(WORD*)(pCurrOut-nOffs);
				else
				{
					BOOL literal;
					EXTRACT_BIT(literal);
					*(WORD*)pCurrOut = (literal 
									  ? *(WORD*)pInBlock
									  :	QuramDDC_diffTable[*pInBlock]
									  ^ *(WORD*)(pCurrOut-nOffs));
					pInBlock += (literal ? 2 : 1);
				}
				pCurrOut += sizeof(WORD);
			}
		}
		else
		{
			AcMemcpyEx(pCurrOut, pInBlock, sizeof(WORD)*2);
			pCurrOut += 2*sizeof(WORD);
			pInBlock += 2*sizeof(WORD);
		}
		nUnpMblks++;
	}
	while (nUnpMblks < nTotalMblks)
	{
		BOOL mixed;
		EXTRACT_BIT(mixed);
		if (mixed)
		{
			BYTE stdlz_bits = *pInBlock;
			pInBlock++;
			for (curr_mbk=0;curr_mbk<4;curr_mbk++)
			{
				BOOL sameoffs;
				CHK_BITS(nRQMBBits);
				EXTRACT_BIT(sameoffs);
				if (!sameoffs)
				{
					BOOL extdoffs;
					EXTRACT_BIT(extdoffs);
					EXTRACT_BITS(nOffs, (extdoffs 
								? nStdDistBits : nExtDistBits));
				}
				for(curr_mbk_wrd=0;curr_mbk_wrd<2;
					curr_mbk_wrd++)
				{
					if ((stdlz_bits & (1 << (7 - MBK_OFFS))))
					{
						*(WORD*)pCurrOut=*(WORD*)(pCurrOut-nOffs);
					}
					else
					{
						BOOL literal;
						EXTRACT_BIT(literal);
						*(WORD*)pCurrOut=(literal ? *(WORD*)pInBlock
										: QuramDDC_diffTable[*pInBlock]
										^*(WORD*)(pCurrOut-nOffs));
						pInBlock += (literal ? 2 : 1);
					}
					pCurrOut += sizeof(WORD);
				}
			}
		}
		else
		{
			BOOL lz;
			char curr_byte;
			EXTRACT_BIT(lz);
			for (curr_byte = 0; curr_byte < 16; curr_byte++)
			{
				if (lz)
					*pCurrOut = *(pCurrOut-nOffs);
				else
				{
					*pCurrOut = *pInBlock;
					pInBlock++;
				}
				pCurrOut++;
			}
		}
		nUnpMblks += 4;
	}
	if (nOutSize > (16 * (nOutSize >> 4)))
		QuramMemcpy(pCurrOut, pInBlock, nOutSize - (16 * (nOutSize >> 4)));
	return nOutSize;
}

/*
**----------------------------------------------------------------------------
**  Function(external use only) Declarations
**----------------------------------------------------------------------------
*/

VOID			Quram_GetTableSize
				(BYTE* pData, DWORD* pnUnpTextSize, DWORD* pnUnpMapSize)
{
	DWORD		nDataOffset;
	nDataOffset = (pData[3] << 24) | (pData[2] << 16) | (pData[1] << 8)
				| *pData;
	*pnUnpMapSize = (pData[11] << 24) | (pData[10] << 16) | (pData[9] << 8)
				  | pData[8];
	*pnUnpTextSize = (pData[nDataOffset + 11] << 24)
				   | (pData[nDataOffset + 10] << 16)
				   | (pData[nDataOffset + 9] << 8)
				   | pData[nDataOffset + 8];
}

LONG			Quram_DecodeTextComp(BYTE* pIn, BYTE* pOutText, BYTE* pOutMap)
{
	DWORD		nOutMapSize;
	DWORD		nOutDataSize;
	DWORD		nOutMapSizeCheck;
	DWORD		nOutDataSizeCheck;
	DWORD		nDataOffset;
	
	nDataOffset = (pIn[3] << 24) | (pIn[2] << 16) | (pIn[1] << 8) | *pIn;
	Quram_GetTableSize(pIn, &nOutDataSize, &nOutMapSize);
	nOutMapSizeCheck = Quram_DataDeCompress(pIn+(2*sizeof(DWORD)), pOutMap);
	if (nOutMapSizeCheck != nOutMapSize)
		return QURAM_FAIL;
	nOutDataSizeCheck = Quram_DataDeCompress(pIn+nDataOffset+(2*sizeof(DWORD)),
										  pOutText);
	if (nOutDataSizeCheck != nOutDataSize)
		return QURAM_FAIL;
	return QURAM_SUCCESS;
}