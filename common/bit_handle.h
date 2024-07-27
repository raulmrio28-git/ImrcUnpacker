/*
** ===========================================================================
** File: bit_handle.h
** Description: Bit extraction handler
** Copyright (c) 2024 raulmrio28-git and contributors.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 05/15/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

//----------------------------------------------------------------------------
// P.S. For use, make sure your destination function(s) contain nCurrBits,
// pCmdBuf, and bCmdBytes.
//----------------------------------------------------------------------------

#ifndef BIT_HANDLE_H
#define BIT_HANDLE_H

#define CHK_BITS(bits)				while (nCurrBits < bits) \
									{ \
										bCmdBytes = (bCmdBytes<<8) | *pCmdBuf++; \
										nCurrBits += 8; \
									} 
#define EXTRACT_BITS(value, bits)	CHK_BITS(bits); \
									nCurrBits-= bits; \
									value = ((bCmdBytes >> nCurrBits) & ((1<<bits)-1));
#define EXTRACT_BIT(value)			EXTRACT_BITS(value, 1)

#endif //BIT_HANDLE_H