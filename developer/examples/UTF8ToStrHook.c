
#include <proto/exec.h>
#include <proto/codesets.h>
#include <stdio.h>
#include <string.h>

#define STR "\
xyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛ�\
�xyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛ\
Üxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚ�\
�Üxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚ\
ÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙ�\
�ÛÜ"

#include "SDI_hook.h"

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

HOOKPROTONH(destFunc, ULONG, struct convertMsg* msg, STRPTR buf)
{
    printf("[%3ld] [%s]\n",msg->len,buf);
    if (msg->state==CODESETV_End) printf("\n");

    return 0;
}
MakeStaticHook(destHook, destFunc);

int main(int argc,char **argv)
{
    int            res;

    if((CodesetsBase = OpenLibrary(CODESETSNAME,CODESETSVER)) &&
        GETINTERFACE(ICodesets, CodesetsBase))
    {
        char        *str;

        if (argc>1) str = argv[1];
        else str = STR;

        if(CodesetsIsValidUTF8(str))
        {
          CodesetsUTF8ToStr(CODESETSA_Source,   str,
                            CODESETSA_DestLen,  32,
                            CODESETSA_DestHook, &destHook,
                            TAG_DONE);
        }
        else
          printf("Error: example string wasn't recognized as UTF8!\n");

        res = 0;

        DROPINTERFACE(ICodesets);
        CloseLibrary(CodesetsBase);
        CodesetsBase = NULL;
    }
    else
    {
        printf("can't open %s %d+\n",CODESETSNAME,CODESETSVER);
        res = 20;
    }

    return res;
}
