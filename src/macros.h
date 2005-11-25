#ifndef _MACROS_H
#define _MACROS_H

/****************************************************************************/
/*
** Some useful macros
*/

#ifdef __MORPHOS__
#define ASM
#define STDARGS
#define REGARGS
#define REG(x)
#define LIBCALL
#define copymem(t,f,l) CopyMem((APTR)(f),(APTR)(t),(ULONG)(l))
#else
#define ASM __asm
#define STDARGS __stdargs
#define REGARGS __regargs
#define REG(x) register __ ## x
#define LIBCALL SAVEDS ASM
#define copymem(t,f,l) memcpy((APTR)(t),(APTR)(f),(ULONG)(l))
#endif

#define SAVEDS __saveds
#define INLINE __inline

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

/****************************************************************************/

#endif /* _MACROS_H */
