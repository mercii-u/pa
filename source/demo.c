#include "pa.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    // -
    // --
    // hola /home/ ./dert

    static const struct pa_option opts[] =
    {
        {"hlpe", 'h', pa_noway_arg},
    };

    unsigned short a;
    pa_get(argc, argv, &a, 1, opts);

    return 0;
}
