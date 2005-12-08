
#include <proto/exec.h>
#include <proto/codesets.h>
#include <stdio.h>
#include <string.h>

struct Library *CodesetsBase = NULL;
#if defined(__amigaos4__)
struct CodesetsIFace* ICodesets = NULL;
#endif

#if defined(__amigaos4__)
#define GETINTERFACE(iface, base)	(iface = (APTR)GetInterface((struct Library *)(base), "main", 1L, NULL))
#define DROPINTERFACE(iface)			(DropInterface((struct Interface *)iface), iface = NULL)
#else
#define GETINTERFACE(iface, base)	TRUE
#define DROPINTERFACE(iface)
#endif

int main(int argc,char **argv)
{
    int res;

    if (argc==3)
    {

        if((CodesetsBase = OpenLibrary(CODESETSNAME,CODESETSVER)) &&
           GETINTERFACE(ICodesets, CodesetsBase))
        {
            ULONG r;

            r = CodesetsDecodeB64(CODESETSA_B64SourceFile, argv[1],
                                  CODESETSA_B64DestFile,   argv[2],
                                  TAG_DONE);
            printf("Res %ld\n",r);

            DROPINTERFACE(ICodesets);
            CloseLibrary(CodesetsBase);
            CodesetsBase = NULL;

            res = 0;
        }
        else
        {
            printf("can't open %s %d+\n",CODESETSNAME,CODESETSVER);
            res = 20;
        }
    }
    else
    {
        printf("Usage: b64d <in_file> <out_file>\n");
        res = 10;
    }
    
    return res;
}
