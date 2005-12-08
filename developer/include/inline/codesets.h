#ifndef _INLINE_CODESETS_H
#define _INLINE_CODESETS_H

#ifndef CLIB_CODESETS_PROTOS_H
#define CLIB_CODESETS_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#ifndef  LIBRARIES_CODESETS_H
#include <libraries/codesets.h>
#endif

#ifndef CODESETS_BASE_NAME
#define CODESETS_BASE_NAME CodesetsBase
#endif

#define CodesetsConvertUTF32toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x24, ULONG, CodesetsConvertUTF32toUTF16, const , sourceStart, a0, const , sourceEnd, a1, UTF16 **, targetStart, a2, UTF16 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsConvertUTF16toUTF32(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x2a, ULONG, CodesetsConvertUTF16toUTF32, const , sourceStart, a0, const , sourceEnd, a1, UTF32 **, targetStart, a2, UTF32 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsConvertUTF16toUTF8(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x30, ULONG, CodesetsConvertUTF16toUTF8, const , sourceStart, a0, const , sourceEnd, a1, UTF8 **, targetStart, a2, UTF8 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsIsLegalUTF8(source, length) \
	LP2(0x36, ULONG, CodesetsIsLegalUTF8, const , source, a0, ULONG, length, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsIsLegalUTF8Sequence(source, sourceEnd) \
	LP2(0x3c, ULONG, CodesetsIsLegalUTF8Sequence, const , source, a0, const , sourceEnd, a1, \
	, CODESETS_BASE_NAME)

#define CodesetsConvertUTF8toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x42, ULONG, CodesetsConvertUTF8toUTF16, const , sourceStart, a0, const , sourceEnd, a1, UTF16 **, targetStart, a2, UTF16 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsConvertUTF32toUTF8(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x48, ULONG, CodesetsConvertUTF32toUTF8, const , sourceStart, a0, const , sourceEnd, a1, UTF8 **, targetStart, a2, UTF8 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsConvertUTF8toUTF32(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x4e, ULONG, CodesetsConvertUTF8toUTF32, const , sourceStart, a0, const , sourceEnd, a1, UTF32 **, targetStart, a2, UTF32 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME)

#define CodesetsSetDefaultA(name, attrs) \
	LP2(0x54, struct codeset *, CodesetsSetDefaultA, STRPTR, name, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsSetDefault(name, tags...) \
	({ULONG _tags[] = {tags}; CodesetsSetDefaultA((name), (struct TagItem *) _tags);})
#endif

#define CodesetsFreeA(obj, attrs) \
	LP2NR(0x5a, CodesetsFreeA, APTR, obj, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsFree(obj, tags...) \
	({ULONG _tags[] = {tags}; CodesetsFreeA((obj), (struct TagItem *) _tags);})
#endif

#define CodesetsSupportedA(attrs) \
	LP1(0x60, STRPTR *, CodesetsSupportedA, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsSupported(tags...) \
	({ULONG _tags[] = {tags}; CodesetsSupportedA((struct TagItem *) _tags);})
#endif

#define CodesetsFindA(name, attrs) \
	LP2(0x66, struct codeset *, CodesetsFindA, STRPTR, name, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsFind(name, tags...) \
	({ULONG _tags[] = {tags}; CodesetsFindA((name), (struct TagItem *) _tags);})
#endif

#define CodesetsFindBestA(text, text_len, error_ptr, attrs) \
	LP4(0x6c, struct codeset *, CodesetsFindBestA, STRPTR, text, a0, ULONG, text_len, d0, ULONG *, error_ptr, a1, struct TagItem *, attrs, a2, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsFindBest(text, text_len, error_ptr, tags...) \
	({ULONG _tags[] = {tags}; CodesetsFindBestA((text), (text_len), (error_ptr), (struct TagItem *) _tags);})
#endif

#define CodesetsUTF8Len(str) \
	LP1(0x72, ULONG, CodesetsUTF8Len, const , str, a0, \
	, CODESETS_BASE_NAME)

#define CodesetsUTF8ToStrA(attrs) \
	LP1(0x78, STRPTR, CodesetsUTF8ToStrA, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsUTF8ToStr(tags...) \
	({ULONG _tags[] = {tags}; CodesetsUTF8ToStrA((struct TagItem *) _tags);})
#endif

#define CodesetsUTF8CreateA(attrs) \
	LP1(0x7e, UTF8 *, CodesetsUTF8CreateA, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsUTF8Create(tags...) \
	({ULONG _tags[] = {tags}; CodesetsUTF8CreateA((struct TagItem *) _tags);})
#endif

#define CodesetsEncodeB64A(attrs) \
	LP1(0x84, ULONG, CodesetsEncodeB64A, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsEncodeB64(tags...) \
	({ULONG _tags[] = {tags}; CodesetsEncodeB64A((struct TagItem *) _tags);})
#endif

#define CodesetsDecodeB64A(attrs) \
	LP1(0x8a, ULONG, CodesetsDecodeB64A, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsDecodeB64(tags...) \
	({ULONG _tags[] = {tags}; CodesetsDecodeB64A((struct TagItem *) _tags);})
#endif

#define CodesetsStrLenA(str, attrs) \
	LP2(0x90, ULONG, CodesetsStrLenA, STRPTR, str, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CodesetsStrLen(str, tags...) \
	({ULONG _tags[] = {tags}; CodesetsStrLenA((str), (struct TagItem *) _tags);})
#endif

#define CodesetsIsValidUTF8(str) \
	LP1(0x96, ULONG, CodesetsIsValidUTF8, STRPTR, str, a0, \
	, CODESETS_BASE_NAME)

#endif /*  _INLINE_CODESETS_H  */
