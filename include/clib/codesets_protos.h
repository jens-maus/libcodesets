#ifndef CLIB_CODESETS_PROTOS_H
#define CLIB_CODESETS_PROTOS_H

/*
**  $VER: codesets_protos.h 5.3 (5.5.2005)
**  Includes Release 5.3
**
**  C prototypes. For use with 32 bit integers only.
**
**  Written by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
**  Released under the terms of the
**  LGPL version 2
*/

#ifndef LIBRARIES_CODESETS_H
#include <libraries/codesets.h>
#endif

ULONG CodesetsConvertUTF32toUTF16 ( const UTF32 **sourceStart , const UTF32 *sourceEnd , UTF16 **targetStart , UTF16 *targetEnd , ULONG flags );
ULONG CodesetsConvertUTF16toUTF32 ( const UTF16 **sourceStart , const UTF16 *sourceEnd , UTF32 **targetStart , UTF32 *targetEnd , ULONG flags );
ULONG CodesetsConvertUTF16toUTF8 ( const UTF16 **sourceStart , const UTF16 *sourceEnd , UTF8 **targetStart , UTF8 *targetEnd , ULONG flags );
ULONG CodesetsIsLegalUTF8 ( const UTF8 *source , ULONG length );
ULONG CodesetsIsLegalUTF8Sequence ( const UTF8 *source , const UTF8 *sourceEnd );
ULONG CodesetsConvertUTF8toUTF16 ( const UTF8 **sourceStart , const UTF8 *sourceEnd , UTF16 **targetStart , UTF16 *targetEnd , ULONG flags );
ULONG CodesetsConvertUTF32toUTF8 ( const UTF32 **sourceStart , const UTF32 *sourceEnd , UTF8 **targetStart , UTF8 *targetEnd , ULONG flags );
ULONG CodesetsConvertUTF8toUTF32 ( const UTF8 **sourceStart , const UTF8 *sourceEnd , UTF32 **targetStart , UTF32 *targetEnd , ULONG flags );

ULONG CodesetsEncodeB64A ( struct TagItem *attrs );
ULONG CodesetsEncodeB64 ( Tag tag1 , ... );
ULONG CodesetsDecodeB64A ( struct TagItem *attrs );
ULONG CodesetsDecodeB64 ( Tag tag1 , ... );

STRPTR *CodesetsSupportedA ( struct TagItem *attrs );
STRPTR *CodesetsSupported ( Tag tag1 , ... );
void CodesetsFreeA ( APTR obj , struct TagItem *attrs );
void CodesetsFree ( APTR obj , Tag tag1 , ... );
struct codeset *CodesetsSetDefaultA ( STRPTR name , struct TagItem *attrs );
struct codeset *CodesetsSetDefault ( STRPTR name , Tag tag1 , ... );
struct codeset *CodesetsFindA ( STRPTR name , struct TagItem *attrs );
struct codeset *CodesetsFind ( STRPTR name , Tag tag1 , ... );
struct codeset *CodesetsFindBestA ( STRPTR text , ULONG text_len , ULONG *error_ptr , struct TagItem *attrs );
struct codeset *CodesetsFindBest ( STRPTR text , ULONG text_len , ULONG *error_ptr , Tag tag1 , ... );
ULONG CodesetsUTF8Len ( const UTF8 *str );
ULONG CodesetsStrLenA ( STRPTR str , struct TagItem *attrs );
ULONG CodesetsStrLen ( STRPTR str , Tag tag1 , ... );
STRPTR CodesetsUTF8ToStrA (struct TagItem *attrs );
STRPTR CodesetsUTF8ToStr ( Tag tag1 , ... );
UTF8 *CodesetsUTF8CreateA ( struct TagItem *attrs );
UTF8 *CodesetsUTF8Create ( Tag tag1 , ... );
ULONG CodesetsIsValidUTF8 ( STRPTR str ) ;

#endif /* CLIB_CODESETS_PROTOS_H */
