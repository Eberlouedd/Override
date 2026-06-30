#include <stdio.h>

int main(int argc, char** argv, char** envp)
{
    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");
    int var_14;
    __isoc99_scanf("%d", &var_14);
    
    if (var_14 != 5276)
    {
        puts("\nInvalid Password!");
        return 1;
    }
    
    puts("\nAuthenticated!");
    system("/bin/sh");
    return 0;
}