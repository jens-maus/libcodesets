#ifndef PROTO_CODESETS_H
#define PROTO_CODESETS_H

#ifndef __NOLIBBASE__
extern struct Library *
#ifdef __CONSTLIBBASEDECL__
__CONSTLIBBASEDECL__
#endif /* __CONSTLIBBASEDECL__ */
CodesetsBase;
#endif /* !__NOLIBBASE__ */

#include <clib/codesets_protos.h>

#ifdef __GNUC__
#ifdef __PPC__
#ifndef _NO_PPCINLINE
#include <ppcinline/codesets.h>
#endif /* _NO_PPCINLINE */
#else
#ifndef _NO_INLINE
#include <inline/codesets.h>
#endif /* _NO_INLINE */
#endif /* __PPC__ */
#else
#include <pragmas/codesets_pragmas.h>
#endif /* __GNUC__ */

#endif /* !PROTO_CODESETS_H */

