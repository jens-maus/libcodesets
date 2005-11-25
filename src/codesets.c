
/***************************************************************************
** SimpleMail - Copyright (C) 2000 Hynek Schlawack and Sebastian Bauer
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
***************************************************************************/

/**************************************************************************
**
** Modified by Alfonso [alfie] Ranieri <alforan@tin.it>
**
***************************************************************************/

#include "lib.h"
#include <ctype.h>
#include <limits.h>
#include "codesets_table.h"
#include "ConvertUTF.h"

/***********************************************************************/

/* search a sorted array in O(log n) e.g.
   BIN_SEARCH(strings,0,sizeof(strings)/sizeof(strings[0]),strcmp(key,array[mid]),res); */
#define BIN_SEARCH(array,low,high,compare,result) \
    {\
        int l = low;\
        int h = high;\
        int m = (low+high)/2;\
        result = NULL;\
        while (l<=h)\
        {\
            int c = compare;\
            if (!c){ result = &array[m]; break; }\
            if (c < 0) h = m - 1;\
            else l = m + 1;\
            m = (l + h)/2;\
        }\
    }

/***********************************************************************/

static int
mystrncmp(STRPTR str1,STRPTR str2,int len)
{
    while (len)
    {
        register int d;

        if ((d = *str1++ - *str2++)) return d;
        len--;
    }

    return 0;
}

/***********************************************************************/

static int
mystricmp(STRPTR str1,STRPTR str2)
{
    if (!str1)
    {
        if (str2) return -1;

        return 0;
    }

    if (!str2) return 1;

    return stricmp(str1,str2);
}

/***********************************************************************/

static int
mystrnicmp(STRPTR str1,STRPTR str2,int n)
{
    if (!n) return 0;

    if (!str1)
    {
        if (str2) return -1;

        return 0;
    }

    if (!str2) return 1;

    return strnicmp(str1,str2,n);
}

/***********************************************************************/

static STRPTR
mystrdup(STRPTR str)
{
    register STRPTR new;
    register int   len;

    if (!str) return NULL;

    len = strlen(str);
    if (!len) return NULL;

    if ((new = allocArbitrateVecPooled(len+1)))
        strcpy(new,str);

    return new;
}

/***********************************************************************/

static STRPTR
mystrndup(STRPTR str1,int n)
{
    register STRPTR dest;

    if ((dest = allocArbitrateVecPooled(n+1)))
    {
        if (str1) strncpy(dest,str1,n);
        else dest[0] = 0;

        dest[n] = 0;
    }

    return dest;
}

/***********************************************************************/

static ULONG
readLine(BPTR fh,STRPTR buf,int size)
{
    register STRPTR c;

    if (!FGets(fh,buf,size)) return FALSE;

    for (c = buf; *c; c++)
    {
        if (*c=='\n' || *c=='\r')
        {
            *c = 0;
            break;
        }
    }

    return TRUE;
}

/***********************************************************************/

static STRPTR
getConfigItem(STRPTR buf,STRPTR item,int len)
{
    if (!mystrnicmp(buf,item,len))
    {
        register UBYTE c;

        buf += len;

        /* skip spaces */
        while ((c = *buf) && isspace(c)) buf++;

        if (*buf!='=') return NULL;
        buf++;

        /* skip spaces */
        while ((c = *buf) && isspace(c)) buf++;

        return buf;
    }

    return NULL;
}

/***********************************************************************/

STRPTR *LIBCALL
CodesetsSupportedA(REG(a0) struct TagItem * attrs)
{
    register STRPTR *array;

    if (array = allocArbitrateVecPooled(sizeof(STRPTR)*(countNodes(&lib_codesets)+1)))
    {
        register struct codeset *code, *succ;
        register int            i;

        ObtainSemaphoreShared(&lib_sem);

        for (i = 0, code = (struct codeset *)lib_codesets.mlh_Head; succ = (struct codeset *)code->node.mln_Succ; code = succ, i++)
            array[i] = code->name;

        array[i] = NULL;

        ReleaseSemaphore(&lib_sem);
    }

    return array;
}

#ifdef __MORPHOS__
STRPTR *
LIB_CodesetsSupportedA(void)
{
    return CodesetsSupportedA((struct TagItem *)REG_A0);
}
#endif

/**************************************************************************/

void LIBCALL
CodesetsFreeA(REG(a0) APTR obj,REG(a1) struct TagItem *attrs)
{
    if (obj) freeArbitrateVecPooled(obj);
}

#ifdef __MORPHOS__
void
LIB_CodesetsFreeA(void)
{
    return CodesetsFreeA((APTR)REG_A0,(struct TagItem *)REG_A1);
}
#endif

/**************************************************************************/

/*
 * The compare function
 */

static int
codesetsCmpUnicode(struct single_convert *arg1,struct single_convert *arg2)
{
    return strcmp(arg1->utf8+1,arg2->utf8+1);
}

/**************************************************************************/
/*
 * Reads a coding table and adds it
 */


#define ITEM_STANDARD           "Standard"
#define ITEM_ALTSTANDARD        "AltStandard"
#define ITEM_READONLY           "ReadOnly"
#define ITEM_CHARACTERIZATION   "Characterization"

static ULONG
codesetsReadTable(struct MinList *codesetsList,STRPTR name)
{
    UBYTE 	   buf[512];
    register BPTR  fh;
    register ULONG res = FALSE;

    if (fh = Open(name,MODE_OLDFILE))
    {
        register struct codeset *codeset;

        if ((codeset = (struct codeset *) allocVecPooled (lib_pool,sizeof(struct codeset))))
        {
            register int i;

            memset(codeset,0,sizeof(struct codeset));

            for (i = 0; i<256; i++)
                codeset->table[i].code = codeset->table[i].ucs4 = i;

            while (readLine(fh,buf,sizeof(buf)))
            {
                register STRPTR result;

                if (*buf=='#') continue;

                if ((result = getConfigItem(buf,ITEM_STANDARD,strlen(ITEM_STANDARD)))) codeset->name = mystrdup(result);
                else if ((result = getConfigItem(buf,ITEM_ALTSTANDARD,strlen(ITEM_ALTSTANDARD)))) codeset->alt_name = mystrdup(result);
                     else if ((result = getConfigItem(buf,ITEM_READONLY,strlen(ITEM_READONLY)))) codeset->read_only = !!atoi(result);
                          else
                            if ((result = getConfigItem(buf,ITEM_CHARACTERIZATION,strlen(ITEM_CHARACTERIZATION))))
                            {
                                if ((result[0]=='_') && (result[1]=='(') && (result[2]=='"'))
                                {
                                    register STRPTR end = strchr(result + 3, '"');

                                    if (end)
                                    {
                                        codeset->characterization = mystrndup(result+3,end-(result+3));
                                    }
                                }
                            }
                            else
                            {
                                STRPTR       p = buf;
                                register int fmt2 = 0;

                                if ((*p=='=') || (fmt2 = ((*p=='0') || (*(p+1)=='x'))))
                                {
                                    p++;
                                    p += fmt2;

                                    i = strtol((const char *)p,(char **)&p,16);
                                    if (i>0 && i<256)
                                    {
                                        while (isspace(*p)) p++;

                                        if (!mystrnicmp(p,"U+",2))
                                        {
                                            p += 2;
                                            codeset->table[i].ucs4 = strtol((const char *)p,(char **)&p,16);
                                        }
                                        else
                                        {
                                            if (*p!='#') codeset->table[i].ucs4 = strtol((const char *)p,(char **)&p,0);
                                        }
                                    }
                                }
                            }
            }

            for (i = 0; i<256; i++)
            {
                UTF32 src = codeset->table[i].ucs4, *src_ptr = &src;
                UTF8  *dest_ptr = &codeset->table[i].utf8[1];

                CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
                *dest_ptr = 0;
                codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)(&codeset->table[i].utf8[1]);
            }

            copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
            qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);

            ADDTAIL(codesetsList,&codeset->node);

            res = TRUE;
        }

        Close(fh);
    }

    return res;
}

/**************************************************************************/

/*
 * Initialized and loads the codesets
 */

ULONG
codesetsInit (struct MinList * codesetsList)
{
    register struct codeset       *codeset;
    UTF32                         src;
    register struct FileInfoBlock *fib;
    register int                  i;

    ObtainSemaphore(&lib_poolSem);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset))))
    {
        ReleaseSemaphore(&lib_poolSem);
        return FALSE;
    }

    codeset->name 	      = mystrdup("ISO-8859-1 + Euro");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("West European (with EURO)");
    codeset->read_only 	      = 1;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i==164) src = 0x20AC; /* the EURO sign */
        else src = i;

        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }

    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-1");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("West European");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8 *dest_ptr = &codeset->table[i].utf8[1];

        src = i;
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof (codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-2");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Central/East European");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = iso_8859_2_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-3");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("South European");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = iso_8859_3_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-4");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("North European");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = iso_8859_4_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("KOI8-R");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Russian");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0x80) src = i;
        else src = koi8r_to_ucs4[i-0x80];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-5");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Slavic languages");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = iso_8859_5_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-9");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Turkish");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = iso_8859_9_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-15");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("West European II");
    codeset->read_only 	      = 0;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = iso_8859_15_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof (codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (!(codeset = allocVecPooled(lib_pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("AmigaPL");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("AmigaPL");
    codeset->read_only 	      = 1;

    for (i = 0; i<256; i++)
    {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        if (i<0xa0) src = i;
        else src = amigapl_to_ucs4[i-0xa0];
        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    copymem(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    ADDTAIL(codesetsList,&codeset->node);

    if (fib = AllocDosObject(DOS_FIB,NULL))
    {
        register BPTR dir;

        if ((dir = Lock("LIBS:Charsets",SHARED_LOCK)) && Examine(dir,fib) && (fib->fib_DirEntryType>=0))
        {
            register BPTR oldDir = CurrentDir(dir);

            while(ExNext(dir,fib))
            {
                if (fib->fib_DirEntryType>=0) continue;
                codesetsReadTable(codesetsList,fib->fib_FileName);
            }

            CurrentDir(oldDir);
        }

        if (dir) UnLock(dir);

        FreeDosObject(DOS_FIB,fib);
    }

  end:
    ReleaseSemaphore(&lib_poolSem);

    return TRUE;
}

/**************************************************************************/
/*
 * Cleanup the memory for the codeset
 */

void
codesetsCleanup(struct MinList *codesetsList)
{
    register struct codeset *code;

    while (code = (struct codeset *)RemHead(LIST(codesetsList)))
    {
        if (code->name) freeArbitrateVecPooled(code->name);
        if (code->alt_name) freeArbitrateVecPooled(code->alt_name);
        if (code->characterization) freeArbitrateVecPooled(code->characterization);
        freeArbitrateVecPooled(code);
    }
}

/**************************************************************************/

static struct codeset *
defaultCodeset(ULONG sem)
{
    UBYTE          	    buf[256];
    register struct codeset *codeset;

    if (sem) ObtainSemaphoreShared(&lib_sem);

    *buf = 0;
    GetVar("codeset_default",buf,sizeof(buf),GVF_GLOBAL_ONLY);

    if (!*buf || !(codeset = codesetsFind(&lib_codesets,buf)))
        codeset = lib_systemCodeset;

    if (sem) ReleaseSemaphore(&lib_sem);

    return codeset;
}

/**************************************************************************/

/*
 * Returns the given codeset.
 */

struct codeset *
codesetsFind(struct MinList *codesetsList,STRPTR name)
{
    register struct codeset *res = NULL;

    if (name && *name)
    {
        register struct codeset *mstate, *succ;

        for (mstate = (struct codeset *)codesetsList->mlh_Head; succ = (struct codeset *)mstate->node.mln_Succ; mstate = succ)
            if (!mystricmp(name,mstate->name) || !mystricmp(name,mstate->alt_name))
                break;

        if (succ) res = mstate;
    }

    return res;
}

/**************************************************************************/

struct codeset *LIBCALL
CodesetsSetDefaultA(REG (a0) STRPTR name,REG(a1) struct TagItem *attrs)
{
    register struct codeset *codeset;

    ObtainSemaphoreShared(&lib_sem);

    if (codeset = codesetsFind(&lib_codesets,name))
    {
        register ULONG flags;

        flags = GVF_SAVE_VAR | (GetTagData(CODESETSA_Save,FALSE,attrs) ? GVF_GLOBAL_ONLY : 0);

        SetVar("codeset_default",codeset->name,strlen(codeset->name),flags);
    }

    ReleaseSemaphore(&lib_sem);

    return codeset;
}

#ifdef __MORPHOS__
struct codeset *
LIB_CodesetsSetDefaultA(void)
{
    return CodesetsSetDefaultA((STRPTR)REG_A0,(struct TagItem *)REG_A1);
}
#endif

/**************************************************************************/

struct codeset *LIBCALL
CodesetsFindA(REG(a0) STRPTR name,REG(a1) struct TagItem *attrs)
{
    register struct codeset *codeset;

    ObtainSemaphoreShared(&lib_sem);

    codeset = codesetsFind(&lib_codesets,name);

    if (!codeset && GetTagData(CODESETSA_NoFail,TRUE,attrs))
        codeset = defaultCodeset(FALSE);

    ReleaseSemaphore(&lib_sem);

    return codeset;
}

#ifdef __MORPHOS__
struct codeset *
LIB_CodesetsFindA(void)
{
    return CodesetsFindA((STRPTR)REG_A0,(struct TagItem *)REG_A1);
}
#endif

/**************************************************************************/

/*
 * Returns the best codeset for the given text
 */

struct codeset *
codesetsFindBest(struct MinList *codesetsList,STRPTR text,int text_len,int *error_ptr)
{
    register struct codeset *codeset, *best_codeset = NULL;
    register int            best_errors = text_len;

    codeset = (struct codeset *)codesetsList->mlh_Head;

    while (codeset)
    {
        if (!codeset->read_only)
        {
            struct single_convert conv;
            register STRPTR       text_ptr = text;
            register int          i, errors = 0;

            for (i = 0; i<text_len; i++)
            {
                register unsigned char c = *text_ptr++;

                if (c)
                {
                    register int len = trailingBytesForUTF8[c];

                    conv.utf8[1] = c;
                    strncpy (&conv.utf8[2], text_ptr, len);
                    conv.utf8[2+len] = 0;
                    text_ptr += len;

                    if (!bsearch(&conv,codeset->table_sorted,256,sizeof(codeset->table_sorted[0]),(APTR)codesetsCmpUnicode))
                        errors++;
                }
                else break;
            }

            if (errors<best_errors)
            {
                best_codeset = codeset;
                best_errors = errors;
            }

            if (!best_errors) break;
        }

        codeset = (struct codeset *)codeset->node.mln_Succ;
    }

    if (!best_codeset) best_codeset = defaultCodeset(FALSE);
    if (error_ptr) *error_ptr = best_errors;

    return best_codeset;
}

/**************************************************************************/

struct codeset *LIBCALL
CodesetsFindBestA(REG(a0) STRPTR text,REG(d0) ULONG text_len,REG(a1) ULONG *error_ptr,REG(a2) struct TagItem *attrs)
{
    register struct codeset *codeset;

    ObtainSemaphoreShared(&lib_sem);

    codeset = codesetsFindBest(&lib_codesets,text,text_len,(int *)error_ptr);

    ReleaseSemaphore(&lib_sem);

    return codeset;
}

#ifdef __MORPHOS__
struct codeset *
LIB_CodesetsFindBestA(void)
{
    return CodesetsFindBestA((STRPTR)REG_A0,(ULONG)REG_D0,(ULONG *)REG_A1,(struct TagItem *)REG_A2);
}
#endif


/**************************************************************************/

/*
 * Returns the number of characters a utf8 string has. This is not
 * identically with the size of memory is required to hold the string.
 */

ULONG LIBCALL
CodesetsUTF8Len(REG(a0) UTF8 *str)
{
    register int           len;
    register unsigned char c;

    if (!str) return 0;

    len = 0;

    while ((c = *str++))
    {
        len++;
        str += trailingBytesForUTF8[c];
    }

    return (ULONG)len;
}

#ifdef __MORPHOS__
ULONG
LIB_CodesetsUTF8Len(void)
{
    return CodesetsUTF8Len((UTF8 *)REG_A0);
}
#endif


/**************************************************************************/

ULONG LIBCALL
CodesetsStrLenA(REG(a0) STRPTR str,REG(a1) struct TagItem *attrs)
{
    register struct codeset *codeset;
    register int            len, res;
    register STRPTR         src;
    register UBYTE          c;

    if (!str) return 0;

    if (!(codeset = (struct codeset *)GetTagData(CODESETSA_Codeset,NULL,attrs)))
        codeset = defaultCodeset(TRUE);

    len = GetTagData(CODESETSA_SourceLen,UINT_MAX,attrs);

    src = str;
    res = 0;

    while (((c = *src++) && (len--)))
        res += codeset->table[c].utf8[0];

    return (ULONG)res;
}

#ifdef __MORPHOS__
ULONG
LIB_CodesetsStrLenA(void)
{
    return CodesetsStrLenA((STRPTR)REG_A0,(struct TagItem *)REG_A1);
}
#endif


/**************************************************************************/
/*
 * Converts a UTF8 string to a given charset. Return the number of bytes
 * written to dest excluding the NULL byte (which is always ensured by this
 * function; it means a NULL str will produce "" as dest; anyway you should
 * check NULL str to not waste your time!).
 */

STRPTR LIBCALL
CodesetsUTF8ToStrA(REG(a0) struct TagItem *attrs)
{
    register UTF8   *str;
    register STRPTR dest;
    register ULONG  *destLenPtr;
    register int    n;

    str = (UTF8 *)GetTagData(CODESETSA_Source,NULL,attrs);
    if (!str) return NULL;

    dest = NULL;
    n    = 0;

    if (str)
    {
        struct convertMsg              msg;
        register struct codeset        *codeset;
        register struct Hook           *hook;
        register struct single_convert *f;
        UBYTE                 	       buf[256];
        register STRPTR                destIter = NULL, b = NULL;
        register ULONG                 destLen;
        register int                   i = 0;

        hook    = (struct Hook *)GetTagData(CODESETSA_DestHook,NULL,attrs);
        destLen = GetTagData(CODESETSA_DestLen,0,attrs);

        if (hook)
        {
            if (destLen<16 || destLen>sizeof(buf)) destLen = sizeof(buf);

            msg.state = CODESETV_Translating;
            b = buf;
            i = 0;
        }
        else
        {
            register APTR                   pool;
            register struct SignalSemaphore *sem;

            //if (destLen==0) return NULL;

            if (!(dest = (STRPTR)GetTagData(CODESETSA_Dest,NULL,attrs)) ||
                GetTagData(CODESETSA_AllocIfNeeded,TRUE,attrs))
            {
                register LONG  len;
                register UBYTE c, *s;

                len = 0;
                s   = str;

                while ((c = *s++))
                {
                    len++;
                    s += trailingBytesForUTF8[c];
                }

                if (!dest || (destLen<len+1))
                {
                    if (pool = (APTR)GetTagData(CODESETSA_Pool,NULL,attrs))
                    {
                        if (sem = (struct SignalSemaphore *)GetTagData(CODESETSA_PoolSem,NULL,attrs))
                        {
                            ObtainSemaphore(sem);
                        }

                        dest = allocVecPooled(pool,len+1);

                        if (sem) ReleaseSemaphore(sem);
                    }
                    else dest = allocArbitrateVecPooled(len+1);

                    destLen  = len+1;
                }

                if (!dest) return NULL;
            }

            destIter = dest;
        }

        if (!(codeset = (struct codeset *)GetTagData(CODESETSA_Codeset,NULL,attrs)))
            codeset = defaultCodeset(TRUE);

        for (; ; n++)
        {
            register UBYTE c, d;

            if (!hook)
            {
                if (n>=destLen-1) break;
            }

            if (c = *str)
            {
                if (c>127)
                {
                    register int lenAdd = trailingBytesForUTF8[c], lenStr = lenAdd+1;

                    BIN_SEARCH(codeset->table_sorted,0,255,mystrncmp(str,codeset->table_sorted[m].utf8+1,lenStr),f);

                    if (f) d = f->code;
                    else d = '_';

                    str += lenAdd;
                }
                else d = c;

                if (hook)
                {
                    *b++ = d;
                    i++;

                    if (i%(destLen-1)==0)
                    {
                        *b = 0;
                        msg.len = i;
                        CallHookPkt(hook,&msg,buf);

                        b  = buf;
                        *b = 0;
                        i  = 0;
                    }
                }
                else
                {
                    *destIter++ = d;
                }

                str++;
            }
            else break;
        }

        if (hook)
        {
            msg.state = CODESETV_End;
            msg.len   = i;
            *b        = 0;
            CallHookPkt(hook,&msg,buf);
        }
        else
        {
            *destIter = 0;
        }
    }

    if (destLenPtr = (ULONG *)GetTagData(CODESETSA_DestLenPtr,NULL,attrs))
        *destLenPtr = n;

    return dest;
}

#ifdef __MORPHOS__
STRPTR
LIB_CodesetsUTF8ToStrA(void)
{
    return CodesetsUTF8ToStrA((struct TagItem *)REG_A0);
}
#endif

/**************************************************************************/
/*
 * Converts a string and a charset to an UTF8. Returns the UTF8.
 * If a destination hook is supplied always return 0.
 * If from is NULL, it returns NULL and doesn't call the hook.
 */

UTF8 *LIBCALL
CodesetsUTF8CreateA(REG(a0) struct TagItem *attrs)
{
    register UTF8   *from;
    register STRPTR dest;
    register ULONG  fromLen, *destLenPtr;
    register int    n;

    dest = NULL;
    n    = 0;

    from = (STRPTR)GetTagData(CODESETSA_Source,NULL,attrs);
    fromLen = GetTagData(CODESETSA_SourceLen,UINT_MAX,attrs);

    if (from && fromLen)
    {
        struct convertMsg       msg;
        register struct codeset *codeset;
        register struct Hook    *hook;
        register ULONG          destLen;
        register int            i = 0;
        UBYTE          		buf[256];
        register UBYTE          *src, *destPtr = NULL, *b = NULL, c;

        if (!(codeset = (struct codeset *)GetTagData(CODESETSA_Codeset,NULL,attrs)))
            codeset = defaultCodeset(TRUE);

        hook    = (struct Hook *)GetTagData(CODESETSA_DestHook,NULL,attrs);
        destLen = GetTagData(CODESETSA_DestLen,0,attrs);

        if (hook)
        {
            if (destLen<16 || destLen>sizeof(buf)) destLen = sizeof(buf);

            msg.state = CODESETV_Translating;
            b = buf;
            i = 0;
        }
        else
        {
            if (!(dest = (STRPTR)GetTagData(CODESETSA_Dest,NULL,attrs)) ||
                GetTagData(CODESETSA_AllocIfNeeded,TRUE,attrs))
            {
                register int len, flen;

                flen = fromLen;
                len  = 0;
                src  = from;

                while (((c = *src++) && (flen--)))
                    len += codeset->table[c].utf8[0];

                if (!dest || (destLen<len+1))
                {
                    register APTR                   pool;
                    register struct SignalSemaphore *sem;

                    if (pool = (APTR)GetTagData(CODESETSA_Pool,NULL,attrs))
                    {
                        if (sem = (struct SignalSemaphore *)GetTagData(CODESETSA_PoolSem,NULL,attrs))
                        {
                            ObtainSemaphore(sem);
                        }

                        dest = allocVecPooled(pool,len+1);

                        if (sem) ReleaseSemaphore(sem);
                    }
                    else dest = allocArbitrateVecPooled(len+1);

                    destLen  = len;
                 }

                 if (!dest) return NULL;
            }

            destPtr = dest;
        }

        for (src = from; fromLen && (c = *src); src++, fromLen--)
        {
            register STRPTR utf8_seq;

            for (utf8_seq = &codeset->table[c].utf8[1]; (c = *utf8_seq); utf8_seq++)
            {
                if (!hook)
                {
                    if (n>=destLen) break;
                }

                if (hook)
                {
                    *b++ = c;
                    i++;

                    if (i%(destLen-1)==0)
                    {
                        *b = 0;
                        msg.len = i;
                        CallHookPkt(hook,&msg,buf);

                        b  = buf;
                        *b = 0;
                        i  = 0;
                    }
                }
                else
                {
                    *destPtr++ = c;
                }

                n++;
            }
        }

        if (hook)
        {
            msg.state = CODESETV_End;
            msg.len   = i;
            *b = 0;
            CallHookPkt(hook,&msg,buf);
        }
        else
        {
            *destPtr = 0;
        }
    }

    if (destLenPtr = (ULONG *)GetTagData(CODESETSA_DestLenPtr,NULL,attrs))
        *destLenPtr = n;

    return dest;
}

#ifdef __MORPHOS__
UTF8 *
LIB_CodesetsUTF8CreateA(void)
{
    return CodesetsUTF8CreateA((struct TagItem *)REG_A0);
}
#endif

/**************************************************************************/

static int
parseUtf8(STRPTR *ps)
{
    register STRPTR s = *ps;
    register int    wc, n, i;

    if (*s<0x80)
    {
        *ps = s+1;

        return *s;
    }

    if (*s<0xc2)
    {
        return -1;
    }
    else
    {
        if (*s<0xe0)
        {
            if ((s[1] & 0xc0)!=0x80)
            {
                return -1;
            }

            *ps = s+2;

            return ((s[0] & 0x1f)<<6) | (s[1] & 0x3f);
        }
        else
        {
            if (*s<0xf0)
            {
                n = 3;
            }
            else
            {
                if (*s<0xf8)
                {
                    n = 4;
                }
                else
                {
                    if (*s<0xfc)
                    {
                        n = 5;
                    }
                    else
                    {
                        if (*s<0xfe)
                        {
                            n = 6;
                        }
                        else
                        {
                            return -1;
                        }
                    }
                }
            }
        }
    }

    wc = *s++ & ((1<<(7-n))-1);

    for (i = 1; i<n; i++)
    {
        if ((*s & 0xc0) != 0x80)
        {
            return -1;
        }

        wc = (wc << 6) | (*s++ & 0x3f);
    }

    if (wc < (1 << (5 * n - 4)))
    {
        return -1;
    }

    *ps = s;

    return wc;
}

#define GOOD_UCS(c) \
     ((c) >= 160 && ((c) & ~0x3ff) != 0xd800 && \
      (c) != 0xfeff && (c) != 0xfffe && (c) != 0xffff)

ULONG LIBCALL
CodesetsIsValidUTF8(REG(a0) STRPTR s)
{
    STRPTR       t = s;
    register int n;

    while ((n = parseUtf8(&t)))
    {
        if (!GOOD_UCS(n))
        {
            return 0;
        }
    }

    return 1;
}

#ifdef __MORPHOS__
ULONG
LIB_CodesetsIsValidUTF8(void)
{
    return CodesetsIsValidUTF8((STRPTR)REG_A0);
}
#endif

/***********************************************************************/

void LIBCALL
CodesetsFreeVecPooledA(REG(a0) APTR pool,REG(a1) APTR mem,REG(a2) struct TagItem *attrs)
{
    if (pool && mem)
    {
        register struct SignalSemaphore *sem;

	if (sem = (struct SignalSemaphore *)GetTagData(CODESETSA_PoolSem,NULL,attrs))
    	    ObtainSemaphore(sem);

    	freeVecPooled(pool,mem);

        if (sem) ReleaseSemaphore(sem);
    }
}

#ifdef __MORPHOS__
void
LIB_CodesetsFreeVecPooledA(void)
{
    return CodesetsFreeVecPooledA((APTR)REG_A0,(APTR)REG_A1,(struct TagItem *)REG_A2);
}
#endif

/***********************************************************************/

