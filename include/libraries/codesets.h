#ifndef LIBRARIES_CODESETS_H
#define LIBRARIES_CODESETS_H

/***************************************************************************

 codesets.library - Amiga shared library for handling different codesets
 Copyright (C) 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
 Copyright (C) 2005-2018 codesets.library Open Source Team

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 codesets.library project: http://sourceforge.net/projects/codesetslib/

 $Id$

***************************************************************************/

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif

#ifdef __GNUC__
  #ifdef __PPC__
    #pragma pack(2)
  #endif
#elif defined(__VBCC__)
  #pragma amiga-align
#endif

/***********************************************************************/
/*
** Actual library name and version.
*/

#define CODESETSNAME    "codesets.library"
#define CODESETSVER     6

/***********************************************************************/
/*
 * Types
 */

#ifndef UTF32_TYPEDEF
#define UTF32_TYPEDEF
typedef unsigned long  UTF32;   /* at least 32 bits */
#endif

#ifndef UTF16_TYPEDEF
#define UTF16_TYPEDEF
typedef unsigned short UTF16;   /* at least 16 bits */
#endif

#ifndef UTF8_TYPEDEF
#define UTF8_TYPEDEF
typedef unsigned char  UTF8;    /* typically 8 bits */
#endif

/***********************************************************************/
/*
 * single_convert
 */

struct single_convert
{
  unsigned char code; /* the code in this representation */
  UTF8 utf8[8];       /* the utf8 string, first byte is alway the length of the string */
  unsigned int  ucs4; /* the full 32 bit unicode */
};

/***********************************************************************/
/*
 * codeset
 */

struct codeset
{
  struct MinNode          node;
  char                    *name;
  char                    *alt_name;
  char                    *characterization;
  int                     read_only;
  struct single_convert   table[256];
  struct single_convert   table_sorted[256];
  unsigned int            MIBenum;
};

/***********************************************************************/
/*
 * codesetList
 */

struct codesetList
{
  struct MinList list;
};

/***********************************************************************/

enum
{
  CSR_ConversionOK=0,  /* conversion successful */
  CSR_SourceExhausted, /* partial character in source, but hit end */
  CSR_TargetExhausted, /* insuff. room in target for conversion */
  CSR_SourceIllegal    /* source sequence is illegal/malformed */
};

enum
{
  CSF_StrictConversion=0,
  CSF_LenientConversion
};

/***********************************************************************/

/*
** Enumerations for CSA_CodesetFamily
*/
enum
{
  CSV_CodesetFamily_Latin=0,   /* Latin Family */
  CSV_CodesetFamily_Cyrillic   /* Cyrillic Family */
};

/***********************************************************************/
/*
** Tags
*/

#define CODESETSLIB_TAG(n)        ((ULONG)0xfec901f4+(n))

#define CSA_Base                   CODESETSLIB_TAG(0)

#define CSA_SourceLen              CODESETSLIB_TAG(1)
#define CSA_Source                 CODESETSLIB_TAG(2)
#define CSA_Dest                   CODESETSLIB_TAG(3)
#define CSA_DestLen                CODESETSLIB_TAG(4)
#define CSA_DestHook               CODESETSLIB_TAG(5)
#define CSA_DestLenPtr             CODESETSLIB_TAG(6)
#define CSA_SourceCodeset          CODESETSLIB_TAG(7)
#define CSA_Pool                   CODESETSLIB_TAG(8)
#define CSA_PoolSem                CODESETSLIB_TAG(9)
#define CSA_AllocIfNeeded          CODESETSLIB_TAG(10)
#define CSA_Save                   CODESETSLIB_TAG(11)
#define CSA_FallbackToDefault      CODESETSLIB_TAG(12)
#define CSA_DestCodeset            CODESETSLIB_TAG(13)
#define CSA_CodesetDir             CODESETSLIB_TAG(14)
#define CSA_CodesetFile            CODESETSLIB_TAG(15)
#define CSA_CodesetList            CODESETSLIB_TAG(16)
#define CSA_FreeCodesets           CODESETSLIB_TAG(17)
#define CSA_CodesetFamily          CODESETSLIB_TAG(18)
#define CSA_ErrPtr                 CODESETSLIB_TAG(19)

#define CSA_B64SourceString        CODESETSLIB_TAG(20)
#define CSA_B64SourceLen           CODESETSLIB_TAG(21)
#define CSA_B64SourceFile          CODESETSLIB_TAG(22)
#define CSA_B64DestPtr             CODESETSLIB_TAG(23)
#define CSA_B64DestFile            CODESETSLIB_TAG(24)
#define CSA_B64MaxLineLen          CODESETSLIB_TAG(25)
#define CSA_B64Unix                CODESETSLIB_TAG(26)
#define CSA_B64FLG_NtCheckErr      CODESETSLIB_TAG(27)

#define CSA_MapForeignChars        CODESETSLIB_TAG(28)
#define CSA_MapForeignCharsHook    CODESETSLIB_TAG(29)

#define CSA_AllowMultibyteCodesets CODESETSLIB_TAG(30)

#define CSA_MIBenum                CODESETSLIB_TAG(31) // equal to CSA_SourceMIBenum
#define CSA_SourceMIBenum          CODESETSLIB_TAG(31) // equal to CSA_MIBenum
#define CSA_DestMIBenum            CODESETSLIB_TAG(32)

/***********************************************************************/
/*
** Returns code from CodesetsEncodeB64A() CodesetsDecodeB64A()
*/

enum
{
  CSR_B64_ERROR_OK = 0,
  CSR_B64_ERROR_MEM,
  CSR_B64_ERROR_DOS,
  CSR_B64_ERROR_INCOMPLETE,
  CSR_B64_ERROR_ILLEGAL,
};

/***********************************************************************/

enum
{
  // the system default encoding
  CS_MIBENUM_SYSTEM          = 0xffffffffUL,

  // standard IANA MIBenum definitions
  CS_MIBENUM_INVALID         = 0,
  CS_MIBENUM_US_ASCII        = 3,
  CS_MIBENUM_ISO_8859_1      = 4,
  CS_MIBENUM_ISO_8859_2      = 5,
  CS_MIBENUM_ISO_8859_3      = 6,
  CS_MIBENUM_ISO_8859_4      = 7,
  CS_MIBENUM_ISO_8859_5      = 8,
  CS_MIBENUM_ISO_8859_6      = 9,
  CS_MIBENUM_ISO_8859_7      = 10,
  CS_MIBENUM_ISO_8859_8      = 11,
  CS_MIBENUM_ISO_8859_9      = 12,
  CS_MIBENUM_ISO_8859_10     = 13,
  CS_MIBENUM_UTF_8           = 106,
  CS_MIBENUM_ISO_8859_13     = 109,
  CS_MIBENUM_ISO_8859_14     = 110,
  CS_MIBENUM_ISO_8859_15     = 111,
  CS_MIBENUM_ISO_8859_16     = 112,
  CS_MIBENUM_ISO_10646_UCS_2 = 1000,
  CS_MIBENUM_ISO_10646_UCS_4 = 1001,
  CS_MIBENUM_UTF_16BE        = 1013,
  CS_MIBENUM_UTF_16LE        = 1014,
  CS_MIBENUM_UTF_16          = 1015,
  CS_MIBENUM_UTF_32          = 1017,
  CS_MIBENUM_UTF_32BE        = 1018,
  CS_MIBENUM_UTF_32LE        = 1019,
  CS_MIBENUM_KOI8_R          = 2084,
  CS_MIBENUM_AMIGA_1251      = 2104,
  CS_MIBENUM_WINDOWS_1250    = 2250,
  CS_MIBENUM_WINDOWS_1251    = 2251,
  CS_MIBENUM_WINDOWS_1252    = 2252,
  CS_MIBENUM_TIS_620         = 2259,

  // non-standard MIBenum numbers
  CS_MIBENUM_AMIGAPL         = 5000,
};

/***********************************************************************/

struct convertMsg
{
  ULONG state;
  ULONG len;
};

enum
{
  CSV_Translating,
  CSV_End,
};

/***********************************************************************/

/*
** the message passed to a CSA_MapForeignCharsHook hook
** the hook function must return the length of the replacement string
** == 0 signals 'no replacement found'
**  > 0 will use the supplied string directly
**  < 0 signals an UTF8 replacement of negated length
*/
struct replaceMsg
{
  char **dst;          /* place the replace string here */
  unsigned char *src;  /* the source UTF8 string */
  int srclen;          /* length of the UTF8 sequence */
};

/***********************************************************************/

#ifdef __GNUC__
  #ifdef __PPC__
    #pragma pack()
  #endif
#elif defined(__VBCC__)
  #pragma default-align
#endif

#endif /* LIBRARIES_CODESETS_H */
