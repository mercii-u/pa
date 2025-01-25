#include "pararg.h"
#include <ctype.h>
#include <stdio.h>

char *pa_argument = NULL;

enum whatIsThis
{
    it_is_single_op,
    it_is_double_op,
    it_is_argument,
    it_is_error,
    it_is_fini
};

static enum whatIsThis what_is_this (const char*);
static char argument_was_provided ();


static signed char handle_single_op (const char, struct pa_option*, const char*);


signed char pa_get (const unsigned int argc, char **argv, unsigned short *_at, struct pa_option *ops)
{
    static unsigned short at = 1;
    *_at = at;

    pa_argument = NULL;

    while (at < argc)
    {
        const char *this = argv[at++];

        switch (what_is_this(this))
        {
            case it_is_error: { return PA_ERR_EMPTY; }
            case it_is_single_op:
            {
                return handle_single_op(*(this + 1), ops, at < argc ? argv[at] : NULL);
            }
        }
    }

    return PA_CEST_FINI;
}

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
            case PA_ERR_EMPTY:
                printf("empty - or --\n");
                break;
            case PA_ERR_ARG_NO_GIVEN:
                printf("no given arg\n");
                break;
            case PA_ERR_ARG_GIVEN:
                printf("given arg\n");
                break;
            case PA_ERR_UNDEF_OP:
                printf("undef of\n");
                break;

            case 'D':
                printf("D was found: %s\n", pa_argument);
                break;
        }
    }


    return 0;
}


static enum whatIsThis what_is_this (const char *this)
{
    if (this == NULL)
    {
        return it_is_fini;
    }

    if (*this == '-' && *(this + 1) == '-')
    {
        return isalnum(*(this + 2)) == 0 ? it_is_error : it_is_double_op;
    }

    if (*this == '-')
    {
        return isalnum(*(this + 1)) == 0 ? it_is_error : it_is_single_op;
    }

    return it_is_argument;
}

static signed char handle_single_op (const char single, struct pa_option *ops, const char *nextinargv)
{
    const enum whatIsThis nextis = what_is_this(nextinargv);
    unsigned short optind = 0;

    while (ops[optind].option)
    {
        if (ops[optind].id == single)
        {
            const enum pa_takes takes = ops[optind].takes;

            if (takes == pa_takes_arg && nextis != it_is_argument) { return PA_ERR_ARG_NO_GIVEN; }
            if (takes == pa_noway_arg && nextis == it_is_argument) { return PA_ERR_ARG_GIVEN; }
            if (nextis != it_is_argument && takes == pa_might_arg) { return single; }

            pa_argument = (char*) nextinargv;
            return single;
        }
        optind++;
    }

    return PA_ERR_UNDEF_OP;
}
