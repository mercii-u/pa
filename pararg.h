#ifndef PARARG_PARARG_H
#define PARARG_PARARG_H

#define PA_CEST_FINI          0
#define PA_ERR_EMPTY         -1
#define PA_ERR_UNDEF_OP      -2
#define PA_ERR_ARG_NO_GIVEN  -3
#define PA_ERR_ARG_GIVEN     -4 

enum pa_takes
{
    pa_takes_arg,
    pa_might_arg,
    pa_noway_arg
};

struct pa_option
{
    char  *option;
    char  id;
    enum  pa_takes takes;
};

extern char *pa_argument;

signed char pa_get (const unsigned int, char**, unsigned short*, struct pa_option*);


#endif
