#include "pa.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    static const struct pa_option opts[] =
    {
        {"takes", 't', pa_takes_arg},
        {"might", 'm', pa_might_arg},
        {"noway", 'n', pa_noway_arg},
    };

    unsigned short a;

    while (1)
    {
        pa_t ret = pa_get(argc, argv, &a, 3, opts);
        if (ret == pa_ret_cest_fini) { break; }

        switch (ret)
        {
            case 't':
            {
                printf("Takes took (%s) : %s\n", pa_flagname, pa_argument);
                break;
            }
            case 'm':
            {
                printf("Might took? (%s): %s <%s>\n", pa_flagname, pa_argument == NULL ? "no" : "yes", pa_argument);
                break;
            }
            case 'n':
            {
                printf("No way was seen! (%s)\n", pa_flagname);
                break;
            }
            case pa_ret_pos_arg:
            {
                printf("pos argument given: %s\n", pa_argument);
                break;
            }
            case pa_ret_undef_flag:
            {
                printf("undeflag! %s\n", argv[a]);
                break;
            }
            case pa_ret_missed_arg:
            {
                printf("missed argument for `%s` flag\n", pa_flagname);
                break;
            }
            case pa_ret_nonsense:
            {
                printf("wtf '%s' does even mean?\n", argv[a]);
                break;
            }
        }
    }

    return 0;
}
