#if !defined _MISCMACROS_H
#define _MISCMACROS_H

/* suppress compiler warnings about unused parameters */
#if defined UNUSED
    #undef UNUSED
#endif

#define UNUSED(param) ((void)param)


/* Possibly a better solution.  Use these for now */
#if defined MIN
    #undef MIN
#endif
#if defined MAX
    #undef MAX
#endif

#define MAX(X,Y)  ((X) > (Y) ? (X) : (Y) )
#define MIN(X,Y)  ((X) < (Y) ? (X) : (Y) )

#endif
