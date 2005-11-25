
#include <proto/exec.h>
#include <proto/codesets.h>
#include <stdio.h>
#include <string.h>

#define SAVEDS  __saveds
#define ASM     __asm
#define REG(x)  register __ ## x

#define STR "\
xyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛ�\
�xyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛ\
Üxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚ�\
�Üxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚ\
ÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜxyz{|}~¡¢£€¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙ�\
�ÛÜ"

static ULONG ASM SAVEDS
destFun(REG(a0) struct Hook *hook,REG(a1) STRPTR buf,REG(a2) struct convertMsg *msg)
{
    printf("[%3ld] [%s]\n",msg->len,buf);
    if (msg->state==CODESETV_End) printf("\n");

    return 0;
}

int main(int argc,char **argv)
{
    struct Library *CodesetsBase;
    int            res;

    if (CodesetsBase = OpenLibrary(CODESETSNAME,CODESETSVER))
    {
        struct Hook dest;
        char        *str;

        if (argc>1) str = argv[1];
        else str = STR;

        dest.h_Entry = (HOOKFUNC)destFun;

        CodesetsUTF8ToStr(CODESETSA_Source,str,CODESETSA_DestLen,32,CODESETSA_DestHook,&dest,TAG_DONE);

        res = 0;

        CloseLibrary(CodesetsBase);
    }
    else
    {
        printf("can't open %s %d+\n",CODESETSNAME,CODESETSVER);
        res = 20;
    }

    return res;
}
