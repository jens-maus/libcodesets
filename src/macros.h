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

#ifndef _MACROS_H
#define _MACROS_H

/****************************************************************************/
/*
** Some useful macros
*/

#include "SDI_compiler.h"

#ifdef __MORPHOS__
#define copymem(t,f,l) CopyMem((APTR)(f),(APTR)(t),(ULONG)(l))
#else
#define copymem(t,f,l) memcpy((APTR)(t),(APTR)(f),(ULONG)(l))
#endif

#undef NODE
#define NODE(a) ((struct Node *)(a))

#undef MINNODE
#define MINNODE(a) ((struct MinNode *)(a))

#undef LIST
#define LIST(a) ((struct List *)(a))

#undef MINLIST
#define MINLIST(a) ((struct MinList *)(a))

#undef MESSAGE
#define MESSAGE(m) ((struct Message *)(m))

#undef NEWLIST
#define NEWLIST(l) (LIST(l)->lh_Head = NODE(&LIST(l)->lh_Tail), \
                    LIST(l)->lh_Tail = NULL, \
                    LIST(l)->lh_TailPred = NODE(&LIST(l)->lh_Head))

#undef QUICKNEWLIST
#define QUICKNEWLIST(l) (LIST(l)->lh_Head = NODE(&LIST(l)->lh_Tail), \
                         LIST(l)->lh_TailPred = NODE(&LIST(l)->lh_Head))

#undef ADDTAIL
#define ADDTAIL(l,n) AddTail(LIST(l),NODE(n))

#undef PORT
#define PORT(m) ((struct MsgPort *)(m))

#undef INITPORT
#define INITPORT(p,s) (PORT(p)->mp_Flags = PA_SIGNAL, \
                       PORT(p)->mp_SigBit = (UBYTE)(s), \
                       PORT(p)->mp_SigTask = FindTask(NULL), \
                       NEWLIST(&((p)->mp_MsgList)))

#undef QUICKINITPORT
#define QUICKINITPORT(p,s,t) (PORT(p)->mp_Flags = PA_SIGNAL, \
                              PORT(p)->mp_SigBit = (UBYTE)(s), \
                              PORT(p)->mp_SigTask = ((struct Task *)(t)), \
                              QUICKNEWLIST(&(PORT(p)->mp_MsgList)))

#undef INITMESSAGE
#define INITMESSAGE(m,p,l) (MESSAGE(m)->mn_Node.ln_Type = NT_MESSAGE, \
                            MESSAGE(m)->mn_ReplyPort = PORT(p), \
                            MESSAGE(m)->mn_Length = ((UWORD)(l)))

#undef BOOLSAME
#define BOOLSAME(a,b) (((BOOL)(a) && (BOOL)(b)) || (!(BOOL)(a) && !(BOOL)(b)))

#if defined(__amigaos4__)
#define GETINTERFACE(iface, base)	(iface = (APTR)GetInterface((struct Library *)(base), "main", 1L, NULL))
#define DROPINTERFACE(iface)			(DropInterface((struct Interface *)iface), iface = NULL)
#else
#define GETINTERFACE(iface, base)	TRUE
#define DROPINTERFACE(iface)
#endif

/****************************************************************************/

#endif /* _MACROS_H */
