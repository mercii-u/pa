#include "pa.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define CHECK_ALLOC(ptr)                                   \
    do {                                                   \
        if (ptr) break;                                    \
        fprintf(stderr, "PA: Cannot allocate memoery\n");  \
        exit(EXIT_FAILURE);                                \
    } while (0)

#define PATH_SPECIFIER(a)   ((a == '~') || (a == '.') || (a == '/'))

/* Extern defs.
 */
char *pa_argument = NULL;
char *pa_flagname = NULL;
char pa_unixstyle_allowed = 1;

/* This array stores the length of every flagname provided
 * in the options, this is made in order to not recalculate
 * them every time PA is looking for an flag.
 */
unsigned int *FlagLengths = NULL;

static void check_options_are_ok (const struct pa_option*, const unsigned short);
static enum pa_return kind_of_thing (const char*);

static enum pa_return handle_flag (const char*, const char, const char*, const struct pa_option*);

pa_t pa_get (const unsigned int argc, char **argv, unsigned short *index, const unsigned short nopts, const struct pa_option *opts)
{
    if (FlagLengths == NULL) { check_options_are_ok(opts, nopts); }

    /* `at` is an index to know what element is being read
     * from argv, this is also a way to let the programmer
     * to know what was the last string read in argv
     */
    static unsigned short at = 1;
    *index = at;

    while (at < argc)
    {
        const char *thing = argv[at++];
        switch (kind_of_thing(thing))
        {
            case pa_ret_nonsense:
            {
                return pa_ret_nonsense;
            }

            case pa_ret_1s_flag:
            case pa_ret_2s_flag:
            {
                break;
            }

            case pa_ret_argument:
            {
                printf("is anum argumenbt\n");
                break;
            }
        }
    }

    if (FlagLengths) { free(FlagLengths); }
    return pa_ret_cest_fini;
}

static void check_options_are_ok (const struct pa_option *opts, const unsigned short nopts)
{
    FlagLengths = (unsigned int*) calloc(nopts, sizeof(*FlagLengths));
    CHECK_ALLOC(FlagLengths);

    char *errReason = NULL;
    unsigned short opterr;

    for (unsigned int i = 0; i < nopts; i++)
    {
        if (opts[i].flag == NULL)
        {
            errReason = "Flagnames cannot be null nor start with spaces";
            opterr = i;
            break;
        }
        if (!isalnum(opts[i].id) || !isalnum(opts[i].flag[0]))
        {
            errReason = "Both id and flagnames must only contain [0-9] [a-z] or [A-Z]";
            opterr = i;
            break;
        }

        FlagLengths[i] = (unsigned int) strlen(opts[i].flag);
        if (FlagLengths[i] == 0)
        {
            errReason = "Flagnames cannot be empty";
            opterr = i;
            break;
        }
    }

    if (errReason != NULL)
    {
        fprintf(stderr, "\n  PA: error: %s\n", errReason);
        fprintf(stderr, "        bad: %dth option\n\n", ++opterr);
        exit(EXIT_FAILURE);
    }
}

static enum pa_return kind_of_thing (const char *thing)
{
    if (*thing == '-' && isalnum(*(thing + 1)) && *(thing + 2) == 0)
    {
        return pa_ret_1s_flag;
    }
    if (*thing == '-' && *(thing + 1) == '-' && isalnum(*(thing + 2)))
    {
        return pa_ret_2s_flag;
    }
    if (isalnum(*thing) || PATH_SPECIFIER(*thing))
    {
        return pa_ret_argument;
    }
    return pa_ret_nonsense;
}

static enum pa_return handle_flag (const char *flag, const char id, const char *next, const struct pa_option *opts)
{
}
