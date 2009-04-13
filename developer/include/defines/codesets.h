#ifndef DEFINES_CODESETS_H
#define DEFINES_CODESETS_H

/*
    *** Automatically generated from '/home/aros/Build/20090324/AROS/workbench/libs/codesetslib/src/codesets.conf'. Edits will be lost. ***
    Copyright © 1995-2009, The AROS Development Team. All rights reserved.
*/

/*
    Desc: Defines for codesets
*/

#include <aros/libcall.h>
#include <exec/types.h>
#include <aros/preprocessor/variadic/cast2iptr.hpp>


#define __CodesetsConvertUTF32toUTF16_WB(__CodesetsBase, __arg1, __arg2, __arg3, __arg4, __arg5) \
        AROS_LC5(ULONG, CodesetsConvertUTF32toUTF16, \
                  AROS_LCA(const UTF32 **,(__arg1),A0), \
                  AROS_LCA(const UTF32 *,(__arg2),A1), \
                  AROS_LCA(UTF16 **,(__arg3),A2), \
                  AROS_LCA(UTF16 *,(__arg4),A3), \
                  AROS_LCA(ULONG,(__arg5),D0), \
        struct Library *, (__CodesetsBase), 6, Codesets)

#define CodesetsConvertUTF32toUTF16(arg1, arg2, arg3, arg4, arg5) \
    __CodesetsConvertUTF32toUTF16_WB(CodesetsBase, (arg1), (arg2), (arg3), (arg4), (arg5))

#define __CodesetsConvertUTF16toUTF32_WB(__CodesetsBase, __arg1, __arg2, __arg3, __arg4, __arg5) \
        AROS_LC5(ULONG, CodesetsConvertUTF16toUTF32, \
                  AROS_LCA(const  UTF16 **,(__arg1),A0), \
                  AROS_LCA(const UTF16 *,(__arg2),A1), \
                  AROS_LCA(UTF32 **,(__arg3),A2), \
                  AROS_LCA(UTF32 *,(__arg4),A3), \
                  AROS_LCA(ULONG,(__arg5),D0), \
        struct Library *, (__CodesetsBase), 7, Codesets)

#define CodesetsConvertUTF16toUTF32(arg1, arg2, arg3, arg4, arg5) \
    __CodesetsConvertUTF16toUTF32_WB(CodesetsBase, (arg1), (arg2), (arg3), (arg4), (arg5))

#define __CodesetsConvertUTF16toUTF8_WB(__CodesetsBase, __arg1, __arg2, __arg3, __arg4, __arg5) \
        AROS_LC5(ULONG, CodesetsConvertUTF16toUTF8, \
                  AROS_LCA(const UTF16 **,(__arg1),A0), \
                  AROS_LCA(const UTF16 *,(__arg2),A1), \
                  AROS_LCA(UTF8 **,(__arg3),A2), \
                  AROS_LCA(UTF8 *,(__arg4),A3), \
                  AROS_LCA(ULONG,(__arg5),D0), \
        struct Library *, (__CodesetsBase), 8, Codesets)

#define CodesetsConvertUTF16toUTF8(arg1, arg2, arg3, arg4, arg5) \
    __CodesetsConvertUTF16toUTF8_WB(CodesetsBase, (arg1), (arg2), (arg3), (arg4), (arg5))

#define __CodesetsIsLegalUTF8_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2(BOOL, CodesetsIsLegalUTF8, \
                  AROS_LCA(const UTF8 *,(__arg1),A0), \
                  AROS_LCA(ULONG,(__arg2),D0), \
        struct Library *, (__CodesetsBase), 9, Codesets)

#define CodesetsIsLegalUTF8(arg1, arg2) \
    __CodesetsIsLegalUTF8_WB(CodesetsBase, (arg1), (arg2))

#define __CodesetsIsLegalUTF8Sequence_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2(BOOL, CodesetsIsLegalUTF8Sequence, \
                  AROS_LCA(const UTF8 *,(__arg1),A0), \
                  AROS_LCA(const UTF8 *,(__arg2),D1), \
        struct Library *, (__CodesetsBase), 10, Codesets)

#define CodesetsIsLegalUTF8Sequence(arg1, arg2) \
    __CodesetsIsLegalUTF8Sequence_WB(CodesetsBase, (arg1), (arg2))

#define __CodesetsConvertUTF8toUTF16_WB(__CodesetsBase, __arg1, __arg2, __arg3, __arg4, __arg5) \
        AROS_LC5(ULONG, CodesetsConvertUTF8toUTF16, \
                  AROS_LCA(const UTF8 **,(__arg1),A0), \
                  AROS_LCA(const UTF8 *,(__arg2),A1), \
                  AROS_LCA(UTF16 **,(__arg3),A2), \
                  AROS_LCA(UTF16 *,(__arg4),A3), \
                  AROS_LCA(ULONG,(__arg5),D0), \
        struct Library *, (__CodesetsBase), 11, Codesets)

#define CodesetsConvertUTF8toUTF16(arg1, arg2, arg3, arg4, arg5) \
    __CodesetsConvertUTF8toUTF16_WB(CodesetsBase, (arg1), (arg2), (arg3), (arg4), (arg5))

#define __CodesetsConvertUTF32toUTF8_WB(__CodesetsBase, __arg1, __arg2, __arg3, __arg4, __arg5) \
        AROS_LC5(ULONG, CodesetsConvertUTF32toUTF8, \
                  AROS_LCA(const UTF32 **,(__arg1),A0), \
                  AROS_LCA(const UTF32 *,(__arg2),A1), \
                  AROS_LCA(UTF8 **,(__arg3),A2), \
                  AROS_LCA(UTF8 *,(__arg4),A3), \
                  AROS_LCA(ULONG,(__arg5),D0), \
        struct Library *, (__CodesetsBase), 12, Codesets)

#define CodesetsConvertUTF32toUTF8(arg1, arg2, arg3, arg4, arg5) \
    __CodesetsConvertUTF32toUTF8_WB(CodesetsBase, (arg1), (arg2), (arg3), (arg4), (arg5))

#define __CodesetsConvertUTF8toUTF32_WB(__CodesetsBase, __arg1, __arg2, __arg3, __arg4, __arg5) \
        AROS_LC5(ULONG, CodesetsConvertUTF8toUTF32, \
                  AROS_LCA(const UTF8 **,(__arg1),A0), \
                  AROS_LCA(const UTF8 *,(__arg2),A1), \
                  AROS_LCA(UTF32 **,(__arg3),A2), \
                  AROS_LCA(UTF32 *,(__arg4),A3), \
                  AROS_LCA(ULONG,(__arg5),D0), \
        struct Library *, (__CodesetsBase), 13, Codesets)

#define CodesetsConvertUTF8toUTF32(arg1, arg2, arg3, arg4, arg5) \
    __CodesetsConvertUTF8toUTF32_WB(CodesetsBase, (arg1), (arg2), (arg3), (arg4), (arg5))

#define __CodesetsSetDefaultA_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2(struct codeset *, CodesetsSetDefaultA, \
                  AROS_LCA(STRPTR,(__arg1),A0), \
                  AROS_LCA(struct TagItem *,(__arg2),A1), \
        struct Library *, (__CodesetsBase), 14, Codesets)

#define CodesetsSetDefaultA(arg1, arg2) \
    __CodesetsSetDefaultA_WB(CodesetsBase, (arg1), (arg2))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsSetDefault(arg1, ...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsSetDefaultA((arg1), (struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsFreeA_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2NR(void, CodesetsFreeA, \
                  AROS_LCA(APTR,(__arg1),A0), \
                  AROS_LCA(struct TagItem *,(__arg2),A1), \
        struct Library *, (__CodesetsBase), 15, Codesets)

#define CodesetsFreeA(arg1, arg2) \
    __CodesetsFreeA_WB(CodesetsBase, (arg1), (arg2))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsFree(arg1, ...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsFreeA((arg1), (struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsSupportedA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(STRPTR *, CodesetsSupportedA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 16, Codesets)

#define CodesetsSupportedA(arg1) \
    __CodesetsSupportedA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsSupported(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsSupportedA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsFindA_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2(struct codeset *, CodesetsFindA, \
                  AROS_LCA(STRPTR,(__arg1),A0), \
                  AROS_LCA(struct TagItem *,(__arg2),A1), \
        struct Library *, (__CodesetsBase), 17, Codesets)

#define CodesetsFindA(arg1, arg2) \
    __CodesetsFindA_WB(CodesetsBase, (arg1), (arg2))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsFind(arg1, ...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsFindA((arg1), (struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsFindBestA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(struct codeset *, CodesetsFindBestA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 18, Codesets)

#define CodesetsFindBestA(arg1) \
    __CodesetsFindBestA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsFindBest(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsFindBestA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsUTF8Len_WB(__CodesetsBase, __arg1) \
        AROS_LC1(ULONG, CodesetsUTF8Len, \
                  AROS_LCA(const UTF8 *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 19, Codesets)

#define CodesetsUTF8Len(arg1) \
    __CodesetsUTF8Len_WB(CodesetsBase, (arg1))

#define __CodesetsUTF8ToStrA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(STRPTR, CodesetsUTF8ToStrA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 20, Codesets)

#define CodesetsUTF8ToStrA(arg1) \
    __CodesetsUTF8ToStrA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsUTF8ToStr(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsUTF8ToStrA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsUTF8CreateA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(UTF8 *, CodesetsUTF8CreateA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 21, Codesets)

#define CodesetsUTF8CreateA(arg1) \
    __CodesetsUTF8CreateA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsUTF8Create(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsUTF8CreateA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsEncodeB64A_WB(__CodesetsBase, __arg1) \
        AROS_LC1(ULONG, CodesetsEncodeB64A, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 22, Codesets)

#define CodesetsEncodeB64A(arg1) \
    __CodesetsEncodeB64A_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsEncodeB64(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsEncodeB64A((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsDecodeB64A_WB(__CodesetsBase, __arg1) \
        AROS_LC1(ULONG, CodesetsDecodeB64A, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 23, Codesets)

#define CodesetsDecodeB64A(arg1) \
    __CodesetsDecodeB64A_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsDecodeB64(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsDecodeB64A((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsStrLenA_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2(ULONG, CodesetsStrLenA, \
                  AROS_LCA(STRPTR,(__arg1),A0), \
                  AROS_LCA(struct TagItem *,(__arg2),A1), \
        struct Library *, (__CodesetsBase), 24, Codesets)

#define CodesetsStrLenA(arg1, arg2) \
    __CodesetsStrLenA_WB(CodesetsBase, (arg1), (arg2))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsStrLen(arg1, ...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsStrLenA((arg1), (struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsIsValidUTF8_WB(__CodesetsBase, __arg1) \
        AROS_LC1(BOOL, CodesetsIsValidUTF8, \
                  AROS_LCA(STRPTR,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 25, Codesets)

#define CodesetsIsValidUTF8(arg1) \
    __CodesetsIsValidUTF8_WB(CodesetsBase, (arg1))

#define __CodesetsFreeVecPooledA_WB(__CodesetsBase, __arg1, __arg2, __arg3) \
        AROS_LC3NR(void, CodesetsFreeVecPooledA, \
                  AROS_LCA(APTR,(__arg1),A0), \
                  AROS_LCA(APTR,(__arg2),A1), \
                  AROS_LCA(struct TagItem *,(__arg3),A2), \
        struct Library *, (__CodesetsBase), 26, Codesets)

#define CodesetsFreeVecPooledA(arg1, arg2, arg3) \
    __CodesetsFreeVecPooledA_WB(CodesetsBase, (arg1), (arg2), (arg3))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsFreeVecPooled(arg1, arg2, ...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsFreeVecPooledA((arg1), (arg2), (struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsConvertStrA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(STRPTR, CodesetsConvertStrA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 27, Codesets)

#define CodesetsConvertStrA(arg1) \
    __CodesetsConvertStrA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsConvertStr(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsConvertStrA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsListCreateA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(struct codesetList *, CodesetsListCreateA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 28, Codesets)

#define CodesetsListCreateA(arg1) \
    __CodesetsListCreateA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsListCreate(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsListCreateA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsListDeleteA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(BOOL, CodesetsListDeleteA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 29, Codesets)

#define CodesetsListDeleteA(arg1) \
    __CodesetsListDeleteA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsListDelete(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsListDeleteA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsListAddA_WB(__CodesetsBase, __arg1, __arg2) \
        AROS_LC2(BOOL, CodesetsListAddA, \
                  AROS_LCA(struct codesetList *,(__arg1),A0), \
                  AROS_LCA(struct TagItem *,(__arg2),A1), \
        struct Library *, (__CodesetsBase), 30, Codesets)

#define CodesetsListAddA(arg1, arg2) \
    __CodesetsListAddA_WB(CodesetsBase, (arg1), (arg2))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsListAdd(arg1, ...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsListAddA((arg1), (struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#define __CodesetsListRemoveA_WB(__CodesetsBase, __arg1) \
        AROS_LC1(BOOL, CodesetsListRemoveA, \
                  AROS_LCA(struct TagItem *,(__arg1),A0), \
        struct Library *, (__CodesetsBase), 31, Codesets)

#define CodesetsListRemoveA(arg1) \
    __CodesetsListRemoveA_WB(CodesetsBase, (arg1))

#if !defined(NO_INLINE_STDARG) && !defined(CODESETS_NO_INLINE_STDARG)
#define CodesetsListRemove(...) \
({ \
    IPTR __args[] = { AROS_PP_VARIADIC_CAST2IPTR(__VA_ARGS__) }; \
    CodesetsListRemoveA((struct TagItem *)__args); \
})
#endif /* !NO_INLINE_STDARG */

#endif /* DEFINES_CODESETS_H*/
