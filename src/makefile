
TARGET  = codesets.library
DEST    = LIBS:

CC 	= ppc-morphos-gcc
CDEFS   = -DUSE_INLINE_STDARG
CFLAGS  = $(CDEFS) $(CWARN) -O2 -fomit-frame-pointer -mcpu=750 -mstring -mmultiple -noixemul -fstrict-aliasing -I../include/
LDFLAGS = -noixemul

CWARN   = -Wall\
	  -Wno-missing-braces\
	  -Wno-parentheses\
          -Wno-switch\
          -Wstrict-prototypes\
	  -Werror-implicit-function-declaration\
          -Wpointer-arith

OBJS    = libinit.o init.o utils.o base64.o convertUTF.o codesets.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -nostartfiles -o $@ $(OBJS)
        ppc-morphos-strip --strip-unneeded --remove-section=.comment $@

install:
	@copy $(TARGET) $(DEST)
	@delete $(TARGET)
	@avail flush

libinit.o:      libinit.c lib.h codesets.library_rev.h base.h macros.h lib_protos.h
init.o:         init.c lib.h base.h macros.h lib_protos.h
utils.o:        utils.c lib.h base.h macros.h lib_protos.h
base64.o:       base64.c lib.h base.h macros.h lib_protos.h
convertUTF.o:   convertUTF.c lib.h ConvertUTF.h base.h macros.h lib_protos.h
codesets.o:     codesets.c lib.h codesets_table.h ConvertUTF.h base.h macros.h lib_protos.h

