/*
**
** Copyright 2001-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
** Released under the terms of the LGPL II.
**
**/

extern UBYTE                    lib_name[];
extern UBYTE	                 lib_ver[];
extern UBYTE                    lib_fullName[];
extern ULONG                    lib_version;
extern ULONG                    lib_revision;

extern struct ExecBase          *SysBase;
extern struct RxsLib            *RexxSysBase;
extern struct DosLibrary        *DOSBase;
extern struct Library           *UtilityBase;
extern struct LocaleBase        *LocaleBase;

extern struct Library           *lib_base;
extern ULONG                    lib_segList;
extern struct SignalSemaphore   lib_sem;
extern struct SignalSemaphore   lib_poolSem;
extern APTR                     lib_pool;
extern struct MinList           lib_codesets;
extern struct codeset           *lib_systemCodeset;
extern ULONG                    lib_flags;

/***************************************************************************/

enum
{
    BASEFLG_Init = 1<<0,
};

/***************************************************************************/
