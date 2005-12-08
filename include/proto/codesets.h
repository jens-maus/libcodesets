#ifndef PROTO_CODESETS_H
#define PROTO_CODESETS_H

/*
**	$Id$
**	Includes Release 50.1
**
**	Prototype/inline/pragma header file combo
**
**	(C) Copyright 2003-2005 Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef LIBRARIES_CODESETS_H
#include <libraries/codesets.h>
#endif

/****************************************************************************/

#ifndef __NOLIBBASE__
 #ifndef __USE_BASETYPE__
  extern struct Library * CodesetsBase;
 #else
  extern struct Library * CodesetsBase;
 #endif /* __USE_BASETYPE__ */
#endif /* __NOLIBBASE__ */

/****************************************************************************/

#ifdef __amigaos4__
 #include <interfaces/codesets.h>
 #ifdef __USE_INLINE__
  #include <inline4/codesets.h>
 #endif /* __USE_INLINE__ */
 #ifndef CLIB_CODESETS_PROTOS_H
  #define CLIB_CODESETS_PROTOS_H 1
 #endif /* CLIB_CODESETS_PROTOS_H */
 #ifndef __NOGLOBALIFACE__
  extern struct CodesetsIFace *ICodesets;
 #endif /* __NOGLOBALIFACE__ */
#else /* __amigaos4__ */
 #ifndef CLIB_CODESETS_PROTOS_H
  #include <clib/codesets_protos.h>
 #endif /* CLIB_CODESETS_PROTOS_H */
 #if defined(__GNUC__)
  #ifndef __PPC__
   #include <inline/codesets.h>
  #else
   #include <ppcinline/codesets.h>
  #endif /* __PPC__ */
 #elif defined(__VBCC__)
  #ifndef __PPC__
   #include <inline/codesets_protos.h>
  #endif /* __PPC__ */
 #else
  #include <pragmas/codesets_pragmas.h>
 #endif /* __GNUC__ */
#endif /* __amigaos4__ */

/****************************************************************************/

#endif /* PROTO_CODESETS_H */
