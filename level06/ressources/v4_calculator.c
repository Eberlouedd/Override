#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int v5 = strnlen(argv[1], 32);
    int v4 = (argv[1][3] ^ 0x1337) + 6221293;

    for (int i = 0; i < v5; ++i)
    {
      if (argv[1][i] <= 31 )
        return 1;
      v4 += (v4 ^ (unsigned int)argv[1][i]) % 0x539;
    }
    printf("%d\n", v4);
    return 0;
}