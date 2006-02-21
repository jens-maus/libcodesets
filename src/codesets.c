/***************************************************************************

 codesets.library - Amiga shared library for handling different codesets
 Copyright (C) 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
 Copyright (C) 2005-2006 by codesets.library Open Source Team

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 codesets.library project: http://sourceforge.net/projects/codesetslib/

 Most of the code included in this file was relicensed from GPL to LGPL
 from the source code of SimpleMail (http://www.sf.net/projects/simplemail)
 with full permissions by its authors.

 $Id$

***************************************************************************/

#include "lib.h"

#include <clib/alib_protos.h>

#include <diskfont/glyph.h>
#include <diskfont/diskfonttag.h>
#include <proto/diskfont.h>
#include <ctype.h>
#include <limits.h>

#include "codesets_table.h"
#include "convertUTF.h"

#include "SDI_stdarg.h"

#include "debug.h"

/**************************************************************************/

/// BIN_SEARCH()
// search a sorted array in O(log n) e.g.
// BIN_SEARCH(strings,0,sizeof(strings)/sizeof(strings[0]),strcmp(key,array[mid]),res);
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

///
/// mystrdup()
static STRPTR
mystrdup(STRPTR str)
{
    STRPTR new;
    int   len;

    if (!str) return NULL;

    len = strlen(str);
    if (!len) return NULL;

    if ((new = allocArbitrateVecPooled(len+1)))
        strcpy(new,str);

    return new;
}
///
/// mystrndup()
static STRPTR
mystrndup(STRPTR str1,int n)
{
    STRPTR dest;

    if ((dest = allocArbitrateVecPooled(n+1)))
    {
        if (str1) strncpy(dest,str1,n);
        else dest[0] = 0;

        dest[n] = 0;
    }

    return dest;
}
///
/// readLine()
static ULONG
readLine(BPTR fh,STRPTR buf,int size)
{
    STRPTR c;

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
///
/// getConfigItem()
static STRPTR
getConfigItem(STRPTR buf,STRPTR item,int len)
{
    if(!strnicmp(buf, item, len))
    {
        UBYTE c;

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
///
/// parseUtf8()
static int
parseUtf8(STRPTR *ps)
{
  STRPTR s = *ps;
  int    wc, n, i;

  ENTER();

  if(*s<0x80)
  {
    *ps = s+1;

    RETURN(*s);
    return *s;
  }

  if(*s<0xc2)
  {
    RETURN(-1);
    return -1;
  }
  else
  {
    if(*s<0xe0)
    {
      if((s[1] & 0xc0)!=0x80)
      {
        RETURN(-1);
        return -1;
      }

      *ps = s+2;

      RETURN(((s[0] & 0x1f)<<6) | (s[1] & 0x3f));
      return ((s[0] & 0x1f)<<6) | (s[1] & 0x3f);
    }
    else
    {
      if(*s<0xf0)
      {
        n = 3;
      }
      else
      {
        if(*s<0xf8)
        {
          n = 4;
        }
        else
        {
          if(*s<0xfc)
          {
            n = 5;
          }
          else
          {
            if(*s<0xfe)
            {
              n = 6;
            }
            else
            {
              RETURN(-1);
              return -1;
            }
          }
        }
      }
    }
  }

  wc = *s++ & ((1<<(7-n))-1);

  for(i = 1; i<n; i++)
  {
    if((*s & 0xc0) != 0x80)
    {
      RETURN(-1);
      return -1;
    }

    wc = (wc << 6) | (*s++ & 0x3f);
  }

  if(wc < (1 << (5 * n - 4)))
  {
    RETURN(-1);
    return -1;
  }

  *ps = s;

  RETURN(wc);
  return wc;
}

#define GOOD_UCS(c) \
     ((c) >= 160 && ((c) & ~0x3ff) != 0xd800 && \
      (c) != 0xfeff && (c) != 0xfffe && (c) != 0xffff)

ULONG LIBFUNC
CodesetsIsValidUTF8(REG(a0, STRPTR s))
{
  STRPTR t = s;
  int n;

  ENTER();

  while((n = parseUtf8(&t)))
  {
    if(!GOOD_UCS(n))
    {
      RETURN(FALSE);
      return FALSE;
    }
  }

  RETURN(TRUE);
  return TRUE;
}

LIBSTUB(CodesetsIsValidUTF8, ULONG, REG(a0, STRPTR s))
{
  #ifdef __MORPHOS__
  return CodesetsIsValidUTF8((STRPTR)REG_A0);
  #else
  return CodesetsIsValidUTF8(s);
  #endif
}
///

/**************************************************************************/

/// defaultCodeset()
static struct codeset *
defaultCodeset(ULONG sem)
{
  char buf[256];
  struct codeset *codeset;

  ENTER();

  if(sem)
    ObtainSemaphoreShared(&CodesetsBase->libSem);

  *buf = 0;
  GetVar("codeset_default",buf,sizeof(buf),GVF_GLOBAL_ONLY);

  if(!*buf || !(codeset = codesetsFind(&CodesetsBase->codesets,buf)))
    codeset = CodesetsBase->systemCodeset;

  if(sem)
    ReleaseSemaphore(&CodesetsBase->libSem);

  RETURN(codeset);
  return codeset;
}
///
/// codesetsCmpUnicode()
// The compare function
static int
codesetsCmpUnicode(struct single_convert *arg1,struct single_convert *arg2)
{
  return strcmp((char*)arg1->utf8+1, (char*)arg2->utf8+1);
}
///
/// codesetsReadTable()

#define ITEM_STANDARD           "Standard"
#define ITEM_ALTSTANDARD        "AltStandard"
#define ITEM_READONLY           "ReadOnly"
#define ITEM_CHARACTERIZATION   "Characterization"

// Reads a coding table and adds it
static ULONG
codesetsReadTable(struct MinList *codesetsList,STRPTR name)
{
  char buf[512];
  BPTR  fh;
  ULONG res = FALSE;

  ENTER();

  D(DBF_STARTUP, "trying to fetch charset file '%s'...", name);

  if((fh = Open(name, MODE_OLDFILE)))
  {
    struct codeset *codeset;

    if((codeset = (struct codeset *)allocVecPooled(CodesetsBase->pool,sizeof(struct codeset))))
    {
      int i;

      memset(codeset,0,sizeof(struct codeset));

      for(i = 0; i<256; i++)
        codeset->table[i].code = codeset->table[i].ucs4 = i;

      while(readLine(fh, buf, sizeof(buf)))
      {
        STRPTR result;

        if(*buf=='#')
          continue;

        if((result = getConfigItem(buf,ITEM_STANDARD,strlen(ITEM_STANDARD))))
          codeset->name = mystrdup(result);
        else if(codeset->name == NULL) // a valid file starts with standard and nothing else!!
          break;
        else if((result = getConfigItem(buf,ITEM_ALTSTANDARD,strlen(ITEM_ALTSTANDARD))))
          codeset->alt_name = mystrdup(result);
        else if((result = getConfigItem(buf,ITEM_READONLY,strlen(ITEM_READONLY))))
          codeset->read_only = !!atoi(result);
        else if((result = getConfigItem(buf,ITEM_CHARACTERIZATION,strlen(ITEM_CHARACTERIZATION))))
        {
          if((result[0]=='_') && (result[1]=='(') && (result[2]=='"'))
          {
            STRPTR end = strchr(result + 3, '"');

            if(end)
              codeset->characterization = mystrndup(result+3,end-(result+3));
          }
          else
            codeset->characterization = mystrdup(result);
        }
        else
        {
          STRPTR p = buf;
          int fmt2 = 0;

          if((*p=='=') || (fmt2 = ((*p=='0') || (*(p+1)=='x'))))
          {
            p++;
            p += fmt2;

            i = strtol((const char *)p,(char **)&p,16);
            if(i>0 && i<256)
            {
              while(isspace(*p)) p++;

              if(!strnicmp(p, "U+", 2))
              {
                p += 2;
                codeset->table[i].ucs4 = strtol((const char *)p,(char **)&p,16);
              }
              else
              {
                if(*p!='#')
                  codeset->table[i].ucs4 = strtol((const char *)p,(char **)&p,0);
              }
            }
          }
        }
      }

      // check if there is not already codeset with the same name in here
      if(codeset->name != NULL && !(codesetsFind(codesetsList, codeset->name)))
      {
        for(i=0; i<256; i++)
        {
          UTF32 src = codeset->table[i].ucs4, *src_ptr = &src;
          UTF8  *dest_ptr = &codeset->table[i].utf8[1];

          CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
          *dest_ptr = 0;
          codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)(&codeset->table[i].utf8[1]);
        }

        memcpy(codeset->table_sorted, codeset->table, sizeof(codeset->table));
        qsort(codeset->table_sorted, 256, sizeof(codeset->table[0]), (int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
        AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);

        res = TRUE;
      }
      else
      {
        // cleanup
        if(codeset->name)             freeArbitrateVecPooled(codeset->name);
        if(codeset->alt_name)         freeArbitrateVecPooled(codeset->alt_name);
        if(codeset->characterization) freeArbitrateVecPooled(codeset->characterization);
        freeArbitrateVecPooled(codeset);
      }
    }

    Close(fh);
  }

  RETURN(res);
  return res;
}
///
/// codesetsScanDir()
static void
codesetsScanDir(struct MinList *codesetsList, STRPTR dirPath)
{
  ENTER();

  if(dirPath != NULL && dirPath[0] != '\0')
  {
    BPTR dirLock;

    if((dirLock = Lock(dirPath, ACCESS_READ)))
    {
      struct ExAllControl *eac;

      D(DBF_STARTUP, "scanning directory '%s' for codesets tables", dirPath);

      if((eac = AllocDosObject(DOS_EXALLCONTROL, NULL)))
      {
        struct ExAllData *ead;
        struct ExAllData *eabuffer;
        LONG more;

        eac->eac_LastKey = 0;
        eac->eac_MatchString = NULL;
        eac->eac_MatchFunc = NULL;

        if((eabuffer = allocVecPooled(CodesetsBase->pool, 32768)))
        {
          do
          {
            more = ExAll(dirLock, eabuffer, 32768, ED_TYPE, eac);
            if(!more && IoErr() != ERROR_NO_MORE_ENTRIES)
              break;

            if(eac->eac_Entries == 0)
              continue;

            ead = (struct ExAllData *)eabuffer;
            do
            {
              // we only take that ead if it is a file (ed_Type < 0)
              if(ead->ed_Type < 0)
              {
                char filePath[620];

                strcpy(filePath, dirPath);
                AddPart(filePath, (char *)ead->ed_Name, 620);

                codesetsReadTable(codesetsList, filePath);
              }
            }
            while((ead = ead->ed_Next));
          }
          while(more);

          freeArbitrateVecPooled(eabuffer);
        }

        FreeDosObject(DOS_EXALLCONTROL, eac);
      }

      UnLock(dirLock);
    }
  }

  LEAVE();
}

///
/// codesetsInit()
// Initialized and loads the codesets
BOOL
codesetsInit(struct MinList * codesetsList)
{
  struct codeset       *codeset = NULL;
  UTF32                src;
  int                  i;
  #if defined(__amigaos4__)
  int                  nextMIB = 3;
  #endif

  ENTER();

  ObtainSemaphore(&CodesetsBase->poolSem);

  NewList((struct List *)&CodesetsBase->codesets);

  // to make the list of the supported codesets complete we also add a
  // fake 'UTF-8' only so that our users can query for that codeset as well.
  if(!(codeset = allocVecPooled(CodesetsBase->pool, sizeof(struct codeset)))) goto end;
  codeset->name 	          = mystrdup("UTF-8");
  codeset->alt_name 	      = NULL;
  codeset->characterization = mystrdup("Unicode");
  codeset->read_only 	      = 0;
  AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  CodesetsBase->utf8Codeset = codeset;

  // on AmigaOS4 we can use diskfont.library to inquire charset information as
  // it comes with a quite rich implementation of different charsets.
  #if defined(__amigaos4__)
  do
  {
    char *mimename;
    char *ianaName;
    ULONG *mapTable;
    int curMIB = nextMIB;

    nextMIB = (ULONG)ObtainCharsetInfo(DFCS_NUMBER, curMIB, DFCS_NEXTNUMBER);
    if(nextMIB == 0)
      break;

    mapTable = (ULONG *)ObtainCharsetInfo(DFCS_NUMBER, curMIB, DFCS_MAPTABLE);
    mimename = (char *)ObtainCharsetInfo(DFCS_NUMBER, curMIB, DFCS_MIMENAME);
    ianaName = (char *)ObtainCharsetInfo(DFCS_NUMBER, curMIB, DFCS_NAME);
    if(mapTable && mimename && !(codesetsFind(codesetsList, mimename)))
    {
      D(DBF_STARTUP, "loading charset '%s' from diskfont.library...", mimename);

      if(!(codeset = allocVecPooled(CodesetsBase->pool, sizeof(struct codeset)))) goto end;
      codeset->name 	          = mystrdup(mimename);
      codeset->alt_name 	      = NULL;
      codeset->characterization = mystrdup(ianaName);
      codeset->read_only 	      = 0;

      for(i=0; i<256; i++)
      {
        UTF32 *src_ptr = &src;
        UTF8  *dest_ptr = &codeset->table[i].utf8[1];

        src = mapTable[i];

        codeset->table[i].code = i;
        codeset->table[i].ucs4 = src;
        CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr, src_ptr+1, &dest_ptr, dest_ptr+6, CONVFLG_StrictConversion);
        *dest_ptr = 0;
        codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
      }
    	memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
      qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
      AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
    }
  }
  while(TRUE);
  #endif

  D(DBF_STARTUP, "loading charsets from Libs:Charsets...");

  // we try to walk to the LIBS:Charsets directory on our own and readin our
  // own charset tables
  codesetsScanDir(codesetsList, "LIBS:Charsets");

  //
  // now we go and initialize our internally supported codesets but only if
  // we have not already loaded a charset with the same name
  //
  D(DBF_STARTUP, "initializing internal charsets...");

  // ISO-8859-1 + EURO
  if(!(codesetsFind(codesetsList, "ISO-8859-1 + Euro")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool, sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-1 + Euro");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("West European (with EURO)");
    codeset->read_only 	      = 1;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i==164)
        src = 0x20AC; /* the EURO sign */
      else
        src = i;

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr, src_ptr+1, &dest_ptr, dest_ptr+6, CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
	  memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-1
  if(!(codesetsFind(codesetsList, "ISO-8859-1")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool, sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-1");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("West European");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8 *dest_ptr = &codeset->table[i].utf8[1];

      src = i;

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr, src_ptr+1, &dest_ptr, dest_ptr+6, CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof (codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-2
  if(!(codesetsFind(codesetsList, "ISO-8859-2")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool, sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-2");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Central/East European");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = iso_8859_2_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr, src_ptr+1, &dest_ptr,dest_ptr+6, CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted, codeset->table, sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-3
  if(!(codesetsFind(codesetsList, "ISO-8859-3")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool, sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-3");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("South European");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = iso_8859_3_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-4
  if(!(codesetsFind(codesetsList, "ISO-8859-4")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-4");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("North European");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = iso_8859_4_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-5
  if(!(codesetsFind(codesetsList, "ISO-8859-5")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-5");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Slavic languages");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = iso_8859_5_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-9
  if(!(codesetsFind(codesetsList, "ISO-8859-9")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("ISO-8859-9");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("Turkish");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = iso_8859_9_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-15
  if(!(codesetsFind(codesetsList, "ISO-8859-15")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
    codeset->name 	          = mystrdup("ISO-8859-15");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("West European II");
    codeset->read_only 	      = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = iso_8859_15_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof (codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // ISO-8859-16
  if(!(codesetsFind(codesetsList, "ISO-8859-16")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
	  codeset->name             = mystrdup("ISO-8859-16");
  	codeset->alt_name         = NULL;
	  codeset->characterization = mystrdup("South-Eastern European");
  	codeset->read_only        = 0;
	  for(i=0;i<256;i++)
  	{
      UTF32 *src_ptr = &src;
      UTF8 *dest_ptr = &codeset->table[i].utf8[1];

      if(i < 0xa0)
        src = i;
		  else
        src = iso_8859_16_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr, src_ptr+1, &dest_ptr, dest_ptr+6, CONVFLG_StrictConversion);
      *dest_ptr = 0;
	  	codeset->table[i].utf8[0] = (ULONG)dest_ptr - (ULONG)&codeset->table[i].utf8[1];
  	}
	  memcpy(codeset->table_sorted, codeset->table, sizeof(codeset->table));
  	qsort(codeset->table_sorted, 256, sizeof(codeset->table[0]), (int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // KOI8-R
  if(!(codesetsFind(codesetsList, "KOI8-R")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
    codeset->name 	            = mystrdup("KOI8-R");
    codeset->alt_name 	        = NULL;
    codeset->characterization   = mystrdup("Russian");
    codeset->read_only 	        = 0;
    for(i = 0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0x80)
        src = i;
      else
        src = koi8r_to_ucs4[i-0x80];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // AmigaPL
  if(!(codesetsFind(codesetsList, "AmigaPL")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
    codeset->name 	      = mystrdup("AmigaPL");
    codeset->alt_name 	      = NULL;
    codeset->characterization = mystrdup("AmigaPL");
    codeset->read_only 	      = 1;
    for(i=0; i<256; i++)
    {
      UTF32 *src_ptr = &src;
      UTF8  *dest_ptr = &codeset->table[i].utf8[1];

      if(i<0xa0)
        src = i;
      else
        src = amigapl_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr,src_ptr+1,&dest_ptr,dest_ptr+6,CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (ULONG)dest_ptr-(ULONG)&codeset->table[i].utf8[1];
    }
    memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
    qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1,const void *arg2))codesetsCmpUnicode);
    AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

  // Amiga-1251
  if(!(codesetsFind(codesetsList, "Amiga-1251")))
  {
    if(!(codeset = allocVecPooled(CodesetsBase->pool,sizeof(struct codeset)))) goto end;
	  codeset->name             = mystrdup("Amiga-1251");
  	codeset->alt_name         = NULL;
	  codeset->characterization = mystrdup("Amiga-1251");
  	codeset->read_only        = 1;
	  for(i=0; i<256; i++)
  	{
      UTF32 *src_ptr = &src;
      UTF8 *dest_ptr = &codeset->table[i].utf8[1];

      if(i < 0xa0)
        src = i;
      else
        src = amiga1251_to_ucs4[i-0xa0];

      codeset->table[i].code = i;
      codeset->table[i].ucs4 = src;
      CodesetsConvertUTF32toUTF8((const UTF32 **)&src_ptr, src_ptr+1, &dest_ptr, dest_ptr+6, CONVFLG_StrictConversion);
      *dest_ptr = 0;
      codeset->table[i].utf8[0] = (char*)dest_ptr - (char*)&codeset->table[i].utf8[1];
  	}
	  memcpy(codeset->table_sorted,codeset->table,sizeof(codeset->table));
  	qsort(codeset->table_sorted,256,sizeof(codeset->table[0]),(int (*)(const void *arg1, const void *arg2))codesetsCmpUnicode);
	  AddTail((struct List *)codesetsList, (struct Node *)&codeset->node);
  }

end:
  ReleaseSemaphore(&CodesetsBase->poolSem);

  RETURN(codeset != 0);
  return codeset != NULL;
}

///
/// codesetsCleanup()
// Cleanup the memory for the codeset
void
codesetsCleanup(struct MinList *codesetsList)
{
  struct codeset *code;

  ENTER();

  while((code = (struct codeset *)RemHead((struct List *)codesetsList)))
  {
    if(code->name) freeArbitrateVecPooled(code->name);
    if(code->alt_name) freeArbitrateVecPooled(code->alt_name);
    if(code->characterization) freeArbitrateVecPooled(code->characterization);

    freeArbitrateVecPooled(code);
  }

  LEAVE();
}

///
/// codesetsFind()
// Returns the given codeset.
struct codeset *
codesetsFind(struct MinList *codesetsList, STRPTR name)
{
  struct codeset *res = NULL;

  ENTER();

  if(name && *name)
  {
    struct codeset *mstate, *succ;

    for(mstate = (struct codeset *)codesetsList->mlh_Head; (succ = (struct codeset *)mstate->node.mln_Succ); mstate = succ)
    {
      if(!stricmp(name, mstate->name) || (mstate->alt_name != NULL && !stricmp(name, mstate->alt_name)))
        break;
    }

    if(succ)
      res = mstate;
  }

  RETURN(res);
  return res;
}
///
/// codesetsFindBest()
// Returns the best codeset for the given text
struct codeset *
codesetsFindBest(struct MinList *codesetsList,STRPTR text,int text_len,int *error_ptr)
{
  struct codeset *codeset, *best_codeset = NULL;
  int            best_errors = text_len;

  codeset = (struct codeset *)codesetsList->mlh_Head;

  while(codeset)
  {
    if(!codeset->read_only && codeset != CodesetsBase->utf8Codeset)
    {
      struct single_convert conv;
      STRPTR       text_ptr = text;
      int          i, errors = 0;

      for(i = 0; i<text_len; i++)
      {
        unsigned char c = *text_ptr++;

        if(c)
        {
          int len = trailingBytesForUTF8[c];

          conv.utf8[1] = c;
          strncpy((char*)&conv.utf8[2], text_ptr, len);
          conv.utf8[2+len] = 0;
          text_ptr += len;

          if(!bsearch(&conv,codeset->table_sorted,256,sizeof(codeset->table_sorted[0]),(APTR)codesetsCmpUnicode))
            errors++;
        }
        else
          break;
      }

      if(errors<best_errors)
      {
        best_codeset = codeset;
        best_errors = errors;
      }

      if(!best_errors)
        break;
    }

    codeset = (struct codeset *)codeset->node.mln_Succ;
  }

  if(!best_codeset)
    best_codeset = defaultCodeset(FALSE);

  if(error_ptr)
    *error_ptr = best_errors;

  RETURN(best_codeset);
  return best_codeset;
}
///

/**************************************************************************/

/// CodesetsSupportedA()
STRPTR *LIBFUNC
CodesetsSupportedA(REG(a0, UNUSED struct TagItem * attrs))
{
  STRPTR *array = NULL;
  struct TagItem *tstate = attrs;
  struct TagItem *tag;
  int numCodesets;

  ENTER();

  // first we need to check how many codesets our supplied
  // lists carry.
  numCodesets = countNodes(&CodesetsBase->codesets);
  while((tag = NextTagItem(&tstate)))
  {
    if(tag->ti_Tag == CODESETSA_CodesetList && tag->ti_Data != 0)
      numCodesets += countNodes((struct MinList *)tag->ti_Data);
  }

  // now that we know how many codesets we have in our lists we
  // can put their names into our string arrays
  if(numCodesets > 0)
  {
    if((array = allocArbitrateVecPooled((numCodesets+1)*sizeof(STRPTR))))
    {
      struct codeset *code;
      struct codeset *succ;
      int i=0;

      // reset the tstate
      tstate = attrs;

      ObtainSemaphoreShared(&CodesetsBase->libSem);

      // first we walk through the internal codesets list and
      // add the names
      for(code = (struct codeset *)CodesetsBase->codesets.mlh_Head; (succ = (struct codeset *)code->node.mln_Succ); code = succ, i++)
        array[i] = code->name;

      // then we also iterate through our private codesets list
      while((tag = NextTagItem(&tstate)))
      {
        if(tag->ti_Tag == CODESETSA_CodesetList && tag->ti_Data != 0)
        {
          struct MinList *list = (struct MinList *)tag->ti_Data;

          for(code = (struct codeset *)list->mlh_Head; (succ = (struct codeset *)code->node.mln_Succ); code = succ, i++)
            array[i] = code->name;
        }
      }

      array[i] = NULL;

      ReleaseSemaphore(&CodesetsBase->libSem);
    }
  }

  RETURN(array);
  return array;
}

LIBSTUB(CodesetsSupportedA, STRPTR*, REG(a0, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsSupportedA((struct TagItem *)REG_A0);
  #else
  return CodesetsSupportedA(attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsSupported, STRPTR*, ...)
{
  STRPTR* res;
  VA_LIST args;

  VA_START(args, self);
  res = CodesetsSupportedA(VA_ARG(args, struct TagItem *));
  VA_END(args);

  return res;
}
#endif

///
/// CodesetsFreeA()
void LIBFUNC
CodesetsFreeA(REG(a0, APTR obj),
              REG(a1, UNUSED struct TagItem *attrs))
{
  ENTER();

  if(obj)
    freeArbitrateVecPooled(obj);

  LEAVE();
}

LIBSTUB(CodesetsFreeA, void, REG(a0, APTR obj), REG(a1, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsFreeA((APTR)REG_A0,(struct TagItem *)REG_A1);
  #else
  return CodesetsFreeA(obj, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsFree, void, REG(a0, APTR obj), ...)
{
  VA_LIST args;

  VA_START(args, obj);
  CodesetsFreeA(obj, VA_ARG(args, struct TagItem *));
  VA_END(args);
}
#endif

///
/// CodesetsSetDefaultA()
struct codeset *LIBFUNC
CodesetsSetDefaultA(REG(a0, STRPTR name),
                    REG(a1, struct TagItem *attrs))
{
  struct codeset *codeset;

  ENTER();

  ObtainSemaphoreShared(&CodesetsBase->libSem);

  if((codeset = codesetsFind(&CodesetsBase->codesets,name)))
  {
    ULONG flags;

    flags = GVF_SAVE_VAR | (GetTagData(CODESETSA_Save,FALSE,attrs) ? GVF_GLOBAL_ONLY : 0);

    SetVar("codeset_default",codeset->name,strlen(codeset->name),flags);
  }

  ReleaseSemaphore(&CodesetsBase->libSem);

  RETURN(codeset);
  return codeset;
}

LIBSTUB(CodesetsSetDefaultA, struct codeset *, REG(a0, STRPTR name), REG(a1, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsSetDefaultA((STRPTR)REG_A0,(struct TagItem *)REG_A1);
  #else
  return CodesetsSetDefaultA(name, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsSetDefault, struct codeset *, REG(a0, STRPTR name), ...)
{
  struct codeset *cs;
  VA_LIST args;

  VA_START(args, name);
  cs = CodesetsSetDefaultA(name, VA_ARG(args, struct TagItem *));
  VA_END(args);

  return cs;
}
#endif

///
/// CodesetsFindA()
struct codeset *LIBFUNC
CodesetsFindA(REG(a0, STRPTR name), REG(a1, struct TagItem *attrs))
{
  struct codeset *codeset = NULL;

  ENTER();

  ObtainSemaphoreShared(&CodesetsBase->libSem);

  // if no name pointer was supplied we have to return
  // the default codeset only.
  if(name != NULL)
  {
    // we first walk through our internal list and check if we
    // can find the requested codeset
    codeset = codesetsFind(&CodesetsBase->codesets, name);

    if(codeset == NULL && attrs != NULL)
    {
      struct TagItem *tstate = attrs;
      struct TagItem *tag;

      // now we walk through our taglist and check if the user
      // supplied
      while((tag = NextTagItem(&tstate)))
      {
        if(tag->ti_Tag == CODESETSA_CodesetList && tag->ti_Data != 0)
        {
          struct MinList *list = (struct MinList *)tag->ti_Data;

          if((codeset = codesetsFind(list, name)) != NULL)
            break;
        }
      }
    }
  }

  // check if we found something or not.
  if(codeset == NULL && (attrs == NULL || GetTagData(CODESETSA_NoFail, TRUE, attrs)))
    codeset = defaultCodeset(FALSE);

  ReleaseSemaphore(&CodesetsBase->libSem);

  RETURN(codeset);
  return codeset;
}

LIBSTUB(CodesetsFindA, struct codeset *, REG(a0, STRPTR name), REG(a1, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsFindA((STRPTR)REG_A0,(struct TagItem *)REG_A1);
  #else
  return CodesetsFindA(name, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsFind, struct codeset *, REG(a0, STRPTR name), ...)
{
  struct codeset *cs;
  VA_LIST args;

  VA_START(args, name);
  cs = CodesetsFindA(name, VA_ARG(args, struct TagItem *));
  VA_END(args);

  return cs;
}
#endif
///
/// CodesetsFindBestA()
struct codeset *LIBFUNC
CodesetsFindBestA(REG(a0, STRPTR text),
                  REG(d0, ULONG text_len),
                  REG(a1, ULONG *error_ptr),
                  REG(a2, struct TagItem *attrs))
{
  struct codeset *codeset = NULL;

  ENTER();

  ObtainSemaphoreShared(&CodesetsBase->libSem);

  // if no text pointer was supplied we have to return
  // the default codeset only.
  if(text != NULL)
  {
    // we first walk through our internal list and check if we
    // can find the requested codeset
    codeset = codesetsFindBest(&CodesetsBase->codesets, text, text_len, (int *)error_ptr);

    if(codeset == NULL && attrs != NULL)
    {
      struct TagItem *tstate = attrs;
      struct TagItem *tag;

      // now we walk through our taglist and check if the user
      // supplied
      while((tag = NextTagItem(&tstate)))
      {
        if(tag->ti_Tag == CODESETSA_CodesetList && tag->ti_Data != 0)
        {
          struct MinList *list = (struct MinList *)tag->ti_Data;

          if((codeset = codesetsFindBest(list, text, text_len, (int *)error_ptr)) != NULL)
            break;
        }
      }
    }
  }

  ReleaseSemaphore(&CodesetsBase->libSem);

  RETURN(codeset);
  return codeset;
}

LIBSTUB(CodesetsFindBestA, struct codeset *, REG(a0, STRPTR text),
                                             REG(d0, ULONG text_len),
                                             REG(a1, ULONG *error_ptr),
                                             REG(a2, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsFindBestA((STRPTR)REG_A0,(ULONG)REG_D0,(ULONG *)REG_A1,(struct TagItem *)REG_A2);
  #else
  return CodesetsFindBestA(text, text_len, error_ptr, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsFindBest, struct codeset *, REG(a0, STRPTR text),
                                              REG(d0, ULONG text_len),
                                              REG(a1, ULONG *error_ptr), ...)
{
  struct codeset *cs;
  VA_LIST args;

  VA_START(args, error_ptr);
  cs = CodesetsFindBestA(text, text_len, error_ptr, VA_ARG(args, struct TagItem *));
  VA_END(args);

  return cs;
}
#endif
///
/// CodesetsUTF8Len()
// Returns the number of characters a utf8 string has. This is not
// identically with the size of memory is required to hold the string.
ULONG LIBFUNC
CodesetsUTF8Len(REG(a0, UTF8 *str))
{
  int           len;
  unsigned char c;

  ENTER();

  if(!str)
    return 0;

  len = 0;

  while((c = *str++))
  {
    len++;
    str += trailingBytesForUTF8[c];
  }

  RETURN((ULONG)len);
  return (ULONG)len;
}

LIBSTUB(CodesetsUTF8Len, ULONG, REG(a0, UTF8* str))
{
  #ifdef __MORPHOS__
  return CodesetsUTF8Len((UTF8 *)REG_A0);
  #else
  return CodesetsUTF8Len(str);
  #endif
}
///
/// CodesetsStrLenA()
ULONG LIBFUNC
CodesetsStrLenA(REG(a0, STRPTR str),
                REG(a1, struct TagItem *attrs))
{
  struct codeset *codeset;
  int            len, res;
  STRPTR         src;
  UBYTE          c;

  ENTER();

  if(!str)
    return 0;

  if(!(codeset = (struct codeset *)GetTagData(CODESETSA_Codeset, 0, attrs)))
    codeset = defaultCodeset(TRUE);

  len = GetTagData(CODESETSA_SourceLen,UINT_MAX,attrs);

  src = str;
  res = 0;

  while(((c = *src++) && (len--)))
    res += codeset->table[c].utf8[0];

  RETURN((ULONG)res);
  return (ULONG)res;
}

LIBSTUB(CodesetsStrLenA, ULONG, REG(a0, STRPTR str),
                                REG(a1, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsStrLenA((STRPTR)REG_A0,(struct TagItem *)REG_A1);
  #else
  return CodesetsStrLenA(str, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsStrLen, ULONG, REG(a0, STRPTR str), ...)
{
  ULONG res;
  VA_LIST args;

  VA_START(args, str);
  res = CodesetsStrLenA(str, VA_ARG(args, struct TagItem *));
  VA_END(args);

  return res;
}
#endif
///
/// CodesetsUTF8ToStrA()
// Converts an UTF8 string to a given charset. Return the number of bytes
// written to dest excluding the NULL byte (which is always ensured by this
// function; it means a NULL str will produce "" as dest; anyway you should
// check NULL str to not waste your time!).
STRPTR LIBFUNC
CodesetsUTF8ToStrA(REG(a0, struct TagItem *attrs))
{
  UTF8   *str;
  STRPTR dest;
  ULONG  *destLenPtr;
  ULONG  n;

  ENTER();

  str = (UTF8 *)GetTagData(CODESETSA_Source, 0, attrs);
  if(!str)
    return NULL;

  dest = NULL;
  n    = 0;

  if(str)
  {
    struct convertMsg     msg;
    struct codeset        *codeset;
    struct Hook           *hook;
    struct single_convert *f;
    char                 	buf[256];
    STRPTR                destIter = NULL, b = NULL;
    ULONG                 destLen;
    int                   i = 0;

    hook    = (struct Hook *)GetTagData(CODESETSA_DestHook, 0, attrs);
    destLen = GetTagData(CODESETSA_DestLen,0,attrs);

    if(hook)
    {
      if(destLen<16 || destLen>sizeof(buf))
        destLen = sizeof(buf);

      msg.state = CODESETV_Translating;
      b = buf;
      i = 0;
    }
    else
    {
      APTR                   pool;
      struct SignalSemaphore *sem;

      if(!(dest = (STRPTR)GetTagData(CODESETSA_Dest, 0, attrs)) ||
        GetTagData(CODESETSA_AllocIfNeeded,TRUE,attrs))
      {
        ULONG len;
        UBYTE c, *s;

        len = 0;
        s   = str;

        while ((c = *s++))
        {
            len++;
            s += trailingBytesForUTF8[c];
        }

        if(!dest || (destLen<len+1))
        {
          if((pool = (APTR)GetTagData(CODESETSA_Pool, 0, attrs)))
          {
            if((sem = (struct SignalSemaphore *)GetTagData(CODESETSA_PoolSem, 0, attrs)))
            {
              ObtainSemaphore(sem);
            }

            dest = allocVecPooled(pool,len+1);

            if(sem)
              ReleaseSemaphore(sem);
          }
          else
            dest = allocArbitrateVecPooled(len+1);

          destLen  = len+1;
        }

        if(!dest)
          return NULL;
      }

      destIter = dest;
    }

    if(!(codeset = (struct codeset *)GetTagData(CODESETSA_Codeset, 0, attrs)))
      codeset = defaultCodeset(TRUE);

    for(;;n++)
    {
      UBYTE c, d;

      if(!hook)
      {
        if(n>=destLen-1)
          break;
      }

      if((c = *str))
      {
        if(c>127)
        {
          int lenAdd = trailingBytesForUTF8[c], lenStr = lenAdd+1;

          BIN_SEARCH(codeset->table_sorted, 0, 255, strncmp((char *)str, (char *)codeset->table_sorted[m].utf8+1, lenStr), f);

          if(f)
            d = f->code;
          else
            d = '_';

          str += lenAdd;
        }
        else
          d = c;

        if(hook)
        {
          *b++ = d;
          i++;

          if(i%(destLen-1)==0)
          {
            *b = 0;
            msg.len = i;
            CallHookPkt(hook, &msg, buf);

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
      else
        break;
    }

    if(hook)
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

  if((destLenPtr = (ULONG *)GetTagData(CODESETSA_DestLenPtr, 0, attrs)))
    *destLenPtr = n;

  RETURN(dest);
  return dest;
}

LIBSTUB(CodesetsUTF8ToStrA, STRPTR, REG(a0, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsUTF8ToStrA((struct TagItem *)REG_A0);
  #else
  return CodesetsUTF8ToStrA(attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsUTF8ToStr, STRPTR, ...)
{
  STRPTR res;
  VA_LIST args;

  VA_START(args, self);
  res = CodesetsUTF8ToStrA(VA_ARG(args, struct TagItem *));
  VA_END(args);

  return res;
}
#endif

///
/// CodesetsUTF8CreateA()
// Converts a string and a charset to an UTF8. Returns the UTF8.
// If a destination hook is supplied always return 0.
// If from is NULL, it returns NULL and doesn't call the hook.
UTF8 *LIBFUNC
CodesetsUTF8CreateA(REG(a0, struct TagItem *attrs))
{
  UTF8   *from;
  UTF8   *dest;
  ULONG  fromLen, *destLenPtr;
  ULONG  n;

  ENTER();

  dest = NULL;
  n    = 0;

  from = (UTF8*)GetTagData(CODESETSA_Source, 0, attrs);
  fromLen = GetTagData(CODESETSA_SourceLen, UINT_MAX, attrs);

  if(from && fromLen)
  {
    struct convertMsg       msg;
    struct codeset *codeset;
    struct Hook    *hook;
    ULONG          destLen;
    int            i = 0;
    UBYTE                		buf[256];
    UBYTE          *src, *destPtr = NULL, *b = NULL, c;

    if(!(codeset = (struct codeset *)GetTagData(CODESETSA_Codeset, 0, attrs)))
      codeset = defaultCodeset(TRUE);

    hook    = (struct Hook *)GetTagData(CODESETSA_DestHook, 0, attrs);
    destLen = GetTagData(CODESETSA_DestLen,0,attrs);

    if(hook)
    {
      if(destLen<16 || destLen>sizeof(buf))
        destLen = sizeof(buf);

      msg.state = CODESETV_Translating;
      b = buf;
      i = 0;
    }
    else
    {
      if(!(dest = (UTF8*)GetTagData(CODESETSA_Dest, 0, attrs)) ||
        GetTagData(CODESETSA_AllocIfNeeded,TRUE,attrs))
      {
        ULONG len, flen;

        flen = fromLen;
        len  = 0;
        src  = from;

        while(((c = *src++) && (flen--)))
          len += codeset->table[c].utf8[0];

        if(!dest || (destLen<len+1))
        {
          APTR                   pool;
          struct SignalSemaphore *sem;

          if((pool = (APTR)GetTagData(CODESETSA_Pool, 0, attrs)))
          {
            if((sem = (struct SignalSemaphore *)GetTagData(CODESETSA_PoolSem, 0, attrs)))
            {
              ObtainSemaphore(sem);
            }

            dest = allocVecPooled(pool,len+1);

            if(sem)
              ReleaseSemaphore(sem);
          }
          else
            dest = allocArbitrateVecPooled(len+1);

          destLen  = len;
        }

        if(!dest)
          return NULL;
      }

      destPtr = (UBYTE*)dest;
    }

    for(src = from; fromLen && (c = *src); src++, fromLen--)
    {
      UTF8* utf8_seq;

      for(utf8_seq = &codeset->table[c].utf8[1]; (c = *utf8_seq); utf8_seq++)
      {
        if(!hook)
        {
          if(n>=destLen)
            break;
        }

        if(hook)
        {
          *b++ = c;
          i++;

          if(i%(destLen-1)==0)
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

    if(hook)
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

  if((destLenPtr = (ULONG *)GetTagData(CODESETSA_DestLenPtr, 0, attrs)))
    *destLenPtr = n;

  RETURN(dest);
  return dest;
}

LIBSTUB(CodesetsUTF8CreateA, UTF8*, REG(a0, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsUTF8CreateA((struct TagItem *)REG_A0);
  #else
  return CodesetsUTF8CreateA(attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsUTF8Create, UTF8*, ...)
{
  UTF8 *res;
  VA_LIST args;

  VA_START(args, self);
  res = CodesetsUTF8CreateA(VA_ARG(args, struct TagItem *));
  VA_END(args);

  return res;
}
#endif

///
/// CodesetsConvertStrA()
// Converts a given string from one source Codeset to a given destination
// codeset and returns the convert string
STRPTR LIBFUNC
CodesetsConvertStrA(REG(a0, struct TagItem *attrs))
{
  STRPTR srcStr;
  STRPTR dstStr = NULL;
  ULONG srcLen;
  ULONG dstLen = 0;

  ENTER();

  // get the ptr to the src string we want to convert
  // from the source codeset to the dest codeset.
  srcStr = (STRPTR)GetTagData(CODESETSA_Source, 0, attrs);
  srcLen = GetTagData(CODESETSA_SourceLen, UINT_MAX, attrs);

  if(srcStr != NULL && srcLen > 0)
  {
    struct codeset *srcCodeset;
    struct codeset *dstCodeset;

    // get the pointer to the codeset in which the src string is encoded
    if(!(srcCodeset = (struct codeset *)GetTagData(CODESETSA_SourceCodeset, 0, attrs)))
      srcCodeset = defaultCodeset(TRUE);

    // get the pointer to the codeset in which the dst string should be encoded
    if(!(dstCodeset = (struct codeset *)GetTagData(CODESETSA_DestCodeset, 0, attrs)))
      dstCodeset = defaultCodeset(TRUE);

    D(DBF_UTF, "srcCodeset: '%s' dstCodeset: '%s'", srcCodeset->name, dstCodeset->name);

    // check that the user didn't supplied the very same codeset
    // or otherwise a conversion is not required.
    if(srcCodeset != NULL && dstCodeset != NULL && srcCodeset != dstCodeset)
    {
      BOOL utf8Create = FALSE;
      BOOL strCreate = FALSE;
      UTF8 *utf8str;
      ULONG utf8strLen = 0;
      ULONG *destLenPtr;

      // if the source codeset is UTF-8 we don't have to use the UTF8Create()
      // function and can directly call the UTF8ToStr() function
      if(srcCodeset != CodesetsBase->utf8Codeset)
      {
        struct TagItem tags[] = { { CODESETSA_Codeset,     (ULONG)srcCodeset  },
                                  { CODESETSA_Source,      (ULONG)srcStr      },
                                  { CODESETSA_SourceLen,   srcLen             },
                                  { CODESETSA_DestLenPtr,  (ULONG)&utf8strLen },
                                  { TAG_DONE,              0                  } };

        utf8str = CodesetsUTF8CreateA((struct TagItem *)&tags[0]);

        utf8Create = TRUE;
      }
      else
      {
        utf8str = (UTF8 *)srcStr;
        utf8strLen = strlen(srcStr);
      }

      // in case the destination codeset is UTF-8 we don't have to actually
      // use the UTF8ToStr() function and can immediately return our
      // UTF8 string
      if(utf8str && utf8strLen > 0 && dstCodeset != CodesetsBase->utf8Codeset)
      {
        struct TagItem tags[] = { { CODESETSA_Codeset,     (ULONG)dstCodeset  },
                                  { CODESETSA_Source,      (ULONG)utf8str     },
                                  { CODESETSA_SourceLen,   utf8strLen         },
                                  { CODESETSA_DestLenPtr,  (ULONG)&dstLen     },
                                  { TAG_DONE,              0                  } };

        dstStr = CodesetsUTF8ToStrA((struct TagItem *)&tags[0]);

        strCreate = TRUE;
      }
      else
      {
        dstStr = (STRPTR)utf8str;
        dstLen = utf8strLen;
      }

      D(DBF_UTF, "srcStr: %lx srcLen: %ld dstStr: %lx dstLen: %ld utf8create: %ld strCreate: %ld", srcStr, srcLen,
                                                                                                   dstStr, dstLen,
                                                                                                   utf8Create,
                                                                                                   strCreate);

      // if everything was successfull we can go and finalize everything
      if(dstStr != NULL && utf8str != NULL)
      {
        // as the conversion was a two way pass we have to either free the
        // memory of the utf8 string or not
        if(utf8Create && strCreate)
          CodesetsFreeA(utf8str, NULL);

        // if the user wants to be informed abour the length
        // of our destination string we store the length now in the supplied ptr.
        if((destLenPtr = (ULONG *)GetTagData(CODESETSA_DestLenPtr, 0, attrs)))
          *destLenPtr = dstLen;

        D(DBF_UTF, "successfully converted string with len %ld", dstLen);
      }
      else
      {
        W(DBF_ALWAYS, "an error occurred while trying to convert a string");

        // free all memory in case the conversion didn't work out
        if(utf8Create && utf8str)
          CodesetsFreeA(utf8str, NULL);

        if(strCreate && dstStr)
          CodesetsFreeA(dstStr, NULL);

        dstStr = NULL;
      }
    }
  }

  RETURN(dstStr);
  return dstStr;
}

LIBSTUB(CodesetsConvertStrA, STRPTR, REG(a0, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsConvertStrA((struct TagItem *)REG_A0);
  #else
  return CodesetsConvertStrA(attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsConvertStr, STRPTR, ...)
{
  STRPTR res;
  VA_LIST args;

  VA_START(args, self);
  res = CodesetsConvertStrA(VA_ARG(args, struct TagItem *));
  VA_END(args);

  return res;
}
#endif

///
/// CodesetsFreeVecPooledA()
void LIBFUNC
CodesetsFreeVecPooledA(REG(a0, APTR pool),
                       REG(a1, APTR mem),
                       REG(a2, struct TagItem *attrs))
{
  ENTER();

  if(pool && mem)
  {
    struct SignalSemaphore *sem;

	  if((sem = (struct SignalSemaphore *)GetTagData(CODESETSA_PoolSem, 0, attrs)))
      ObtainSemaphore(sem);

    freeVecPooled(pool,mem);

    if(sem)
      ReleaseSemaphore(sem);
  }

  LEAVE();
}

LIBSTUB(CodesetsFreeVecPooledA, void, REG(a0, APTR pool),
                                      REG(a1, APTR mem),
                                      REG(a2, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsFreeVecPooledA((APTR)REG_A0,(APTR)REG_A1,(struct TagItem *)REG_A2);
  #else
  return CodesetsFreeVecPooledA(pool, mem, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsFreeVecPooled, void, REG(a0, APTR pool),
                                       REG(a1, APTR mem), ...)
{
  VA_LIST args;

  VA_START(args, mem);
  CodesetsFreeVecPooledA(pool, mem, VA_ARG(args, struct TagItem *));
  VA_END(args);
}
#endif
///
/// CodesetsListCreateA()
struct MinList *LIBFUNC
CodesetsListCreateA(REG(a0, struct TagItem *attrs))
{
  struct MinList *codesetsList = NULL;

  ENTER();

  ObtainSemaphore(&CodesetsBase->poolSem);

  // no matter what, we create a codesets list we will return to the user
  if((codesetsList = allocVecPooled(CodesetsBase->pool, sizeof(struct MinList))))
  {
    BOOL scanProgDir = TRUE;
    BOOL forceProgDirScan = FALSE;
    struct TagItem *tstate = attrs;
    struct TagItem *tag;

    // initialize the new private codeset list and put it into a separate list
    NewList((struct List *)codesetsList);

    // first we get the path of the directory from which we go
    // and scan for charset tables from
    while((tag = NextTagItem(&tstate)))
    {
      switch(tag->ti_Tag)
      {
        case CODESETSA_CodesetDir:
        {
          if(tag->ti_Data != 0)
          {
            scanProgDir = FALSE;
            codesetsScanDir(codesetsList, (STRPTR)tag->ti_Data);
          }
          else
            forceProgDirScan = TRUE;
        }
        break;

        case CODESETSA_CodesetFile:
        {
          codesetsReadTable(codesetsList, (STRPTR)tag->ti_Data);
        }
        break;

        case CODESETSA_Codeset:
        {
          struct codeset *cs = (struct codeset *)tag->ti_Data;

          AddTail((struct List *)codesetsList, (struct Node *)&cs->node);
        }
        break;
      }
    }

    // in case the user also wants us to scan PROGDIR:
    // we do so
    if(scanProgDir == TRUE || forceProgDirScan == TRUE)
      codesetsScanDir(codesetsList, "PROGDIR:Charsets");
  }

  ReleaseSemaphore(&CodesetsBase->poolSem);

  RETURN(codesetsList);
  return codesetsList;
}

LIBSTUB(CodesetsListCreateA, struct MinList *, REG(a0, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsListCreateA((struct TagItem *)REG_A0);
  #else
  return CodesetsListCreateA(attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsListCreate, struct MinList *, ...)
{
  struct MinList *res;
  VA_LIST args;

  VA_START(args, self);
  res = CodesetsListCreateA(VA_ARG(args, struct TagItem *));
  VA_END(args);

  return res;
}
#endif

///
/// CodesetsListDelete()
void LIBFUNC
CodesetsListDelete(REG(a0, struct MinList *codesetsList))
{
  ENTER();

  if(codesetsList != NULL)
  {
    // cleanup the list
    codesetsCleanup(codesetsList);

    // then free the list itself
    freeArbitrateVecPooled(codesetsList);
  }

  LEAVE();
}

LIBSTUB(CodesetsListDelete, void, REG(a0, struct MinList *codesetsList))
{
  #ifdef __MORPHOS__
  return CodesetsListDelete((struct MinList *)REG_A0);
  #else
  return CodesetsListDelete(codesetsList);
  #endif
}
///
/// CodesetsListAddA()
void LIBFUNC
CodesetsListAddA(REG(a0, struct MinList *codesetsList),
                 REG(a1, struct TagItem *attrs))
{
  ENTER();

  ObtainSemaphore(&CodesetsBase->poolSem);

  if(codesetsList != NULL)
  {
    struct TagItem *tstate = attrs;
    struct TagItem *tag;

    // now we iterate through or tagItems and see if the user
    // wants to scan a whole directory or just adds a file.
    while((tag = NextTagItem(&tstate)))
    {
      switch(tag->ti_Tag)
      {
        case CODESETSA_CodesetDir:
        {
          codesetsScanDir(codesetsList, (STRPTR)tag->ti_Data);
        }
        break;

        case CODESETSA_CodesetFile:
        {
          codesetsReadTable(codesetsList, (STRPTR)tag->ti_Data);
        }
        break;

        case CODESETSA_Codeset:
        {
          struct codeset *cs = (struct codeset *)tag->ti_Data;

          AddTail((struct List *)codesetsList, (struct Node *)&cs->node);
        }
        break;
      }
    }
  }

  ReleaseSemaphore(&CodesetsBase->poolSem);

  LEAVE();
}

LIBSTUB(CodesetsListAddA, void, REG(a0, struct MinList *codesetsList), REG(a1, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsListAddA((struct MinList *)REG_A0, (struct TagItem *)REG_A1);
  #else
  return CodesetsListAddA(codesetsList, attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsListAdd, void, struct MinList *list, ...)
{
  VA_LIST args;

  VA_START(args, list);
  CodesetsListAddA(list, VA_ARG(args, struct TagItem *));
  VA_END(args);
}
#endif

///
/// CodesetsListRemoveA()
void LIBFUNC
CodesetsListRemoveA(REG(a0, struct TagItem *attrs))
{
  ENTER();

  ObtainSemaphore(&CodesetsBase->poolSem);

  if(attrs != NULL)
  {
    BOOL freeCodesets;
    struct TagItem *tstate = attrs;
    struct TagItem *tag;

    // check if the caller wants us also to free the codesets
    freeCodesets = (BOOL)GetTagData(CODESETSA_FreeCodesets, TRUE, attrs);

    // now we iterate through or tagItems and see what the
    // user wants to remove from the list
    while((tag = NextTagItem(&tstate)))
    {
      switch(tag->ti_Tag)
      {
        case CODESETSA_Codeset:
        {
          struct codeset *cs = (struct codeset *)tag->ti_Data;

          if(cs)
          {
            struct MinNode *mstate = &cs->node;

            // before we actually remove the node from its list, we
            // have to make sure it isn't part of our internal codesets list
            while(mstate->mln_Succ)
              mstate = mstate->mln_Succ;

            if(mstate != CodesetsBase->codesets.mlh_Tail)
            {
              Remove((struct Node *)&cs->node);

              // free all codesets data if requested.
              if(freeCodesets == TRUE)
              {
                if(cs->name)             freeArbitrateVecPooled(cs->name);
                if(cs->alt_name)         freeArbitrateVecPooled(cs->alt_name);
                if(cs->characterization) freeArbitrateVecPooled(cs->characterization);

                freeArbitrateVecPooled(cs);
              }
            }
            else
              W(DBF_ALWAYS, "user tried to remove an internal codesets!");
          }
        }
        break;
      }
    }
  }

  ReleaseSemaphore(&CodesetsBase->poolSem);

  LEAVE();
}

LIBSTUB(CodesetsListRemoveA, void, REG(a0, struct TagItem *attrs))
{
  #ifdef __MORPHOS__
  return CodesetsListRemoveA((struct TagItem *)REG_A0);
  #else
  return CodesetsListRemoveA(attrs);
  #endif
}

#ifdef __amigaos4__
LIBSTUBVA(CodesetsListRemove, void, ...)
{
  VA_LIST args;

  VA_START(args, self);
  CodesetsListRemoveA(VA_ARG(args, struct TagItem *));
  VA_END(args);
}
#endif

///

/**************************************************************************/
