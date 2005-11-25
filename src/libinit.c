/*
**
** Copyright 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
** Released under the terms of the LGPL II.
**
**/

#include "lib.h"
#include "codesets.library_rev.h"

/****************************************************************************/

UBYTE                   lib_name[] = PRG;
UBYTE		               lib_ver[] = VSTRING;
UBYTE                   lib_fullName[] = PRGNAME;
ULONG                   lib_version = VERSION;
ULONG                   lib_revision = REVISION;

struct ExecBase         *SysBase = NULL;
struct DosLibrary       *DOSBase = NULL;
struct Library          *UtilityBase = NULL;
struct LocaleBase       *LocaleBase = NULL;

struct SignalSemaphore  lib_sem = {0};
struct Library          *lib_base = NULL;
ULONG                   lib_segList = NULL;
struct SignalSemaphore  lib_poolSem = {0};
APTR                    lib_pool = NULL;
ULONG                   lib_flags = 0;
struct MinList          lib_codesets = {0};
struct codeset          *lib_systemCodeset = NULL;

/****************************************************************************/

#ifdef __MORPHOS__
#include <exec/resident.h>

static ULONG first(void) __attribute((unused));
static struct Library *initLib ( struct Library *base , BPTR segList , struct ExecBase *sys );
static struct Library *openLib ( void );
static ULONG expungeLib ( void );
static ULONG closeLib ( void );
static ULONG nil (void );

/****************************************************************************/

static ULONG
first(void)
{
    return -1;
}

/****************************************************************************/

static const APTR funcTable[] =
{
	(APTR)   FUNCARRAY_32BIT_NATIVE,
	(APTR)   openLib,
	(APTR)   closeLib,
	(APTR)   expungeLib,
	(APTR)   nil,

	(APTR)   nil,

        (APTR)LIB_CodesetsConvertUTF32toUTF16,
        (APTR)LIB_CodesetsConvertUTF16toUTF32,
        (APTR)LIB_CodesetsConvertUTF16toUTF8,
        (APTR)LIB_CodesetsIsLegalUTF8,
        (APTR)LIB_CodesetsIsLegalUTF8Sequence,
        (APTR)LIB_CodesetsConvertUTF8toUTF16,
        (APTR)LIB_CodesetsConvertUTF32toUTF8,
        (APTR)LIB_CodesetsConvertUTF8toUTF32,
        (APTR)LIB_CodesetsSetDefaultA,
        (APTR)LIB_CodesetsFreeA,
        (APTR)LIB_CodesetsSupportedA,
        (APTR)LIB_CodesetsFindA,
        (APTR)LIB_CodesetsFindBestA,
        (APTR)LIB_CodesetsUTF8Len,
        (APTR)LIB_CodesetsUTF8ToStrA,
        (APTR)LIB_CodesetsUTF8CreateA,

        (APTR)LIB_CodesetsEncodeB64A,
        (APTR)LIB_CodesetsDecodeB64A,

        (APTR)LIB_CodesetsStrLenA,

        (APTR)LIB_CodesetsIsValidUTF8,

        (APTR)LIB_CodesetsFreeVecPooledA,

        (APTR)   -1
};

static const ULONG initTable[] =
{
	sizeof(struct Library),
	(ULONG)funcTable,
	NULL,
	(ULONG)initLib
};

const struct Resident romTag =
{
	RTC_MATCHWORD,
	(struct Resident *)&romTag,
	(struct Resident *)&romTag+1,
	RTF_AUTOINIT|RTF_PPC|RTF_EXTENDED,
	VERSION,
	NT_LIBRARY,
	0,
	(STRPTR)lib_name,
	(STRPTR)lib_ver,
	(APTR)initTable,
	REVISION,
        NULL
};

const ULONG __abox__	= 1;
#endif

/****************************************************************************/

#ifdef __MORPHOS__
static struct Library *initLib(struct Library *base,BPTR segList,struct ExecBase *sys)
#else
struct Library *SAVEDS ASM initLib(REG(a0) ULONG segList,REG(a6) APTR sys,REG(d0)struct Library *base)
#endif
{

#define SysBase sys
    InitSemaphore(&lib_sem);
    InitSemaphore(&lib_poolSem);
#undef SysBase

    SysBase     = sys;
    lib_segList = segList;

    return lib_base = base;
}

/****************************************************************************/

#ifdef __MORPHOS__
static struct Library *openLib(void)
#else
struct Library * SAVEDS ASM openLib(REG(a6) struct Library *base)
#endif
{
#ifdef __MORPHOS__
	struct Library *base = (struct Library *)REG_A6;
#endif
    register struct Library *res;

    ObtainSemaphore(&lib_sem);

    base->lib_OpenCnt++;
    base->lib_Flags &= ~LIBF_DELEXP;

    if (!(lib_flags & BASEFLG_Init) && !initBase())
    {
        base->lib_OpenCnt--;
        res = NULL;
    }
    else res = base;

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG expungeLib(void)
#else
ULONG SAVEDS ASM expungeLib(REG(a6) struct Library *base)
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register ULONG res;

    ObtainSemaphore(&lib_sem);

    if (!base->lib_OpenCnt)
    {
        Remove((struct Node *)base);
        FreeMem((UBYTE *)base-base->lib_NegSize,base->lib_NegSize+base->lib_PosSize);

        res = lib_segList;
    }
    else
    {
        base->lib_Flags |= LIBF_DELEXP;
        res = NULL;
    }

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG closeLib(void)
#else
ULONG SAVEDS ASM closeLib(REG(a6) struct Library *base)
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register ULONG res = NULL;

    ObtainSemaphore(&lib_sem);

    if (!--base->lib_OpenCnt)
    {
        freeBase();

        if (base->lib_Flags & LIBF_DELEXP)
	{
            Remove((struct Node *)base);
    	    FreeMem((UBYTE *)base-base->lib_NegSize,base->lib_NegSize+base->lib_PosSize);

            res = lib_segList;
	}
    }

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG nil(void)
{
    return 0;
}
#endif

/****************************************************************************/

