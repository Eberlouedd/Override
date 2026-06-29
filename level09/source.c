#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

typedef struct struct_0 {
    char padding_0[180];
    unsigned int field_b4;
} struct_0;

extern unsigned long stdin;
int secret_backdoor(void)
{
    char v0[128];  // [bp-0x88]

    fgets(&v0, 128, stdin);
    return system(&v0);
}

void handle_msg(void)
{
    struct_0 v0;  // [bp-0xc8]
    unsigned long long flag;  // [bp-0x3c]
    unsigned long long choice;  // [bp-0x34]
    unsigned long long flag2;  // [bp-0x2c]
    unsigned long long flag3;  // [bp-0x24]
    unsigned long long flag4;  // [bp-0x1c]
    unsigned int v6;  // [bp-0x14]

    flag = 0;
    choice = 0;
    flag2 = 0;
    flag3 = 0;
    flag4 = 0;
    v6 = 140;
    set_username(&v0);
    set_msg(&v0);
    puts(">: Msg sent!");
    return;
}

void set_msg(struct_0 *a0)
{
    unsigned long long *cur;  // rdi
    unsigned long long v3;  // rcx
    char v0[1024];  // [bp-0x408]

    cur = &v0;
    for (v3 = 128; v3; cur += 1)
    {
        v3 -= 1;
        *(cur) = 0;
    }
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(&v0, 0x400, stdin);
    strncpy(a0, &v0, a0->field_b4);
    return;
}

extern unsigned long stdin;

void set_username(struct_0 *a0)
{
    unsigned long long *cur;  // rdi
    unsigned long long v4;  // rcx
    char v0[140];  // [bp-0x98]
    int j;  // [bp-0xc]

    cur = &v0;
    for (v4 = 16; v4; cur += 1)
    {
        v4 -= 1;
        *(cur) = 0;
    }
    puts(">: Enter your username");
    printf(">>: ");
    fgets(&v0, 128, stdin);
    for (j = 0; j <= 40; j += 1)
    {
        if (!v0[j])
        {
            printf(">: Welcome, %s", a0 + 140);
            return;
        }
        a0->padding_0[140 + j] = v0[j];
    }
    printf(">: Welcome, %s", a0 + 140);
    return;
}

unsigned int main(void)
{
    puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------");
    handle_msg();
    return 0;
}