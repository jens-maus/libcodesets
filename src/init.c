/***************************************************************************

 codesets.library - Amiga shared library for handling different codesets
 Copyright (C) 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
 Copyright (C) 2005      by codesets.library Open Source Team

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

#include <clib/alib_protos.h>
#include <proto/locale.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>

#if defined(DEBUG)
#include "debug.h"
#endif

#if defined(__amigaos4__)
struct Library    *DOSBase = NULL;
struct Library    *UtilityBase = NULL;
struct Library    *LocaleBase = NULL;
struct Library    *DiskfontBase = NULL;

struct DOSIFace*      IDOS = NULL;
struct UtilityIFace*  IUtility = NULL;
struct LocaleIFace*   ILocale = NULL;
struct DiskfontIFace* IDiskfont = NULL;
#elif defined(__MORPHOS__)
struct DosLibrary *DOSBase = NULL;
struct Library    *UtilityBase = NULL;
struct Library    *LocaleBase = NULL;
#else
struct DosLibrary *DOSBase = NULL;
struct Library    *UtilityBase = NULL;
struct LocaleBase *LocaleBase = NULL;
#endif

/****************************************************************************/

void
freeBase(struct LibraryHeader *lib)
{
  if(LocaleBase)
  {
    DROPINTERFACE(ILocale);
    CloseLibrary((struct Library *)LocaleBase);
    LocaleBase = NULL;
  }

  #if defined(__amigaos4__)
  if(DiskfontBase)
  {
    DROPINTERFACE(IDiskfont);
    CloseLibrary((struct Library *)DiskfontBase);
    LocaleBase = NULL;
  }
  #endif

  if(lib->pool)
  {
    DeletePool(lib->pool);
    lib->pool = NULL;
  }

  if(UtilityBase)
  {
    DROPINTERFACE(IUtility);
    CloseLibrary(UtilityBase);
    UtilityBase = NULL;
  }

  if(DOSBase)
  {
    DROPINTERFACE(IDOS);
    CloseLibrary((struct Library *)DOSBase);
    DOSBase = NULL;
  }

  lib->flags &= ~BASEFLG_Init;
}

/***********************************************************************/

struct loc
{
    STRPTR name;
    ULONG  len;
    STRPTR codesetName;
};

static const struct loc locs[] =
{
  { "bosanski",     8,  "ISO-8859-2"        },
  { "català",       6,  "ISO-8859-1 + Euro" },
  { "czech",        5,  "ISO-8859-2"        },
  { "dansk",        5,  "ISO-8859-1 + Euro" },
  { "deutsch",      7,  "ISO-8859-1 + Euro" },
  { "english",      7,  "ISO-8859-1 + Euro" },
  { "español",      7,  "ISO-8859-1 + Euro" },
  { "farsi",        5,  "ISO-8859-1"        },
  { "français",     8,  "ISO-8859-1 + Euro" },
  { "greek",        5,  "ISO-8859-7"        },
  { "hrvatski",     8,  "ISO-8859-2"        },
  { "italiano",     8,  "ISO-8859-1 + Euro" },
  { "magyar",       6,  "ISO-8859-2"        },
  { "nederlands",  10,  "ISO-8859-4"        },
  { "nihongo",      7,  "ISO-8859-1"        },
  { "norsk",        5,  "ISO-8859-1 + Euro" },
  { "polski",       6,  "ISO-8859-2"        },
  { "português",    9,  "ISO-8859-1 + Euro" },
  { "russian",      7,  "Amiga-1251"        },
  { "slovak",       6,  "ISO-8859-2"        },
  { "slovensko",    9,  "ISO-8859-2"        },
  { "srpski",       6,  "ISO-8859-2"        },
  { "suomi",        5,  "ISO-8859-9"        },
  { "svenska",      7,  "ISO-8859-1"        },
  { "türkçe",       6,  "ISO-8859-9"        },
};

static void
getSystemCodeset(struct LibraryHeader *lib)
{
  struct Locale *locale;

  if((locale = OpenLocale(NULL)))
  {
    struct codeset *codeset;
    struct loc     *loc = NULL;
    STRPTR         ln;
    ULONG          found = FALSE;
    int            low, high;

    ln = locale->loc_LanguageName;

    for(low = 0, high = (sizeof(locs)/sizeof(struct loc))-1; low<=high; )
    {
      int mid = (low+high)>>1, cond;

      loc = (struct loc *)&locs[mid];

      if(!(cond = strnicmp(ln, loc->name, loc->len)))
      {
        found = TRUE;
        break;
      }

      if(cond<0)
        high = mid-1;
      else
        low = mid+1;
    }

    CloseLocale(locale);

    if(found && (codeset = codesetsFind(&lib->codesets, loc->codesetName)))
      lib->systemCodeset = codeset;
  }
}

/***********************************************************************/

ULONG
initBase(struct LibraryHeader *lib)
{
  NewList((struct List *)&lib->codesets);

  if((DOSBase = (APTR)OpenLibrary("dos.library", 37)) &&
     GETINTERFACE(IDOS, DOSBase))
  {
    if((UtilityBase = OpenLibrary("utility.library", 37)) &&
       GETINTERFACE(IUtility, UtilityBase))
    {
      // setup the debugging stuff
      #if defined(DEBUG)
      SetupDebug();
      #endif

      #if defined(__amigaos4__)
      if((DiskfontBase = OpenLibrary("diskfont.library", 50)) &&
        GETINTERFACE(IDiskfont, DiskfontBase))
      {
      #endif
        if((lib->pool = CreatePool(MEMF_ANY, 4096, 512)))
        {
          if(codesetsInit(&lib->codesets))
          {
            lib->systemCodeset = (struct codeset *)lib->codesets.mlh_Head;

            if((LocaleBase = (APTR)OpenLibrary("locale.library", 37)) &&
               GETINTERFACE(ILocale, LocaleBase))
            {
              getSystemCodeset(lib);
            }

            lib->flags |= BASEFLG_Init;

            return TRUE;
          }
        }
      #if defined(__amigaos4__)
      }
      #endif
    }
  }

  freeBase(lib);

  return FALSE;
}

/***********************************************************************/
