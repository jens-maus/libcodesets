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

/****************************************************************************/

APTR
allocVecPooled(APTR pool,ULONG size)
{
  ULONG *mem;

  if((mem = AllocPooled(pool,size += sizeof(ULONG))))
    *mem++ = size;

  return mem;
}

/****************************************************************************/

void
freeVecPooled(APTR pool,APTR mem)
{
  FreePooled(pool,(LONG *)mem - 1,*((LONG *)mem - 1));
}

/****************************************************************************/

APTR
allocArbitratePooled(ULONG s)
{
  APTR mem;

  ObtainSemaphore(&CodesetsBase->poolSem);
  mem = AllocPooled(CodesetsBase->pool, s);
  ReleaseSemaphore(&CodesetsBase->poolSem);

  return mem;
}

/****************************************************************************/

void
freeArbitratePooled(APTR mem,ULONG s)
{
  ObtainSemaphore(&CodesetsBase->poolSem);
  FreePooled(CodesetsBase->pool, mem, s);
  ReleaseSemaphore(&CodesetsBase->poolSem);
}

/****************************************************************************/

APTR
allocArbitrateVecPooled(ULONG size)
{
  ULONG *mem;

  ObtainSemaphore(&CodesetsBase->poolSem);
  mem = allocVecPooled(CodesetsBase->pool, size);
  ReleaseSemaphore(&CodesetsBase->poolSem);

  return mem;
}

/****************************************************************************/

void
freeArbitrateVecPooled(APTR mem)
{
  ObtainSemaphore(&CodesetsBase->poolSem);
  freeVecPooled(CodesetsBase->pool, mem);
  ReleaseSemaphore(&CodesetsBase->poolSem);
}

/****************************************************************************/

#ifdef __MORPHOS__
void
sprintf(STRPTR buf,STRPTR fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}
#elif !defined(__amigaos4__)
static UWORD sprintfStuff[] = {0x16c0, 0x4e75};

void STDARGS
sprintf(STRPTR to,STRPTR fmt,...)
{
  RawDoFmt(fmt,&fmt+1,(APTR)sprintfStuff,to);
}
#endif

/****************************************************************************/

struct stream
{
    STRPTR buf;
    int    size;
    int    counter;
    int    stop;
};

#ifdef __MORPHOS__
static void
snprintfStuff(void)
{
    struct stream *s = (struct stream *)REG_A3;
    UBYTE         c  = (UBYTE)REG_D0;

    if (!s->stop)
    {
        if (++s->counter>=s->size)
        {
            *(s->buf) = 0;
            s->stop   = 1;
        }
        else *(s->buf++) = c;
    }
}

static struct EmulLibEntry snprintfStuffTrap = {TRAP_LIB,0,(void *)&snprintfStuff};

int STDARGS
snprintf(STRPTR buf,int size,STRPTR fmt,...)
{
    struct stream s;
    va_list       va;

    va_start(va,fmt);

    s.buf     = buf;
    s.size    = size;
    s.counter = 0;
    s.stop    = 0;

    RawDoFmt(fmt,va->overflow_arg_area,(APTR)&snprintfStuffTrap,&s);

    va_end(va);

    return s.counter-1;
}
#elif !defined(__amigaos4__)
static void ASM
snprintfStuff(REG(d0, UBYTE c), REG(a3, struct stream *s))
{
    if (!s->stop)
    {
        if (++s->counter>=s->size)
        {
            *(s->buf) = 0;
            s->stop   = 1;
        }
        else *(s->buf++) = c;
    }
}

int STDARGS
snprintf(STRPTR buf,int size,STRPTR fmt,...)
{
    struct stream s;

    s.buf     = buf;
    s.size    = size;
    s.counter = 0;
    s.stop    = 0;

    RawDoFmt(fmt,&fmt+1,(APTR)snprintfStuff,&s);

    return s.counter-1;
}
#endif
