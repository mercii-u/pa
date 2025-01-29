#include "pa.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TOOK_ARG        1
#define DIDNT_TAKE_ARG  2

enum element_is
{
    ele_is_single,
    ele_is_double,
    ele_is_argument,
    ele_is_dash_alone,
    ele_is_end
};

enum arg_style
{
    style_unix,
    style_separated
};

char *pa_argument = NULL;
char *pa_flagname = NULL;

static void check_list_was_provided_ok (const struct pa_option*);
static enum element_is get_element_kind (const char*);

static signed char handle_single_dash (const char, const struct pa_option*, const char*);
static signed char handle_double_dash (const char*, const struct pa_option*, const char*, enum arg_style*);

static signed char check_arg_was_given_properly (const enum pa_takes, const enum element_is);
static char *arg_was_given_unix_like (const char*);

signed char pa_get (const unsigned int argc, char **argv, unsigned short *reading, const struct pa_option *opts)
{
    check_list_was_provided_ok(opts);
    static unsigned short index = 1;

    *reading = index;
    pa_argument = NULL;

    while (index < argc)
    {
        const char *ele = argv[index++];
        const char *next_ele = (index < argc) ? argv[index] : NULL;

        switch (get_element_kind(ele))
        {
            case ele_is_single:
            {
                signed char ret = handle_single_dash(*(ele + 1), opts, next_ele);
                if (pa_argument != NULL) index++;
                return ret;
            }
            case ele_is_double:
            {
                enum arg_style style;
                signed char ret = handle_double_dash(ele + 2, opts, next_ele, &style);
                if (pa_argument != NULL && style != style_unix) index++;
                return ret;
            }
            case ele_is_argument:   
            case ele_is_dash_alone: { pa_argument = (char*) ele; return PA_POSITIONAL_ARG; }
            case ele_is_end:        { return PA_CEST_FINI; }
        }
    }

    return PA_CEST_FINI;
}

static void check_list_was_provided_ok (const struct pa_option *opts)
{
    for (unsigned short i = 0; opts[i].option != NULL; i++)
    {
        if (!isalnum(opts[i].id))
        {
            static const char *const errmsg =
             "\n"
             "  PA: error: Id cannot be '%c' -> %d ASCII\n"
             "  flag `%s` was assigned to `%c` id\n"
             "  make sure the id is either a digit [0 - 9] or a character from the alphabet [a - z] or [A - Z]\n"
             "\n";

            const char id  = opts[i].id;
            fprintf(stderr, errmsg, id, id, opts[i].option, id);
            exit(EXIT_FAILURE);
        }
    }
}

static enum element_is get_element_kind (const char *ele)
{
    if (ele == NULL)
    {
        return ele_is_end;
    }
    if (*ele == '-' && *(ele + 1) == '-')
    {
        return isalnum(*(ele + 2)) ? ele_is_double : ele_is_dash_alone;
    }
    if (*ele == '-')
    {
        return isalnum(*(ele + 1)) ? ele_is_single : ele_is_dash_alone;
    }
    return ele_is_argument;
}

static signed char handle_single_dash (const char flag, const struct pa_option *opts, const char *next_ele)
{
    for (unsigned short i = 0; opts[i].option != NULL; i++)
    {
        if (opts[i].id != flag) { continue; }

        pa_flagname = opts[i].option;
        switch (check_arg_was_given_properly(opts[i].takes, get_element_kind(next_ele)))
        {
            case PA_ERR_ARG_NO_GIVEN: { return PA_ERR_ARG_NO_GIVEN; }
            case DIDNT_TAKE_ARG: { return opts[i].id; }
            case TOOK_ARG: { pa_argument = (char*) next_ele; return opts[i].id; }
        }
    }
    return PA_ERR_UNDEF_OP;
}

static signed char handle_double_dash (const char *flag, const struct pa_option *opts, const char *next_ele, enum arg_style *style)
{

    /* UNIX/GNU like:
     * GNU uses this style --flag=value i really do not like it
     * but it is useful as well.
     */
    size_t eqSignAt = 0;
    for (size_t i = 0; flag[i]; i++)
    {
        if (flag[i] == '=') { eqSignAt = i; break; }
    }

    for (unsigned short i = 0; opts[i].option != NULL; i++)
    {
        if (strncmp(opts[i].option, flag, strlen(opts[i].option))) { continue; }

        pa_flagname = opts[i].option;

        if (eqSignAt != 0 && (opts[i].takes != pa_takes_arg))
        {
            *style = style_unix;
            pa_argument = arg_was_given_unix_like(flag + eqSignAt + 1);
            return pa_argument == NULL ? PA_ERR_ARG_NO_GIVEN : opts[i].id;
        }

        *style = style_separated;
        switch (check_arg_was_given_properly(opts[i].takes, get_element_kind(next_ele)))
        {
            case PA_ERR_ARG_NO_GIVEN: { return PA_ERR_ARG_NO_GIVEN; }

            case DIDNT_TAKE_ARG: { return opts[i].id; }
            case TOOK_ARG: { pa_argument = (char*) next_ele; return opts[i].id; }
        }
    }

    return PA_ERR_UNDEF_OP;
}

static signed char check_arg_was_given_properly (const enum pa_takes takes, const enum element_is next_is)
{
    if (takes == pa_takes_arg && next_is != ele_is_argument) { return PA_ERR_ARG_NO_GIVEN; }
    if (takes == pa_noway_arg && next_is == ele_is_argument) { return PA_POSITIONAL_ARG; }

    if (takes == pa_might_arg && next_is != ele_is_argument) { return DIDNT_TAKE_ARG; }
    if (takes == pa_noway_arg && next_is != ele_is_single) { return DIDNT_TAKE_ARG; }

    return TOOK_ARG;
}

static char *arg_was_given_unix_like (const char *value)
{
    size_t length = 0;
    for (; value[length]; length++) ;
    
    if (length == 0) return NULL;

    char *arg = (char*) calloc(1, length + 1);
    if (arg == NULL)
    {
        fprintf(stderr, "\n  PA: Cannot allocate memoery\n\n");
        exit(EXIT_FAILURE);
    }

    memcpy(arg, value, length);
    return arg;
}
