#ifndef PARARG_PARARG_H
#define PARARG_PARARG_H

/* Specifies if a determinated flag takes arguments:
 * pa_takes_arg: --flag [arg-must-be-here]
 *           or: --flag=arg-must-here
 *
 * pa_might_arg: --flag [arg-might-be]
 *           or: --flag=arg-might-be
 *           or: --flag
 *
 * pa_noway_arg: --flag
 */
enum pa_takes
{
    pa_takes_arg,
    pa_might_arg,
    pa_noway_arg
};

/* `pa_get` function will always return any of these
 * or the id of the last flag seen
 */
enum pa_return
{
    /* External usage, the caller function
     * uses these enums to handle errors
     */
    pa_ret_cest_fini    =  0,                               // ok
    pa_ret_pos_arg      = -1,                               // ok
    pa_ret_undef_flag   = -2,                               // ok
    pa_ret_missed_arg   = -3,
    pa_ret_nonsense     = -4,                               // ok
    /* Internal usage, pa.c uses these enums
     * to know what is the nature of each
     * element in argv
     */
    pa_ret_argument     = -5,                               // ok
    pa_ret_end_argv     = -6,                               // might unnecessary
    pa_ret_1s_flag      = -7,                               // ok
    pa_ret_2s_flag      = -8,                               // ok
};

struct pa_option
{
    char  *flag;
    char  id;
    enum  pa_takes takes;
};

/* This variable stores the value of the last argument given
 */
extern char *pa_argument;

/* This variable stores the name of the last flag used
 */
extern char *pa_flagname;

/* Migth the programmer does not want to allow
 * unix style for providing arguments.
 * Allowed by default.
 */
extern char pa_unixstyle_allowed;

/* This is not the best name, but it's good enough.
 * This can either be an error, or the id of the
 * last argument given.
 */
typedef signed char pa_t;

pa_t pa_get (const unsigned int, char**, unsigned short*, const unsigned short, const struct pa_option*);

#endif
