#ifndef _LIB_PROTOS_H
#define _LIB_PROTOS_H

/* init.c */
void freeBase ( void );
ULONG initBase ( void );

/* utils.c */
APTR allocVecPooled ( APTR pool , ULONG size );
void freeVecPooled ( APTR pool , APTR mem );
APTR allocArbitratePooled ( ULONG s );
void freeArbitratePooled ( APTR mem , ULONG s );
APTR allocArbitrateVecPooled ( ULONG size );
void freeArbitrateVecPooled ( APTR mem );
#ifdef __MORPHOS__
void sprintf ( STRPTR buf , STRPTR fmt , ...) __attribute((varargs68k));
int snprintf ( STRPTR buf , int size , STRPTR fmt , ...) __attribute((varargs68k));
#else
void STDARGS sprintf ( STRPTR to , STRPTR fmt , ...);
int STDARGS snprintf ( STRPTR buf , int size , STRPTR fmt , ...);
#endif
int countNodes ( struct MinList *List );

/* base64.c */
ULONG LIBCALL CodesetsEncodeB64A ( REG (a0 )struct TagItem *attrs );
ULONG LIBCALL CodesetsDecodeB64A ( REG (a0 )struct TagItem *attrs );

/* convertUTF.c */
ULONG LIBCALL CodesetsConvertUTF32toUTF16 ( REG (a0 )const UTF32 **sourceStart , REG (a1 )const UTF32 *sourceEnd , REG (a2 )UTF16 **targetStart , REG (a3 )UTF16 *targetEnd , REG (d0 )ULONG flags );
ULONG LIBCALL CodesetsConvertUTF16toUTF32 ( REG (a0 )const UTF16 **sourceStart , REG (a1 )const UTF16 *sourceEnd , REG (a2 )UTF32 **targetStart , REG (a3 )UTF32 *targetEnd , REG (d0 )ULONG flags );
ULONG LIBCALL CodesetsConvertUTF16toUTF8 ( REG (a0 )const UTF16 **sourceStart , REG (a1 )const UTF16 *sourceEnd , REG (a2 )UTF8 **targetStart , REG (a3 )UTF8 *targetEnd , REG (d0 )ULONG flags );
ULONG LIBCALL CodesetsIsLegalUTF8 ( REG (a0 )const UTF8 *source , REG (d0 )ULONG length );
ULONG LIBCALL CodesetsIsLegalUTF8Sequence ( REG (a0 )const UTF8 *source , REG (a1 )const UTF8 *sourceEnd );
ULONG LIBCALL CodesetsConvertUTF8toUTF16 ( REG (a0 )const UTF8 **sourceStart , REG (a1 )const UTF8 *sourceEnd , REG (a2 )UTF16 **targetStart , REG (a3 )UTF16 *targetEnd , REG (d0 )ULONG flags );
ULONG LIBCALL CodesetsConvertUTF32toUTF8 ( REG (a0 )const UTF32 **sourceStart , REG (a1 )const UTF32 *sourceEnd , REG (a2 )UTF8 **targetStart , REG (a3 )UTF8 *targetEnd , REG (d0 )ULONG flags );
ULONG LIBCALL CodesetsConvertUTF8toUTF32 ( REG (a0 )const UTF8 **sourceStart , REG (a1 )const UTF8 *sourceEnd , REG (a2 )UTF32 **targetStart , REG (a3 )UTF32 *targetEnd , REG (d0 )ULONG flags );
#ifdef __MORPHOS__
ULONG LIB_CodesetsConvertUTF32toUTF16 ( void );
ULONG LIB_CodesetsConvertUTF16toUTF32 ( void );
ULONG LIB_CodesetsConvertUTF16toUTF8 ( void );
ULONG LIB_CodesetsIsLegalUTF8 ( void );
ULONG LIB_CodesetsConvertUTF8toUTF16 ( void );
ULONG LIB_CodesetsConvertUTF32toUTF8 ( void );
ULONG LIB_CodesetsConvertUTF8toUTF32 ( void );
#endif

/* codesets.c */
STRPTR *LIBCALL CodesetsSupportedA ( REG (a0 )struct TagItem *attrs );
void LIBCALL CodesetsFreeA ( REG (a0 )APTR obj , REG (a1 )struct TagItem *attrs );
ULONG codesetsInit ( struct MinList *codesetsList );
void codesetsCleanup ( struct MinList *codesetsList );
struct codeset *codesetsFind ( struct MinList *codesetsList , STRPTR name );
struct codeset *LIBCALL CodesetsSetDefaultA ( REG (a0 )STRPTR name , REG (a1 )struct TagItem *attrs );
struct codeset *LIBCALL CodesetsFindA ( REG (a0 )STRPTR name , REG (a1 )struct TagItem *attrs );
struct codeset *codesetsFindBest ( struct MinList *codesetsList , STRPTR text , int text_len , int *error_ptr );
struct codeset *LIBCALL CodesetsFindBestA ( REG (a0 )STRPTR text , REG (d0 )ULONG text_len , REG (a1 )ULONG *error_ptr , REG (a2 )struct TagItem *attrs );
ULONG LIBCALL CodesetsUTF8Len ( REG (a0 )UTF8 *str );
ULONG LIBCALL CodesetsStrLenA ( REG (a0 )STRPTR str , REG (a1 )struct TagItem *attrs );
STRPTR LIBCALL CodesetsUTF8ToStrA ( REG (a0 )struct TagItem *attrs );
UTF8 *LIBCALL CodesetsUTF8CreateA ( REG (a0 )struct TagItem *attrs );
ULONG LIBCALL CodesetsIsValidUTF8 ( REG (a0 )STRPTR s );
void LIBCALL CodesetsFreeVecPooledA ( REG (a0 ) APTR pool , REG (a1 ) APTR mem , REG (a2 ) struct TagItem *attrs);
#ifdef __MORPHOS__
ULONG LIB_CodesetsConvertUTF32toUTF16 ( void );
ULONG LIB_CodesetsConvertUTF16toUTF32 ( void );
ULONG LIB_CodesetsConvertUTF16toUTF8 ( void );
ULONG LIB_CodesetsIsLegalUTF8 ( void );
ULONG LIB_CodesetsIsLegalUTF8Sequence ( void );
ULONG LIB_CodesetsConvertUTF8toUTF16 ( void );
ULONG LIB_CodesetsConvertUTF32toUTF8 ( void );
ULONG LIB_CodesetsConvertUTF8toUTF32 ( void );
struct codeset * LIB_CodesetsSetDefaultA ( void );
void LIB_CodesetsFreeA ( void );
STRPTR * LIB_CodesetsSupportedA ( void );
struct codeset * LIB_CodesetsFindA ( void );
struct codeset * LIB_CodesetsFindBestA ( void );
ULONG LIB_CodesetsUTF8Len ( void );
STRPTR LIB_CodesetsUTF8ToStrA ( void );
UTF8 * LIB_CodesetsUTF8CreateA ( void );
ULONG LIB_CodesetsEncodeB64A ( void );
ULONG LIB_CodesetsDecodeB64A ( void );
ULONG LIB_CodesetsStrLenA ( void );
ULONG LIB_CodesetsIsValidUTF8 ( void );
void LIB_CodesetsFreeVecPooledA ( void );
#endif

#endif /* _LIB_PROTOS_H */
