VERSION EQU 5
REVISION EQU 3
DATE MACRO
    dc.b '5.5.2005'
    ENDM
VERS MACRO
    dc.b 'codesets.library 5.3'
    ENDM
VSTRING MACRO
    dc.b 'codesets.library 5.3 (5.5.2005)',13,10,0
    ENDM
VERSTAG MACRO
    dc.b 0,'$VER: codesets.library 5.3 (5.5.2005)',0
    ENDM
