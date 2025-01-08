/*
** ===========================================================================
** File: imrc2raw.c
** Description: IMRC to RAW decompressor.
** Copyright (c) 2025 raulmrio28-git and contributors.
** Format Copyright (C) 2006 I-master/Quram Co. Ltd.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 01/07/2025	raulmrio28-git	Initial version
** ===========================================================================
*/

/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "imaster_imrc.h"
#include "quram_imrc.h"

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

int main(int argc, char* argv[])
{
    if (argc != 3)
	{
		printf("Usage: imrc2raw <input> <output>\n");
		return 1;
	}
	{
		FILE* in, * out;
		BYTE* inbuff = NULL, * outbuff = NULL;
		INT inlen = 0, outlen = 0;
		BOOL quram = FALSE;
		if ((in = fopen(argv[1], "rb")) == NULL)
		{
			printf("Can't open %s\n", argv[1]);
			return 1;
		}
		fseek(in, 0, SEEK_END);
		inlen = ftell(in);
		fseek(in, 0, SEEK_SET);
		inbuff = malloc(inlen);
		if (inbuff == NULL)
		{
			printf("Can't allocate memory\n");
			fclose(in);
			return 1;
		}
		fread(inbuff, inlen, 1, in);
		if (*(DWORD*)&inbuff[0] != 'CRMI')
		{
			printf("Not a valid imrc file\n");
			fclose(in);
			return 1;
		}
		if (*(DWORD*)&inbuff[4] >= 0x2000000 || *(DWORD*)&inbuff[4] == 0x1000)
		{
			QuramDataDecomp_HdrInfo hdri;
			QuramDataDecomp_Buf_GetHeaderInfo(inbuff, &hdri);
			outlen = hdri.nDecompSize;
			quram = TRUE;
		}
		else
		{
			ImasterDataDecomp_HdrInfo hdri;
			ImasterDataDecomp_Buf_GetHeaderInfo(inbuff, &hdri);
			outlen = hdri.nDecompSize;
			quram = FALSE;
		}
		outbuff = malloc(outlen);
		if (outbuff == NULL)
		{
			printf("Can't allocate memory\n");
			fclose(in);
			return 1;
		}
		if (quram)
			QuramDataDecomp_Buf(inbuff, (WORD*)outbuff);
		else
			ImasterDataDecomp_Buf(inbuff, (WORD*)outbuff);
		if ((out = fopen(argv[2], "wb")) == NULL)
		{
			printf("Can't open %s\n", argv[2]);
			fclose(in);
			return 1;
		}
		fwrite(outbuff, outlen, 1, out);
		fclose(in);
		fclose(out);
		free(inbuff);
		free(outbuff);
	}
}