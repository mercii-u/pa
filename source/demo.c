#include "pa.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    static const struct pa_option flags[] =
    {
        {"song",  's', pa_takes_arg},
        {"band",  'b', pa_takes_arg},
        {"date",  'd', pa_might_arg},
        {"liked", 'L', pa_noway_arg},
        {NULL, 0, 0}
    };

    unsigned short at = 0;
    while (1)
    {
        signed char got = pa_get(argc, argv, &at, flags);
        if (got == PA_CEST_FINI) break;

        switch (got)
        {
            case 's':
            {
                printf("Listening to: %s song\n", pa_argument);
                break;
            }
            case 'b':
            {
                printf("By: %s band\n", pa_argument);
                break;
            }
            case 'd':
            {
                printf("Date: %s\n", pa_argument ? pa_argument : "unknown");
                break;
            }
            case 'L':
            {
                printf("You like this song!\n");
                break;
            }
            case PA_POSITIONAL_ARG:
            {
                printf("Pos arg: %s\n", pa_argument);
                break;
            }
            case PA_ERR_DASH_ONLY:
            {
                printf("- or -- does not make any sense\n");
                exit(0);
            }
            case PA_ERR_UNDEF_OP:
            {
                printf("error: undefined option: %s\n", argv[at]);
                exit(0);
            }
            case PA_ERR_ARG_NO_GIVEN:
            {
                printf("error: argument was not given: %s\n", pa_flagname);
                exit(0);
            }
        }
    }

    return 0;
}
