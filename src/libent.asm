
        SECTION codesets.library,CODE

        NOLIST

        INCLUDE "codesets.library_rev.i"
        INCLUDE "exec/libraries.i"
        INCLUDE "exec/resident.i"
        INCLUDE "exec/initializers.i"
        INCLUDE "exec/lists.i"
        INCLUDE "exec/semaphores.i"
        INCLUDE "utility/hooks.i"

        LIST

        XREF    _LinkerDB
        XREF    _lib_name
        XREF    ENDCODE
        XDEF    _ID

        XREF    _openLib
        XREF    _closeLib
        XREF    _expungeLib
        XREF    _initLib

        XREF    _CodesetsConvertUTF32toUTF16
        XREF    _CodesetsConvertUTF16toUTF32
        XREF    _CodesetsConvertUTF16toUTF8
        XREF    _CodesetsIsLegalUTF8
        XREF    _CodesetsIsLegalUTF8Sequence
        XREF    _CodesetsConvertUTF8toUTF16
        XREF    _CodesetsConvertUTF32toUTF8
        XREF    _CodesetsConvertUTF8toUTF32
        XREF    _CodesetsSetDefaultA
        XREF    _CodesetsFreeA
        XREF    _CodesetsSupportedA
        XREF    _CodesetsFindA
        XREF    _CodesetsFindBestA
        XREF    _CodesetsUTF8Len
        XREF    _CodesetsStrLenA
        XREF    _CodesetsUTF8ToStrA
        XREF    _CodesetsUTF8CreateA

        XREF    _CodesetsEncodeB64A
        XREF    _CodesetsDecodeB64A

        XREF    _CodesetsIsValidUTF8

        XREF	_CodesetsFreeVecPooledA

PRI     EQU 0

start:  moveq   #-1,d0
        rts

romtag:
        dc.w    RTC_MATCHWORD
        dc.l    romtag
        dc.l    ENDCODE
        dc.b    RTF_AUTOINIT
        dc.b    VERSION
        dc.b    NT_LIBRARY
        dc.b    PRI
        dc.l    _lib_name
        dc.l    _ID
        dc.l    init

_ID:    VSTRING

        CNOP    0,4

init:   dc.l    LIB_SIZE
        dc.l    funcTable
        dc.l    dataTable
        dc.l    _initLib

V_DEF   MACRO
    dc.w    \1+(*-funcTable)
    ENDM

funcTable:
        dc.w   -1

        V_DEF  _openLib
        V_DEF  _closeLib
        V_DEF  _expungeLib
        V_DEF  nil
        V_DEF  nil

        V_DEF  _CodesetsConvertUTF32toUTF16
        V_DEF  _CodesetsConvertUTF16toUTF32
        V_DEF  _CodesetsConvertUTF16toUTF8
        V_DEF  _CodesetsIsLegalUTF8
        V_DEF  _CodesetsIsLegalUTF8Sequence
        V_DEF  _CodesetsConvertUTF8toUTF16
        V_DEF  _CodesetsConvertUTF32toUTF8
        V_DEF  _CodesetsConvertUTF8toUTF32
        V_DEF  _CodesetsSetDefaultA
        V_DEF  _CodesetsFreeA
        V_DEF  _CodesetsSupportedA
        V_DEF  _CodesetsFindA
        V_DEF  _CodesetsFindBestA
        V_DEF  _CodesetsUTF8Len
        V_DEF  _CodesetsUTF8ToStrA
        V_DEF  _CodesetsUTF8CreateA

        V_DEF  _CodesetsEncodeB64A
        V_DEF  _CodesetsDecodeB64A

        V_DEF   _CodesetsStrLenA

        V_DEF   _CodesetsIsValidUTF8

        V_DEF  	_CodesetsFreeVecPooledA

        dc.w   -1

dataTable:
        INITBYTE LN_TYPE,NT_LIBRARY
        INITLONG LN_NAME,_lib_name
        INITBYTE LIB_FLAGS,(LIBF_SUMUSED!LIBF_CHANGED)
        INITWORD LIB_VERSION,VERSION
        INITWORD LIB_REVISION,REVISION
        INITLONG LIB_IDSTRING,_ID
        dc.w     0

        CNOP    0,4

nil:    moveq   #0,d0
        rts

        END
