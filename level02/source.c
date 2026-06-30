#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>


int main(int argc, char** argv, char** envp)
{
    int argc_1 = argc;
    char** argv_1 = argv;
    char var_78[0x64];
    char password[0x30];
    char userEntry[0x40];
    int var_10 = 0;
    int var_14 = 0;
    FILE* fp = fopen("/home/users/level03/.pass", "r");
    
    if (!fp)
    {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
        /* no return */
    }
    
    int rax = fread(&password, 1, 0x29, fp);
    password[strcspn(&password, "\n")] = 0;
    
    if (rax != 0x29)
    {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        exit(1);
        /* no return */
    }
    
    fclose(fp);
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\**************************************/");
    printf("--[ Username: ");
    fgets(&var_78, 0x64, stdin);
    var_78[strcspn(&var_78, "\n")] = 0;
    printf("--[ Password: ");
    fgets(&userEntry, 0x64, stdin);
    userEntry[strcspn(&userEntry, "\n")] = 0;
    puts("*****************************************");
    
    if (!strncmp(&password, &userEntry, 0x29))
    {
        printf("Greetings, %s!\n", &var_78, &var_78);
        system("/bin/sh");
        return 0;
    }
    printf(&var_78);
    puts(" does not have access!");
    exit(1);
    /* no return */
}