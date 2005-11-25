
#include <proto/exec.h>
#include <proto/codesets.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
    int res;

    if (argc==3)
    {
        struct Library *CodesetsBase;

        if (CodesetsBase = OpenLibrary(CODESETSNAME,CODESETSVER))
        {
            ULONG r;

            r = CodesetsEncodeB64(CODESETSA_B64SourceFile, argv[1],
                                  CODESETSA_B64DestFile,   argv[2],
                                  TAG_DONE);
            printf("Res %ld\n",r);

            CloseLibrary(CodesetsBase);

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
        printf("Usage: b64e <in_file> <out_file>\n");
        res = 10;
    }
    
    return res;
}
