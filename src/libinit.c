/***************************************************************************

 codesets.library - Amiga shared library for handling different codesets
 Copyright (C) 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
 Copyright (C) 2005-2007 by codesets.library Open Source Team

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

#include "lib.h"
#include "version.h"

#include <exec/resident.h>
#include <proto/exec.h>

#include "debug.h"

/****************************************************************************/


#if defined(__amigaos4__)
struct Library *SysBase = NULL;
struct ExecIFace* IExec = NULL;
#if defined(__NEWLIB__)
struct Library *NewlibBase = NULL;
struct NewlibIFace* INewlib = NULL;
#endif
#else
struct ExecBase *SysBase = NULL;
#endif

struct LibraryHeader *CodesetsBase = NULL;

static const char UserLibName[] = "codesets.library";
static const char UserLibID[]   = "$VER: codesets.library " LIB_REV_STRING CPU " (" LIB_DATE ") " LIB_COPYRIGHT;

/****************************************************************************/

#define libvector LibNull                                 \
                  LFUNC_FA_(CodesetsConvertUTF32toUTF16)  \
                  LFUNC_FA_(CodesetsConvertUTF16toUTF32)  \
                  LFUNC_FA_(CodesetsConvertUTF16toUTF8)   \
                  LFUNC_FA_(CodesetsIsLegalUTF8)          \
                  LFUNC_FA_(CodesetsIsLegalUTF8Sequence)  \
                  LFUNC_FA_(CodesetsConvertUTF8toUTF16)   \
                  LFUNC_FA_(CodesetsConvertUTF32toUTF8)   \
                  LFUNC_FA_(CodesetsConvertUTF8toUTF32)   \
                  LFUNC_FA_(CodesetsSetDefaultA)          \
                  LFUNC_VA_(CodesetsSetDefault)           \
                  LFUNC_FA_(CodesetsFreeA)                \
                  LFUNC_VA_(CodesetsFree)                 \
                  LFUNC_FA_(CodesetsSupportedA)           \
                  LFUNC_VA_(CodesetsSupported)            \
                  LFUNC_FA_(CodesetsFindA)                \
                  LFUNC_VA_(CodesetsFind)                 \
                  LFUNC_FA_(CodesetsFindBestA)            \
                  LFUNC_VA_(CodesetsFindBest)             \
                  LFUNC_FA_(CodesetsUTF8Len)              \
                  LFUNC_FA_(CodesetsUTF8ToStrA)           \
                  LFUNC_VA_(CodesetsUTF8ToStr)            \
                  LFUNC_FA_(CodesetsUTF8CreateA)          \
                  LFUNC_VA_(CodesetsUTF8Create)           \
                  LFUNC_FA_(CodesetsEncodeB64A)           \
                  LFUNC_VA_(CodesetsEncodeB64)            \
                  LFUNC_FA_(CodesetsDecodeB64A)           \
                  LFUNC_VA_(CodesetsDecodeB64)            \
                  LFUNC_FA_(CodesetsStrLenA)              \
                  LFUNC_VA_(CodesetsStrLen)               \
                  LFUNC_FA_(CodesetsIsValidUTF8)          \
                  LFUNC_FA_(CodesetsFreeVecPooledA)       \
                  LFUNC_VA_(CodesetsFreeVecPooled)        \
                  LFUNC_FA_(CodesetsConvertStrA)          \
                  LFUNC_VA_(CodesetsConvertStr)           \
                  LFUNC_FA_(CodesetsListCreateA)          \
                  LFUNC_VA_(CodesetsListCreate)           \
                  LFUNC_FA_(CodesetsListDeleteA)          \
                  LFUNC_VA_(CodesetsListDelete)           \
                  LFUNC_FA_(CodesetsListAddA)             \
                  LFUNC_VA_(CodesetsListAdd)              \
                  LFUNC_FA_(CodesetsListRemoveA)          \
                  LFUNC_VA_(CodesetsListRemove)


/****************************************************************************/

#if defined(__amigaos4__)

static struct LibraryHeader * LIBFUNC LibInit    (struct LibraryHeader *base, BPTR librarySegment, struct ExecIFace *pIExec);
static BPTR                   LIBFUNC LibExpunge (struct LibraryManagerInterface *Self);
static struct LibraryHeader * LIBFUNC LibOpen    (struct LibraryManagerInterface *Self, ULONG version);
static BPTR                   LIBFUNC LibClose   (struct LibraryManagerInterface *Self);
static LONG                   LIBFUNC LibNull    (void);

#elif defined(__MORPHOS__)

static struct LibraryHeader * LIBFUNC LibInit   (struct LibraryHeader *base, BPTR librarySegment, struct ExecBase *sb);
static BPTR                   LIBFUNC LibExpunge(void);
static struct LibraryHeader * LIBFUNC LibOpen   (void);
static BPTR                   LIBFUNC LibClose  (void);
static LONG                   LIBFUNC LibNull   (void);

#else

static struct LibraryHeader * LIBFUNC LibInit    (REG(a0, BPTR Segment), REG(d0, struct LibraryHeader *lh), REG(a6, struct ExecBase *sb));
static BPTR                   LIBFUNC LibExpunge (REG(a6, struct LibraryHeader *base));
static struct LibraryHeader * LIBFUNC LibOpen    (REG(a6, struct LibraryHeader *base));
static BPTR                   LIBFUNC LibClose   (REG(a6, struct LibraryHeader *base));
static LONG                   LIBFUNC LibNull    (void);

#endif

/****************************************************************************/

#if defined(__amigaos4__)
int _start(void)
#else
int Main(void)
#endif
{
  return RETURN_FAIL;
}

static LONG LIBFUNC LibNull(VOID)
{
  return(0);
}

/****************************************************************************/

#if defined(__amigaos4__)
/* ------------------- OS4 Manager Interface ------------------------ */
STATIC ULONG LibObtain(struct LibraryManagerInterface *Self)
{
   return(Self->Data.RefCount++);
}

STATIC ULONG LibRelease(struct LibraryManagerInterface *Self)
{
   return(Self->Data.RefCount--);
}

STATIC CONST APTR LibManagerVectors[] =
{
   (APTR)LibObtain,
   (APTR)LibRelease,
   (APTR)NULL,
   (APTR)NULL,
   (APTR)LibOpen,
   (APTR)LibClose,
   (APTR)LibExpunge,
   (APTR)NULL,
   (APTR)-1
};

STATIC CONST struct TagItem LibManagerTags[] =
{
   {MIT_Name,             (ULONG)"__library"},
   {MIT_VectorTable,      (ULONG)LibManagerVectors},
   {MIT_Version,          1},
   {TAG_DONE,             0}
};

/* ------------------- Library Interface(s) ------------------------ */

STATIC CONST APTR LibVectors[] =
{
   (APTR)LibObtain,
   (APTR)LibRelease,
   (APTR)NULL,
   (APTR)NULL,
   (APTR)libvector,
   (APTR)-1
};

STATIC CONST struct TagItem MainTags[] =
{
   {MIT_Name,        (ULONG)"main"},
   {MIT_VectorTable, (ULONG)LibVectors},
   {MIT_Version,     1},
   {TAG_DONE,        0}
};

STATIC CONST ULONG LibInterfaces[] =
{
   (ULONG)LibManagerTags,
   (ULONG)MainTags,
   (ULONG)0
};

// Out libraries always have to carry a 68k jump table with it, so
// lets define it here as extern, as we are going to link it to
// our binary here.
#ifndef NO_VECTABLE68K
extern const APTR VecTable68K[];
#endif

STATIC CONST struct TagItem LibCreateTags[] =
{
   {CLT_DataSize,   (ULONG)(sizeof(struct LibraryHeader))},
   {CLT_InitFunc,   (ULONG)LibInit},
   {CLT_Interfaces, (ULONG)LibInterfaces},
   #ifndef NO_VECTABLE68K
   {CLT_Vector68K,  (ULONG)VecTable68K},
   #endif
   {TAG_DONE,       0}
};

#else

STATIC CONST APTR LibVectors[] =
{
  #ifdef __MORPHOS__
  (APTR)FUNCARRAY_32BIT_NATIVE,
  #endif
  (APTR)LibOpen,
  (APTR)LibClose,
  (APTR)LibExpunge,
  (APTR)LibNull,
  (APTR)libvector,
  (APTR)-1
};

STATIC CONST ULONG LibInitTab[] =
{
  sizeof(struct LibraryHeader),
  (ULONG)LibVectors,
  (ULONG)NULL,
  (ULONG)LibInit
};

#endif

/****************************************************************************/

static const USED_VAR struct Resident ROMTag =
{
  RTC_MATCHWORD,
  (struct Resident *)&ROMTag,
  (struct Resident *)&ROMTag + 1,
  #if defined(__amigaos4__)
  RTF_AUTOINIT|RTF_NATIVE,      // The Library should be set up according to the given table.
  #elif defined(__MORPHOS__)
  RTF_AUTOINIT|RTF_PPC,
  #else
  RTF_AUTOINIT,
  #endif
  LIB_VERSION,
  NT_LIBRARY,
  0,
  (char *)UserLibName,
  (char *)UserLibID+6,
  #if defined(__amigaos4__)
  (APTR)LibCreateTags           // This table is for initializing the Library.
  #else
  (APTR)LibInitTab,
  #endif
  #if defined(__MORPHOS__)
  LIB_REVISION,
  0
  #endif
};

#if defined(__MORPHOS__)
/*
 * To tell the loader that this is a new emulppc elf and not
 * one for the ppc.library.
 * ** IMPORTANT **
 */
const USED_VAR ULONG __amigappc__ = 1;
const USED_VAR ULONG __abox__ = 1;

#endif /* __MORPHOS */

/****************************************************************************/

#if defined(__amigaos4__)
static struct LibraryHeader * LibInit(struct LibraryHeader *base, BPTR librarySegment, struct ExecIFace *pIExec)
{
  struct ExecBase *sb = (struct ExecBase *)pIExec->Data.LibBase;
  IExec = pIExec;
#elif defined(__MORPHOS__)
static struct LibraryHeader * LibInit(struct LibraryHeader *base, BPTR librarySegment, struct ExecBase *sb)
{
#else
static struct LibraryHeader * LIBFUNC LibInit(REG(a0, BPTR librarySegment), REG(d0, struct LibraryHeader *base), REG(a6, struct ExecBase *sb))
{
#endif

  SysBase = (APTR)sb;

  #if defined(__amigaos4__) && defined(__NEWLIB__)
  if((NewlibBase = OpenLibrary("newlib.library", 3)) &&
     GETINTERFACE(INewlib, NewlibBase))
  #endif
  {
    D(DBF_STARTUP, "LibInit()");

    // cleanup the library header structure beginning with the
    // library base.
    base->libBase.lib_Node.ln_Type = NT_LIBRARY;
    base->libBase.lib_Node.ln_Pri  = 0;
    base->libBase.lib_Node.ln_Name = (char *)UserLibName;
    base->libBase.lib_Flags        = LIBF_CHANGED | LIBF_SUMUSED;
    base->libBase.lib_Version      = LIB_VERSION;
    base->libBase.lib_Revision     = LIB_REVISION;
    base->libBase.lib_IdString     = (char *)(UserLibID+6);

    base->sysBase = (APTR)SysBase;
    base->segList = librarySegment;
    InitSemaphore(&base->libSem);
    base->pool = NULL;
    InitSemaphore(&base->poolSem);
    base->flags = 0;
    base->systemCodeset = NULL;
    base->wasInitialized = FALSE;

    // set the CodesetsBase
    CodesetsBase = base;
  }

  return CodesetsBase;
}

/****************************************************************************/

#if defined(__amigaos4__)
static struct LibraryHeader *LibOpen(struct LibraryManagerInterface *Self, ULONG version UNUSED)
{
  struct LibraryHeader *base = (struct LibraryHeader *)Self->Data.LibBase;
#elif defined(__MORPHOS__)
static struct LibraryHeader *LibOpen(void)
{
  struct LibraryHeader *base = (struct LibraryHeader*)REG_A6;
#else
static struct LibraryHeader * LIBFUNC LibOpen(REG(a6, struct LibraryHeader *base))
{
#endif
  struct LibraryHeader *res;

  D(DBF_STARTUP, "LibOpen()");

  ObtainSemaphore(&base->libSem);

  // increase the open counter first, as initBase() might take a while and
  // a LibExpunge() may be invoked in the mean time which will in turn expunge
  // a not yet fully initialized library.
  base->libBase.lib_OpenCnt++; 

  // in case our codesets list wasn't already initialized
  // we do it right now.
  if(base->wasInitialized == FALSE && initBase(base) == FALSE)
  {
    // initialization didn't work out as expected
    res = NULL;
    // decrease the open counter again
    base->libBase.lib_OpenCnt--;
  }
  else
  {
    // if we reach here then we have successfully initialized
    // our library and can flag it as such.
    base->wasInitialized = TRUE;
    // delete the late expunge flag
    base->libBase.lib_Flags &= ~LIBF_DELEXP; 

    // return the base address on success.
    res = base;
  }

  ReleaseSemaphore(&base->libSem);

  return res;
}

/****************************************************************************/

#ifndef __amigaos4__
#define DeleteLibrary(LIB) \
  FreeMem((STRPTR)(LIB)-(LIB)->lib_NegSize, (ULONG)((LIB)->lib_NegSize+(LIB)->lib_PosSize))
#endif

#if defined(__amigaos4__)
static BPTR LibExpunge(struct LibraryManagerInterface *Self)
{
  struct LibraryHeader *base = (struct LibraryHeader *)Self->Data.LibBase;
#elif defined(__MORPHOS__)
static BPTR LibExpunge(void)
{
	struct LibraryHeader *base = (struct LibraryHeader*)REG_A6;
#else
static BPTR LIBFUNC LibExpunge(REG(a6, struct LibraryHeader *base))
{
#endif
  BPTR rc;

  D(DBF_STARTUP, "LibExpunge()");

  // in case our open counter is still > 0, we have
  // to set the late expunge flag and return immediately
  if(base->libBase.lib_OpenCnt > 0)
  {
    base->libBase.lib_Flags |= LIBF_DELEXP;
    rc = 0;
  }
  else
  {
    // in case the open counter is zero we can go
    // and remove/expunge the library.
    SysBase = (APTR)base->sysBase;

    // we expunge all our private data now. We haven't done
    // that in LibClose() already because we want to keep
    // our stuff kinda cached so that we don't have to
    // initialize our codesets tables all the time when a user
    // opens/closes the only app using codesets.library.
    if(base->wasInitialized)
    {
      // free all our private data and stuff.
      freeBase(base);
      base->wasInitialized = FALSE;
    }

    #if defined(__amigaos4__) && defined(__NEWLIB__)
    if(NewlibBase)
    {
      DROPINTERFACE(INewlib);
      CloseLibrary(NewlibBase);
      NewlibBase = NULL;
    }
    #endif

    Remove((struct Node *)base);
    rc = base->segList;
    DeleteLibrary(&base->libBase);
  }

  return rc;
}

/****************************************************************************/

#if defined(__amigaos4__)
static BPTR LibClose(struct LibraryManagerInterface *Self)
{
  struct LibraryHeader *base = (struct LibraryHeader *)Self->Data.LibBase;
#elif defined(__MORPHOS__)
static BPTR LibClose(void)
{
	struct LibraryHeader *base = (struct LibraryHeader *)REG_A6;
#else
static BPTR LIBFUNC LibClose(REG(a6, struct LibraryHeader *base))
{
#endif
  BPTR rc = 0;

  D(DBF_STARTUP, "LibClose()");

  ObtainSemaphore(&base->libSem);

  // decrease the open counter
  base->libBase.lib_OpenCnt--;

  // in case the opern counter is <= 0 we can
  // make sure that we free everything
  if(base->libBase.lib_OpenCnt <= 0)
  {
    // in case the late expunge flag is set we go and
    // expunge the library base right now
    if(base->libBase.lib_Flags & LIBF_DELEXP)
    {
      ReleaseSemaphore(&base->libSem);

      #if defined(__amigaos4__)
      rc = LibExpunge(Self);
      #elif defined(__MORPHOS__)
      rc = LibExpunge();
      #else
      rc = LibExpunge(base);
      #endif

      return rc;
    }
  }

  ReleaseSemaphore(&base->libSem);

  return rc;
}

/****************************************************************************/
