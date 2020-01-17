#include "../tcp/include/tcp.h"

#include <stdio.h>

int main(void)
{
    printf("%lu\n", sizeof(struct cun_server) / 1024 / 1024);
}