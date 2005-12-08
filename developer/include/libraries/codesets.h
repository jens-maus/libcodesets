#ifndef LIBRARIES_CODESETS_H
#define LIBRARIES_CODESETS_H

/*
**  $VER: codesets.h 5.3 (5.5.2005)
**  Includes Release 5.3
**
**  Written by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
**  Released under the terms of the
**  LGPL version 2.
*/

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
#define CODESETSVER     4

/***********************************************************************/
/*
 * Types
 */

typedef unsigned long   UTF32;   /* at least 32 bits */
typedef unsigned short  UTF16;   /* at least 16 bits */
typedef unsigned char   UTF8;    /* typically 8 bits */

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
};

/***********************************************************************/

enum
{
    CONVRES_ConversionOK,       /* conversion successful */
    CONVRES_SourceExhausted,    /* partial character in source, but hit end */
    CONVRES_TargetExhausted,    /* insuff. room in target for conversion */
    CONVRES_SourceIllegal       /* source sequence is illegal/malformed */
};

enum
{
    CONVFLG_StrictConversion = 0,
    CONVFLG_LenientConversion
};

/***********************************************************************/
/*
** Tags
*/

/* This value breaks the GPL2: it is strictly PRIVATE and CAN'T BE RE-USED! */
#define CODESETSLIB_TAG(n)          ((int)0xfec901f4+(n))

#define CODESETSA_Base                CODESETSLIB_TAG(0)

#define CODESETSA_SourceLen           CODESETSLIB_TAG(1)
#define CODESETSA_Source              CODESETSLIB_TAG(2)
#define CODESETSA_Dest                CODESETSLIB_TAG(3)
#define CODESETSA_DestLen             CODESETSLIB_TAG(4)
#define CODESETSA_DestHook            CODESETSLIB_TAG(5)
#define CODESETSA_DestLenPtr          CODESETSLIB_TAG(6)
#define CODESETSA_Codeset             CODESETSLIB_TAG(7)
#define CODESETSA_Pool                CODESETSLIB_TAG(8)
#define CODESETSA_PoolSem             CODESETSLIB_TAG(9)
#define CODESETSA_AllocIfNeeded       CODESETSLIB_TAG(10)
#define CODESETSA_Save                CODESETSLIB_TAG(11)
#define CODESETSA_NoFail              CODESETSLIB_TAG(12)

#define CODESETSA_B64SourceString     CODESETSLIB_TAG(20)
#define CODESETSA_B64SourceLen        CODESETSLIB_TAG(21)
#define CODESETSA_B64SourceFile       CODESETSLIB_TAG(22)
#define CODESETSA_B64DestPtr          CODESETSLIB_TAG(23)
#define CODESETSA_B64DestFile         CODESETSLIB_TAG(24)
#define CODESETSA_B64MaxLineLen       CODESETSLIB_TAG(25)
#define CODESETSA_B64Unix             CODESETSLIB_TAG(26)
#define CODESETSA_B64FLG_NtCheckErr   CODESETSLIB_TAG(27)

/***********************************************************************/
/*
** Returns code from CodesetsEncodeB64A() CodesetsDecodeB64A()
*/

enum
{
    B64_ERROR_OK = 0,
    B64_ERROR_MEM,
    B64_ERROR_DOS,
    B64_ERROR_INCOMPLETE,
    B64_ERROR_ILLEGAL,
};

/***********************************************************************/

struct convertMsg
{
    ULONG state;
    ULONG len;
};

enum
{
    CODESETV_Translating,
    CODESETV_End,
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
