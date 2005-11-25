/*
**
** Copyright 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
** Released under the terms of the LGPL II.
**
**/

#include "lib.h"
#include <proto/locale.h>

/****************************************************************************/

void
freeBase(void)
{
    if (LocaleBase)
    {
        CloseLibrary((struct Library *)LocaleBase);
        LocaleBase = NULL;
    }

    if (lib_pool)
    {
        DeletePool(lib_pool);
        lib_pool = NULL;
    }

    if (UtilityBase)
    {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
    }

    if (DOSBase)
    {
        CloseLibrary((struct Library *)DOSBase);
        DOSBase = NULL;
    }

    lib_flags &= ~BASEFLG_Init;
}

/***********************************************************************/

struct loc
{
    STRPTR name;
    ULONG  len;
    STRPTR codesetName;
};

static struct loc locs[] =
{
    "bosanski",     8,  "ISO-8859-2",
    "català",       6,  "ISO-8859-1 + Euro",
    "czech",        5,  "ISO-8859-2",
    "dansk",        5,  "ISO-8859-1 + Euro",
    "deutsch",      7,  "ISO-8859-1 + Euro",
    "english",      7,  "ISO-8859-1 + Euro",
    "español",      7,  "ISO-8859-1 + Euro",
    "farsi",        5,  "ISO-8859-1",
    "français",     8,  "ISO-8859-1 + Euro",
    "greek",        5,  "ISO-8859-7",
    "hrvatski",     8,  "ISO-8859-2",
    "italiano",     8,  "ISO-8859-1 + Euro",
    "magyar",       6,  "ISO-8859-2",
    "nederlands",  10,  "ISO-8859-4",
    "nihongo",      7,  "ISO-8859-1",
    "norsk",        5,  "ISO-8859-1 + Euro",
    "polski",       6,  "ISO-8859-2",
    "português",    9,  "ISO-8859-1 + Euro",
    "russian",      7,  "Amiga-1251",
    "slovak",       6,  "ISO-8859-2",
    "slovensko",    9,  "ISO-8859-2",
    "srpski",       6,  "ISO-8859-2",
    "suomi",        5,  "ISO-8859-9",
    "svenska",      7,  "ISO-8859-1",
    "türkçe",       6,  "ISO-8859-9",
};

static void
getSystemCodeset(void)
{
    register struct Locale *locale;

    if (locale = OpenLocale(NULL))
    {
        register struct codeset *codeset;
        register struct loc     *loc = NULL;
        register STRPTR         ln;
        register ULONG          found = FALSE;
        register int            low, high;

        ln = locale->loc_LanguageName;
        //kprintf("Searching for %s\n",ln);

        for (low = 0, high = (sizeof(locs)/sizeof(struct loc))-1; low<=high; )
        {
            register int  mid = (low+high)>>1, cond;

            loc = locs+mid;

            if (!(cond = strnicmp(ln,loc->name,loc->len)))
            {
                found = TRUE;
                break;
            }

            if (cond<0) high = mid-1;
            else low = mid+1;
        }

        CloseLocale(locale);

        //if (found)
        //    kprintf("FOUND %s %s %lx\n",ln,loc->name,codesetsFind(&lib_codesets,loc->codesetName));

        if (found && (codeset = codesetsFind(&lib_codesets,loc->codesetName)))
            lib_systemCodeset = codeset;
    }
}

/***********************************************************************/

ULONG
initBase(void)
{
    NEWLIST(&lib_codesets);

    if ((DOSBase = (struct DosLibrary *)OpenLibrary("dos.library",37)) &&
        (UtilityBase = OpenLibrary("utility.library",37)) &&
        (lib_pool = CreatePool(MEMF_ANY,4096,512)))
    {
        if (codesetsInit(&lib_codesets))
        {
            lib_systemCodeset = (struct codeset *)lib_codesets.mlh_Head;

            if (LocaleBase = (struct LocaleBase *)OpenLibrary("locale.library",37))
                getSystemCodeset();

            lib_flags |= BASEFLG_Init;

            return TRUE;
        }
    }

    freeBase();

    return FALSE;
}

/***********************************************************************/
