#ifndef _PPCINLINE_CODESETS_H
#define _PPCINLINE_CODESETS_H

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif

#ifndef CODESETS_BASE_NAME
#define CODESETS_BASE_NAME CodesetsBase
#endif

#define CodesetsConvertUTF32toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x24, ULONG, CodesetsConvertUTF32toUTF16, const UTF32 **, sourceStart, a0, const UTF32 *, sourceEnd, a1, UTF16 **, targetStart, a2, UTF16 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsConvertUTF16toUTF32(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x2a, ULONG, CodesetsConvertUTF16toUTF32, const UTF16 **, sourceStart, a0, const UTF16 *, sourceEnd, a1, UTF32 **, targetStart, a2, UTF32 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsConvertUTF16toUTF8(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x30, ULONG, CodesetsConvertUTF16toUTF8, const UTF16 **, sourceStart, a0, const UTF16 *, sourceEnd, a1, UTF8 **, targetStart, a2, UTF8 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsIsLegalUTF8(source, length) \
	LP2(0x36, ULONG, CodesetsIsLegalUTF8, const UTF8 *, source, a0, ULONG, length, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsIsLegalUTF8Sequence(source, sourceEnd) \
	LP2(0x3c, ULONG, CodesetsIsLegalUTF8Sequence, const UTF8 *, source, a0, const UTF8 *, sourceEnd, a1, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsConvertUTF8toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x42, ULONG, CodesetsConvertUTF8toUTF16, const UTF8 **, sourceStart, a0, const UTF8 *, sourceEnd, a1, UTF16 **, targetStart, a2, UTF16 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsConvertUTF32toUTF8(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x48, ULONG, CodesetsConvertUTF32toUTF8, const UTF32 **, sourceStart, a0, const UTF32 *, sourceEnd, a1, UTF8 **, targetStart, a2, UTF8 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsConvertUTF8toUTF32(sourceStart, sourceEnd, targetStart, targetEnd, flags) \
	LP5(0x4e, ULONG, CodesetsConvertUTF8toUTF32, const UTF8 **, sourceStart, a0, const UTF8 *, sourceEnd, a1, UTF32 **, targetStart, a2, UTF32 *, targetEnd, a3, ULONG, flags, d0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsSetDefaultA(name, attrs) \
	LP2(0x54, struct codeset *, CodesetsSetDefaultA, STRPTR, name, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsFreeA(obj, attrs) \
	LP2NR(0x5a, CodesetsFreeA, APTR, obj, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsSupportedA(attrs) \
	LP1(0x60, STRPTR *, CodesetsSupportedA, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsFindA(name, attrs) \
	LP2(0x66, struct codeset *, CodesetsFindA, STRPTR, name, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsFindBestA(text, text_len, error_ptr, attrs) \
	LP4(0x6c, struct codeset *, CodesetsFindBestA, STRPTR, text, a0, ULONG, text_len, d0, ULONG *, error_ptr, a1, struct TagItem *, attrs, a2, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsUTF8Len(str) \
	LP1(0x72, ULONG, CodesetsUTF8Len, const UTF8 *, str, a0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsUTF8ToStrA(attrs) \
	LP1(0x78, STRPTR, CodesetsUTF8ToStrA, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsUTF8CreateA(attrs) \
	LP1(0x7e, UTF8 *, CodesetsUTF8CreateA, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsEncodeB64A(attrs) \
	LP1(0x84, ULONG, CodesetsEncodeB64A, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsDecodeB64A(attrs) \
	LP1(0x8a, ULONG, CodesetsDecodeB64A, struct TagItem *, attrs, a0, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define CodesetsStrLenA(str, attrs) \
	LP2(0x90, ULONG, CodesetsStrLenA, STRPTR, str, a0, struct TagItem *, attrs, a1, \
	, CODESETS_BASE_NAME, 0, 0, 0, 0, 0, 0)

#ifdef USE_INLINE_STDARG

#include <stdarg.h>

#define CodesetsFree(obj, ...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsFreeA((obj), (struct TagItem *) _tags);})

#define CodesetsSetDefault(name, ...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsSetDefaultA((name), (struct TagItem *) _tags);})

#define CodesetsSupported(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsSupportedA((struct TagItem *) _tags);})

#define CodesetsFind(name, ...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsFindA((name), (struct TagItem *) _tags);})

#define CodesetsFindBest(text, text_len, error_ptr, ...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsFindBestA((text), (text_len), (error_ptr), (struct TagItem *) _tags);})

#define CodesetsUTF8ToStr(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsUTF8ToStrA((struct TagItem *) _tags);})

#define CodesetsUTF8Create(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsUTF8CreateA((struct TagItem *) _tags);})

#define CodesetsEncodeB64(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsEncodeB64A((struct TagItem *) _tags);})

#define CodesetsDecodeB64(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsDecodeB64A((struct TagItem *) _tags);})

#define CodesetsStrLen(str, ...) \
	({ULONG _tags[] = {__VA_ARGS__}; CodesetsStrLenA((str), (struct TagItem *) _tags);})

#endif /* USE_INLINE_STDARG */

#endif /*  _PPCINLINE_CODESETS_H  */
