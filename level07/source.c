#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

int clear_stdin()
{
    char var_d = 0;
    char i;
    
    do
    {
        i = getchar();
        
        if (i == 0xa)
            break;
    } while (i != 0xff);
    
    return i;
}

int get_unum()
{
    int result = 0;
    fflush(stdout);
    __isoc99_scanf("%u", &result);
    clear_stdin();
    return result;
}

void prog_timeout()
{
    exit(1);
}

int store_number(int arg1)
{
    int var_14 = 0;
    int var_10 = 0;
    printf(" Number: ");
    int eax = get_unum();
    printf(" Index: ");
    int eax_1 = get_unum();
    
    if (eax_1 % 3 && eax >> 0x18 != 0xb7)
    {
        *((eax_1 << 2) + arg1) = eax;
        return 0;
    }
    
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return 1;
}

int read_number(int arg1)
{
    int var_10 = 0;
    printf(" Index: ");
    int eax = get_unum();
    printf(" Number at data[%u] is %u\n", eax, ((eax << 2) + arg1));
    return 0;
}

int main(int argc, const char **argv, const char **envp)
{
  _BYTE v6[400]; // [esp+24h] [ebp-1B8h] BYREF = int v6[100]
  int number; // [esp+1B4h] [ebp-28h]
  char s[4]; // [esp+1B8h] [ebp-24h] BYREF
  int v9; // [esp+1BCh] [ebp-20h]
  int v10; // [esp+1C0h] [ebp-1Ch]
  int v11; // [esp+1C4h] [ebp-18h]
  int v12; // [esp+1C8h] [ebp-14h]
  unsigned int v13; // [esp+1CCh] [ebp-10h]

  v13 = __readgsdword(0x14u);
  number = 0;
  *s = 0;
  v9 = 0;
  v10 = 0;
  v11 = 0;
  v12 = 0;
  memset(v6, 0, sizeof(v6));
  while ( *argv )
  {
    memset((void *)*argv, 0, strlen(*argv));
    ++argv;
  }
  while ( *envp )
  {
    memset((void *)*envp, 0, strlen(*envp));
    ++envp;
  }
  puts(
    "----------------------------------------------------\n"
    "  Welcome to wil's crappy number storage service!   \n"
    "----------------------------------------------------\n"
    " Commands:                                          \n"
    "    store - store a number into the data storage    \n"
    "    read  - read a number from the data storage     \n"
    "    quit  - exit the program                        \n"
    "----------------------------------------------------\n"
    "   wil has reserved some storage :>                 \n"
    "----------------------------------------------------\n");
  while ( 1 )
  {
    printf("Input command: ");
    number = 1;
    fgets(s, 20, stdin);
    s[strlen(s) - 1] = 0;
    if ( !memcmp(s, "store", 5u) )
    {
      number = store_number((int)v6);
      goto LABEL_13;
    }
    if ( !memcmp(s, "read", 4u) )
    {
      number = read_number((int)v6);
      goto LABEL_13;
    }
    if ( !memcmp(s, "quit", 4u) )
      return 0;
LABEL_13:
    if ( number )
      printf(" Failed to do %s command\n", s);
    else
      printf(" Completed %s command successfully\n", s);
    *s = 0;
    v9 = 0;
    v10 = 0;
    v11 = 0;
    v12 = 0;
  }
}