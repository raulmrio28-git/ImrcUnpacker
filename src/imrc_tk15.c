/*
** ===========================================================================
** File: imrc_tk15.c
** Description: Code for TkTool1.5+ IMRC file handling.
** Copyright (c) 2024 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 05/15/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#include "../common/tkcommon.h"
#include "../common/imrc_int.h"
#include "../common/bit_handle.h"
#include "tk15.h"
#include <stdbool.h>
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

typedef struct {
	LONG			Magic;
	WORD			nMinVer;
	WORD			nMajVer;
	DWORD			nBlkSize;
	DWORD			nDistStdBits;
	DWORD			nDistExtBits;
	DWORD			nTotalDecSize;
	DWORD			nBlocks;
} tk15_IMRCHeader;

/*
**----------------------------------------------------------------------------
**  Variable Declarations
**----------------------------------------------------------------------------
*/

LOCAL	QuramDataDecomp_BlockInfo*	QuramDataDecomp_BlockInfos;
LOCAL	BYTE*						QuramDataDecomp_Input;
LOCAL	DWORD						QuramDataDecomp_BlkSize = 0;
LOCAL	DWORD						QuramDataDecomp_StdDistBits = 0;
LOCAL	DWORD						QuramDataDecomp_ExtDistBits = 0;
LOCAL	DWORD						QuramDataDecomp_DecompSize = 0;
LOCAL	DWORD						QuramDataDecomp_Blocks = 0;
LOCAL	DWORD*						QuramDataDecomp_BlockOffsets;

LOCAL	WORD						diffTbl[] =
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

LOCAL	DWORD	IM_DataDecompress(BYTE* pInBlock, DWORD nOutSize, 
								  BYTE* pOutData, DWORD nDistStdBits,
								  DWORD nDistExtBits, DWORD nBlkSize,
								  DWORD nBlock)
{
	BYTE* pCmdBuf;
	BYTE* pCurrOut;
	DWORD nCurrBits;
	DWORD bCmdBytes;
	DWORD nOffs;
	DWORD nTotalBlks;
	DWORD nUnpMblks, nTotalMblks;
	DWORD nFQMBBits, nRQMBBits;
	pCurrOut = pOutData;
	nTotalBlks = nOutSize / nBlkSize + ((nOutSize % nBlkSize)!=0);
	nRQMBBits = nDistStdBits + 4;
	nFQMBBits = nDistStdBits + 6;
	while (nTotalBlks)
	{
		nTotalBlks--;
		if (QuramDataDecomp_BlockInfos[nBlock].nCmdSize)
		{
			DWORD nCurrMacroBlk;
			DWORD nCurrMacroWord;
			nCurrBits = 0;
			bCmdBytes = 0;
			pCmdBuf = pInBlock;
			nOffs = 4;
			if (!nTotalBlks && (((nOutSize - (nBlkSize
								* (nOutSize / nBlkSize))) >> 4) << 2) != 0)
				nTotalMblks = ((nOutSize - (nBlkSize
								* (nOutSize / nBlkSize))) >> 4) << 2;
			else
				nTotalMblks = nBlkSize >> 2;
			pInBlock += QuramDataDecomp_BlockInfos[nBlock].nCmdSize;
			//starting quad-macro-block
			for (nCurrMacroBlk = 0; nCurrMacroBlk < 4; nCurrMacroBlk++)
			{
				if (nCurrMacroBlk)
				{
					BOOL sameoffs;
					CHK_BITS(nFQMBBits);
					EXTRACT_BIT(sameoffs);
					if (!sameoffs)
					{
						BOOL extdoffs;
						EXTRACT_BIT(extdoffs);
						EXTRACT_BITS(nOffs, (extdoffs
											 ? nDistExtBits : nDistStdBits));
					}
					for (nCurrMacroWord=0;nCurrMacroWord<2;nCurrMacroWord++)
					{
						BOOL stdlz;
						EXTRACT_BIT(stdlz);
						if (stdlz)
							*(WORD*)pCurrOut = *(WORD*)(pCurrOut-nOffs);
						else
						{
							BOOL literal;
							EXTRACT_BIT(literal);
							*(WORD*)&pCurrOut = (literal 
											  ? *(WORD*)pInBlock
											  :	diffTbl[*pInBlock]
											  ^ *(WORD*)(pCurrOut-nOffs));
							pInBlock += (literal ? 2 : 1);
						}
						pCurrOut += sizeof(WORD);
					}
				}
				else
				{
					for (nCurrMacroWord=0;nCurrMacroWord<2;nCurrMacroWord++)
					{
						AcMemcpyEx(pCurrOut, pInBlock, 2);
						pCurrOut += sizeof(WORD);
						pInBlock += sizeof(WORD);
					}
				}
				nUnpMblks++;
			}
			while (nUnpMblks < nTotalMblks)
			{
				BOOL mixed;
				EXTRACT_BIT(mixed);
				if (mixed)
				{
					unsigned char stdlz_bits = *pInBlock;
					pInBlock++;
					for (nCurrMacroBlk=0;nCurrMacroBlk<4;nCurrMacroBlk++)
					{
						BOOL sameoffs;
						CHK_BITS(nRQMBBits);
						EXTRACT_BIT(sameoffs);
						if (!sameoffs)
						{
							BOOL extdoffs;
							EXTRACT_BIT(extdoffs);
							EXTRACT_BITS(nOffs, (extdoffs 
										? nDistExtBits : nDistStdBits));
						}
						for(nCurrMacroWord=0;nCurrMacroWord<2;
							nCurrMacroWord++)
						{
							if ((stdlz_bits & (1 << (7
													- ((nCurrMacroBlk << 1)
													+ nCurrMacroWord)))))
							{
								*(WORD*)pCurrOut=*(WORD*)(pCurrOut-nOffs);
							}
							else
							{
								BOOL literal;
								EXTRACT_BIT(literal);
								*(WORD*)&pCurrOut=(literal ? *(WORD*)pInBlock
												: diffTbl[*pInBlock]
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
		}
		else
		{
			AcMemcpyEx(pCurrOut, pInBlock,
								QuramDataDecomp_BlockInfos[nBlock].nDataSize);
			pCurrOut += QuramDataDecomp_BlockInfos[nBlock].nDataSize;
			pInBlock += QuramDataDecomp_BlockInfos[nBlock].nDataSize;
		}
		nBlock++;
	}
	if (QuramDataDecomp_BlockInfos[nBlock - 1].nCmdSize)
	{
		if (nOutSize - (nBlkSize * (nOutSize / nBlkSize)) > nTotalMblks << 2)
			AcMemcpyEx(pCurrOut, pInBlock,
					  (nOutSize - (nBlkSize * (nOutSize / nBlkSize)))
					  - (nTotalMblks << 2));
	}
	return nOutSize + ((nUnpMblks - nTotalMblks) << 2);
}

BOOL	QuramDataDecompInit(BYTE* pData, LONG sz)
{
	int					i;
	tk15_IMRCHeader*	pstHeader;
	bool				bResult = FALSE;
	QuramDataDecomp_Input = pData;
	pstHeader = (tk15_IMRCHeader*)pData;
	if (pstHeader->Magic == IMRC_MAGIC
		&& (VERSION_TO_DOUBLE(BSWAP_WORD(pstHeader->nMajVer),
			BSWAP_WORD(pstHeader->nMinVer)) == 2.4
			|| VERSION_TO_DOUBLE(BSWAP_WORD(pstHeader->nMajVer),
			BSWAP_WORD(pstHeader->nMinVer)) == 2.5
			|| VERSION_TO_DOUBLE_WRD(pstHeader->nMinVer) >= 3.1))
	{
		bResult = TRUE;
		QuramDataDecomp_BlkSize = pstHeader->nBlkSize;
		QuramDataDecomp_StdDistBits = pstHeader->nDistStdBits;
		QuramDataDecomp_ExtDistBits = pstHeader->nDistExtBits;
		QuramDataDecomp_DecompSize = pstHeader->nTotalDecSize;
		QuramDataDecomp_Blocks = pstHeader->nBlocks;
		FREE(QuramDataDecomp_BlockInfos);
		ALLOC_CHECK(QuramDataDecomp_BlockInfos,
					sizeof(QuramDataDecomp_BlockInfo)
				  * QuramDataDecomp_Blocks, QuramDataDecomp_BlockInfo*)
		FREE(QuramDataDecomp_BlockOffsets);
		ALLOC_CHECK(QuramDataDecomp_BlockOffsets,
					sizeof(DWORD) * (QuramDataDecomp_Blocks + 1), DWORD*)
		QuramMemcpy(QuramDataDecomp_BlockInfos,
						pData + sizeof(tk15_IMRCHeader),
						sizeof(QuramDataDecomp_BlockInfo)
					  * QuramDataDecomp_Blocks);
		*QuramDataDecomp_BlockOffsets = sizeof(QuramDataDecomp_BlockInfo)
									  * QuramDataDecomp_Blocks
									  + sizeof(tk15_IMRCHeader);
		for (i = 1; i <= QuramDataDecomp_Blocks; ++i)
			QuramDataDecomp_BlockOffsets[i]=QuramDataDecomp_BlockOffsets[i-1]
			+ QuramDataDecomp_BlockInfos[i - 1].nCmdSize
			+ QuramDataDecomp_BlockInfos[i - 1].nDataSize;
	}
	return bResult;
}

LONG	QuramDataDecompress(BYTE* pBlock, LONG nBlockSize, BYTE* pOutBlock)
{
	int		i;
	DWORD	nBlkOffsetFromInData;
	LONG	nStartBlk;
	LONG	nBlks;
	BYTE*	pBlockData;
	BYTE*	pDecoderOut;
	DWORD	nPackedSize;
	DWORD	nUnpSize;
	DWORD	nActualUnpSize;
	DWORD	nOutSize;

	nBlkOffsetFromInData = pBlock - QuramDataDecomp_Input;
	nStartBlk = nBlkOffsetFromInData / QuramDataDecomp_BlkSize;
	if (nStartBlk > QuramDataDecomp_Blocks - 1)
		return -1;
	if (QuramDataDecomp_DecompSize <= QuramDataDecomp_BlkSize)
	{
		nBlks = 1;
	}
	else
	{
		MAXLIMIT(nBlockSize, QuramDataDecomp_DecompSize)
		nBlks = (nBlockSize + nStartBlk + QuramDataDecomp_BlkSize - 1)
				 / QuramDataDecomp_BlkSize;
		for (i = (&pBlock[-nStartBlk] - QuramDataDecomp_Input)
			   + (QuramDataDecomp_BlkSize * (nBlks - 1));
		     i > QuramDataDecomp_DecompSize; i -= QuramDataDecomp_BlkSize)
			nBlks--;
	}
	ALLOC_CHECK(pDecoderOut, QuramDataDecomp_BlkSize * nBlks, BYTE*, -1)
	nPackedSize = QuramDataDecomp_BlockOffsets[nStartBlk + nBlks]
					- QuramDataDecomp_BlockOffsets[nStartBlk];
	ALLOC_CHECK(pBlockData, nPackedSize, BYTE*, -1)
	QuramFlashRead(pBlockData,
			  &QuramDataDecomp_Input[QuramDataDecomp_BlockOffsets[nStartBlk]],
			  nPackedSize);
	nActualUnpSize = QuramDataDecomp_DecompSize
				   - (&pBlock[-nStartBlk] - QuramDataDecomp_Input);
	nUnpSize = nBlks * QuramDataDecomp_BlkSize;
	nUnpSize = MIN(nActualUnpSize, (nBlks * QuramDataDecomp_BlkSize));
	IM_DataDecompress(pBlockData, nUnpSize, pDecoderOut,
					  QuramDataDecomp_StdDistBits,QuramDataDecomp_ExtDistBits,
					  QuramDataDecomp_BlkSize, nStartBlk);
	nOutSize = MIN(nBlockSize, (nActualUnpSize - nStartBlk));
	ImasterMemcpy(pOutBlock, &pDecoderOut[nStartBlk], nOutSize);
	ImasterMemFree(pDecoderOut);
	ImasterMemFree(pBlockData);
	return nOutSize;
}

BOOL	QuramDataDecompShutdown()
{
	FREE(QuramDataDecomp_BlockInfos);
	FREE(QuramDataDecomp_BlockOffsets);
	return TRUE;
}

/*
**----------------------------------------------------------------------------
**  Function(external use only) Declarations
**----------------------------------------------------------------------------
*/

BOOL	Tk15_DataDecompInit(BYTE* pData, LONG nSize)
{
	return QuramDataDecompInit(pData, nSize);
}

LONG	Tk15_DataDecompress(BYTE* pBlock, LONG nBlockSize, 
								 BYTE* pOutBlock)
{
	return QuramDataDecompress(pBlock, nBlockSize, pOutBlock);
}

VOID	Tk15_DataDecompShutdown()
{
	QuramDataDecompShutdown();
}