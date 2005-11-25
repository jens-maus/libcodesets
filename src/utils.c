/*
**
** Copyright 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
** Released under the terms of the LGPL II.
**
**/

#include "lib.h"

/****************************************************************************/

APTR
allocVecPooled(APTR pool,ULONG size)
{
    register ULONG *mem;

    if (mem = AllocPooled(pool,size += sizeof(ULONG)))
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
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = AllocPooled(lib_pool, s);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/****************************************************************************/

void
freeArbitratePooled(APTR mem,ULONG s)
{
    ObtainSemaphore(&lib_poolSem);
    FreePooled(lib_pool,mem,s);
    ReleaseSemaphore(&lib_poolSem);
}

/****************************************************************************/

APTR
allocArbitrateVecPooled(ULONG size)
{
    register ULONG *mem;

    ObtainSemaphore(&lib_poolSem);
    mem = allocVecPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/****************************************************************************/

void
freeArbitrateVecPooled(APTR mem)
{
    ObtainSemaphore(&lib_poolSem);
    freeVecPooled(lib_pool,mem);
    ReleaseSemaphore(&lib_poolSem);
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
#else
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
    register struct stream *s = (struct stream *)REG_A3;
    register UBYTE         c  = (UBYTE)REG_D0;

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
#else
static void ASM
snprintfStuff(REG(d0) UBYTE c,REG(a3) struct stream *s)
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

/****************************************************************************/

int
countNodes(struct MinList *List)
{
    register struct MinNode *node, *succ;
    register int            num;

    for (node = List->mlh_Head, num = 0; succ = node->mln_Succ; node = succ)
        ++num;

    return num;
}

/***********************************************************************/
