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
#include "quram_qtc.h"

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
		BYTE* inbuff = NULL;
		INT inlen = 0;
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
		if (*(DWORD*)&inbuff[0] == 'CRMI')
		{
			BYTE* outbuff = NULL;
			DWORD outlen = 0;
			LONG ret;
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
				free(inbuff);
				return 1;
			}
			if (quram)
				ret = QuramDataDecomp_Buf(inbuff, (WORD*)outbuff);
			else
				ret = ImasterDataDecomp_Buf(inbuff, (WORD*)outbuff);
			if (ret == -1)
			{
				printf("Decompression failed\n");
				fclose(in);
				free(inbuff);
				free(outbuff);
				return 1;
			}
			if ((out = fopen(argv[2], "wb")) == NULL)
			{
				printf("Can't open %s\n", argv[2]);
				fclose(in);
				free(inbuff);
				free(outbuff);
				return 1;
			}
			fwrite(outbuff, outlen, 1, out);
			free(outbuff);
			fclose(out);
		}
		else if ((*(DWORD*)&inbuff[4]) == '2CTQ')
		{
			BYTE* outbuff_map = NULL;
			BYTE* outbuff_data = NULL;
			DWORD outlen_map = 0, outlen_data = 0;
			CHAR outname[1024] = { 0 };
			Quram_GetTableSize(inbuff, &outlen_data, &outlen_map);
			outbuff_map = malloc(outlen_map);
			outbuff_data = malloc(outlen_data);
			if (outbuff_map == NULL || outbuff_data == NULL)
			{
				printf("Can't allocate memory\n");
				fclose(in);
				free(inbuff);
				return 1;
			}
			if (Quram_DecodeTextComp(inbuff, outbuff_data, outbuff_map) == -1)
			{
				printf("Decompression failed\n");
				fclose(in);
				free(inbuff);
				free(outbuff_map);
				free(outbuff_data);
				return 1;
			}
			sprintf(outname, "%s.map", argv[2]);
			if ((out = fopen(outname, "wb")) == NULL)
			{
				printf("Can't open %s\n", outname);
				fclose(in);
				free(inbuff);
				free(outbuff_map);
				free(outbuff_data);
				return 1;
			}
			fwrite(outbuff_map, outlen_map, 1, out);
			fclose(out);
			sprintf(outname, "%s.data", argv[2]);
			if ((out = fopen(outname, "wb")) == NULL)
			{
				printf("Can't open %s\n", outname);
				fclose(in);
				free(inbuff);
				free(outbuff_map);
				free(outbuff_data);
				return 1;
			}
			fwrite(outbuff_data, outlen_data, 1, out);
			fclose(out);
			free(outbuff_map);
			free(outbuff_data);
		}
		else
		{
			printf("Unknown format\n");
			fclose(in);
			free(inbuff);
			return 1;
		}
		fclose(in);
		free(inbuff);
	}
}