/*
** ===========================================================================
** File: typedef.h
** Description: Typedefs for ImrcUnpacker
** Copyright (c) 2024 raulmrio28-git and contributors.
** History:
** when			who				what, where, why
** MM-DD-YYYY-- --------------- --------------------------------
** 05/16/2024	raulmrio28-git	Initial version
** ===========================================================================
*/

#ifndef TYPEDEF_H
#define TYPEDEF_H
/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/* not to generate macro redefintion warning */
#undef CONST
#undef STATIC
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

#ifndef NULL
#define NULL    0
#endif
#define CONST           const
#ifndef LOCAL
#define LOCAL           static      /* scope */
#endif
#define STATIC          static      /* memory category */
#define TRUE            (1)
#define FALSE           (!TRUE)
#define PACKED			__packed 

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef unsigned char   BOOL;
typedef unsigned char   ONOFF;
typedef char            CHAR;
typedef unsigned char   BYTE;
typedef short           SHORT;
typedef int             INT;
typedef unsigned int    UINT;
typedef long            LONG;
typedef float           FLOAT;
typedef double          DOUBLE;
typedef unsigned long	SIZE_T;
typedef unsigned short  WCHAR;
typedef unsigned char   UCHAR;
typedef signed char     INT8;
typedef unsigned char   UINT8;
typedef signed short    INT16;
typedef unsigned short  UINT16;
typedef signed long     INT32;
typedef unsigned long   UINT32;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef float           FLOAT32; 
typedef void			VOID;

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

#endif //TYPEDEF_H