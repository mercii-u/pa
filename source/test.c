#include "pa.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    static struct pa_option ops[] = {
        {"document", 'D', pa_takes_arg},
        {"help",     'H', pa_might_arg},
        {"readonly", 'R', pa_noway_arg},
        {NULL, 0, 0}
    };


    unsigned short a; 
    while (1)
    {
        signed char got = pa_get(argc, argv, &a, ops);
        if (got == PA_CEST_FINI) break;

        switch (got)
        {
            case 'D':
            {
                printf("got 'D': %s\n", pa_argument);
                break;
            }
            case 'H':
            {
                printf("got 'H': %s\n", pa_argument ? pa_argument : "no arg");
                break;
            }
            case 'R':
            {
                printf("got 'R': %s\n", pa_argument ? pa_argument : "no arg");
                break;
            }
            case PA_POSITIONAL_ARG:
            {
                printf("positional arg: %s\n", pa_argument);
                break;
            }

            case PA_ERR_DASH_ONLY:
            {
                printf("error: -- or -\n");
                exit(0);
            }
            case PA_ERR_UNDEF_OP:
            {
                printf("error: undefined option: %s\n", argv[a]); // given as it is
                exit(0);
            }
            case PA_ERR_ARG_NO_GIVEN:
            {
                printf("error: argument was not given: %s\n", pa_flagname);
                exit(0);
            }
            case PA_ERR_ARG_GIVEN:
            {
                printf("error: argument was given even tho is it not needed: %s\n", argv[a]);
                exit(0);
            }

        }
    }

    return 0;
}
