/*****************************************************************
 *****************************************************************
 *******                                                  ********
 ****** Copyright (C) 1988-2010 Tecplot, Inc.              *******
 *******                                                  ********
 *****************************************************************
 *****************************************************************/
/* CORE SOURCE CODE REMOVED */

#ifndef _MASTER_H_
#define _MASTER_H_

/*
 * Uncomment the following to force generated slices to be saved as generated data instead of instructions.
 */
//#define SAVE_GENERATED_DATA


/*
 * Uncomment to activate orphaned datasets.  
 */
//#define ENABLE_ORPHANED_DATASETS
//

/*
 * Uncomment to make deferred zone extraction operations "available".   To really enable in the beta you must also put the following
 * in your tecplot.cfg file:
 *
 *   $!Internal "TechnologyPreview = 'OPERATE-TIME-DEFERRED'"
 *
 * NOTE: If NOT enabled then edit tecutilo.cpp and change LINKTOADDONS to XINKTOADDONS for TecUtilDataSetAddTransientJournalCommand
 * NOTE: This has been enabled since at least 2019.2....
 *
 */
#define DEFER_TRANSIENT_OPERATIONS

/*
 *  Uncomment to activate COBS in the dataset
 */

//#define COBS_IN_DATASET

/*
 * Optionally uncomment ....
 */
//#define DEBUG_ORPHANED_DATASETS


/*
 * Annotations that specify attributes of functions, function return values, and function input and
 * output parameters. The following table specifies the meaning in their context. The annotations
 * provide code generation tools with information for building language bindings to various Tecplot
 * 360 and Tecplot SDK related libraries.
 *
 * For purposes of this table the client is one making the call and the service is the recipient.
 *
 * +==================+=========================+=================================================================+
 * | Function Context | Annotation              | Meaning                                                         |
 * |   Result or      |                         |                                                                 |
 * |   Parameter      |                         |                                                                 |
 * |==================+=========================+=================================================================|
 * | Function         | TP_DIRECT               | Annotates a function for the serialized executor as one that    |
 * |                  |                         | should be called directly instead of being dispatched to the    |
 * |                  |                         | Engine's thread.                                                |
 * |==================+=========================+=================================================================|
 * | Result           | TP_OUT                  | Default for a function return value that does not transfer      |
 * |                  |                         | ownership. Because this is the most common scenario this        |
 * |                  |                         | annotation is implied and never explicitly used in this         |
 * |                  |                         | context.                                                        |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Scalar Result    | TP_GIVES                | Annotates a function scalar return value as one who's ownership |
 * |                  |                         | is transfered to the client. The client is responsible for      |
 * |                  |                         | properly disposing the value.                                   |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Array Result     | TP_ARRAY_GIVES          | Annotates a function array return value as one who's ownership  |
 * |                  |                         | is transfered to the client. The client is responsible for      |
 * |                  |                         | properly disposing the value.                                   |
 * |==================+=========================+=================================================================|
 * | Parameter        | TP_IN                   | Default for a function input parameter value sent to the        |
 * |                  |                         | service. Because this is the most common scenario this          |
 * |                  |                         | annotation is implied and never explicitly used.                |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Parameter        | TP_ACQUIRES             | Annotates a function parameter as one that sends a value to     |
 * |                  |                         | the service through the parameter and acquires shared           |
 * |                  |                         | ownership of the input value with the client. The service is    |
 * |                  |                         | not responsible for disposing the value however it is           |
 * |                  |                         | expected that a symmetric API exists that "releases" the        |
 * |                  |                         | library of this shared ownership. For example:                  |
 * |                  |                         |   void addListener(TP_ACQUIRES Listener& listener);             |
 * |                  |                         |   void removeListener(TP_RELEASES Listener& listener);          |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Parameter        | TP_RELEASES             | Annotates a function parameter as one that sends a value to     |
 * |                  |                         | the service through the parameter and releases previously       |
 * |                  |                         | shared ownership of the                                         |
 * |                  |                         | input value with the client. The service is not responsible     |
 * |                  |                         | for disposing the value however it is expected that a           |
 * |                  |                         | symmetric API exists that "releases" the library of this        |
 * |                  |                         | shared ownership. For example:                                  |
 * |                  |                         |   void addListener(TP_ACQUIRES Listener& listener);             |
 * |                  |                         |   void removeListener(TP_RELEASES Listener& listener);          |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Scalar Parameter | TP_OUT                  | Annotates a function scalar parameter as one that returns a     |
 * |                  |                         | value to the client through the parameter but does not          |
 * |                  |                         | transfer ownership of the output value to the client.           |
 * |                  |                         | The client is not responsible for disposing the value.          |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Scalar Parameter | TP_IN_OUT               | Annotates a function scalar parameter as one that both sends    |
 * |                  |                         | a value to the service and returns a value to the client        |
 * |                  |                         | through the parameter. Ownership of the input value is not      |
 * |                  |                         | transfered to the service nor is ownership of the output value  |
 * |                  |                         | transfered to the client. The service is not responsible for    |
 * |                  |                         | disposing  the input value and the client is not responsible    |
 * |                  |                         | for disposing the output value.                                 |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Array Parameter  | TP_ARRAY_OUT            | Annotates a function array parameter as one that returns a      |
 * |                  |                         | value to the client through the parameter but does not          |
 * |                  |                         | transfer ownership of the output value to the client.           |
 * |                  |                         | The client is not responsible for disposing the value.          |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Array Parameter  | TP_ARRAY_IN_OUT         | Annotates a function array parameter as one that both sends     |
 * |                  |                         | a value to the service and returns a value to the client        |
 * |                  |                         | through the parameter. Ownership of the input value is not      |
 * |                  |                         | transfered to the service nor is ownership of the output value  |
 * |                  |                         | transfered to the client. The service is not responsible for    |
 * |                  |                         | disposing  the input value and the client is not responsible    |
 * |                  |                         | for disposing the output value.                                 |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Scalar Parameter | TP_GIVES                | Annotates a function scalar parameter as one that returns a     |
 * |                  |                         | value to the client through the parameter and transfers         |
 * |                  |                         | ownership of the output value to the client. The client is      |
 * |                  |                         | responsible for properly disposing the value.                   |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Scalar Parameter | TP_RECEIVES             | Annotates a function scalar parameter as one that sends a value |
 * |                  |                         | to the service through the parameter and transfers ownership    |
 * |                  |                         | of the input value to the service. The service is responsible   |
 * |                  |                         | for properly disposing the value.                               |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Scalar Parameter | TP_RECEIVES_GIVES       | Annotates a function scalar parameter as one that both sends    |
 * |                  |                         | a value to the service and returns a value to the client        |
 * |                  |                         | through the  parameter. Ownership of the input value is         |
 * |                  |                         | transfered to the service and ownership of the output value is  |
 * |                  |                         | transfered to the client. The service is responsible for        |
 * |                  |                         | properly disposing the input value and the client is            |
 * |                  |                         | responsible for properly disposing the output value.            |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Array Parameter  | TP_ARRAY_GIVES          | Annotates a function array parameter as one that returns a      |
 * |                  |                         | value to the client through the parameter and transfers         |
 * |                  |                         | ownership of the output value to the client. The client is      |
 * |                  |                         | responsible for properly disposing the value.                   |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Array Parameter  | TP_ARRAY_RECEIVES       | Annotates a function array parameter as one that sends a value  |
 * |                  |                         | to the service through the parameter and transfers ownership    |
 * |                  |                         | of the input value to the service. The service is responsible   |
 * |                  |                         | for properly disposing the value.                               |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Array Parameter  | TP_ARRAY_RECEIVES_GIVES | Annotates a function array parameter as one that both sends     |
 * |                  |                         | a value to the service and returns a value to the client        |
 * |                  |                         | through the  parameter. Ownership of the input value is         |
 * |                  |                         | transfered to the service and ownership of the output value is  |
 * |                  |                         | transfered to the client. The service is responsible for        |
 * |                  |                         | properly disposing the input value and the client is            |
 * |                  |                         | responsible for properly disposing the output value.            |
 * |==================+=========================+=================================================================|
 * | The following annotations are not related to life-cycle but denote properties of the associated parameter.   |
 * | They are only meaningful if present and there is no "default," or the default can be considered the same as  |
 * | "unknown". These all #define out to nothing and are only to be found in the TecUtil public API header files. |
 * |------------------+-------------------------+-----------------------------------------------------------------|
 * | Queried Result   | TP_QUERY                | This applies to output parameters and return values and         |
 * | or Parameter     |                         | indicates the value is the one requested and not a status of    |
 * |                  |                         | success or failure of the request itself. For example, the      |
 * |                  |                         | return value of TecUtilIsBusy() is the requested value:         |
 * |                  |                         |   TP_QUERY Boolean_t TecUtilIsBusy()                            |
 * |                  |                         | whereas the return value of TecUtilNewLayout() is the status of |
 * |                  |                         | the function call itself:                                       |
 * |                  |                         |   Boolean_t TecUtilNewLayout()                                  |
 * |==================+=========================+=================================================================|
 */

/*
 * First check to make sure that our attribute keywords are not in conflict with any system defines.
 */
#if defined TP_DIRECT               || \
    defined TP_ACQUIRES             || \
    defined TP_RELEASES             || \
    defined TP_OUT                  || \
    defined TP_IN_OUT               || \
    defined TP_ARRAY_OUT            || \
    defined TP_ARRAY_IN_OUT         || \
    defined TP_GIVES                || \
    defined TP_RECEIVES             || \
    defined TP_RECEIVES_GIVES       || \
    defined TP_ARRAY_GIVES          || \
    defined TP_ARRAY_RECEIVES       || \
    defined TP_ARRAY_RECEIVES_GIVES || \
    defined TP_QUERY
        #error "Tecplot's attribute keywords are in direct conflict with other meanings."
#endif

#if defined USE_GCCXML_ATTRIBUTES
    #define TP_DIRECT               __attribute((gccxml("direct")))
    #define TP_ACQUIRES             __attribute((gccxml("acquires","in")))
    #define TP_RELEASES             __attribute((gccxml("releases","in")))
    #define TP_OUT                  __attribute((gccxml("out")))
    #define TP_IN_OUT               __attribute((gccxml("in","out")))
    #define TP_ARRAY_OUT            __attribute((gccxml("array","out")))
    #define TP_ARRAY_IN_OUT         __attribute((gccxml("array","in","out")))
    #define TP_GIVES                __attribute((gccxml("gives","out")))
    #define TP_RECEIVES             __attribute((gccxml("receives","in")))
    #define TP_RECEIVES_GIVES       __attribute((gccxml("receives","in","gives","out")))
    #define TP_ARRAY_GIVES          __attribute((gccxml("array","gives","out")))
    #define TP_ARRAY_RECEIVES       __attribute((gccxml("array","receives","in")))
    #define TP_ARRAY_RECEIVES_GIVES __attribute((gccxml("array","receives","in","gives","out")))
    #define TP_QUERY                __attribute((gccxml("query")))
#else
    #define TP_DIRECT
    #define TP_ACQUIRES
    #define TP_RELEASES
    #define TP_OUT
    #define TP_IN_OUT
    #define TP_ARRAY_OUT
    #define TP_ARRAY_IN_OUT
    #define TP_GIVES
    #define TP_RECEIVES
    #define TP_RECEIVES_GIVES
    #define TP_ARRAY_GIVES
    #define TP_ARRAY_RECEIVES
    #define TP_ARRAY_RECEIVES_GIVES
    #define TP_QUERY
#endif


/* CORE SOURCE CODE REMOVED */

#if defined _WIN32

#if !defined TECPLOTKERNEL

#if !defined MSWIN
#define MSWIN
#endif /* !MSWIN */

/* For the sake of some older add-ons,
   defined _WINDOWS, WINDOWS, and WIN32
   New code should always use MSWIN */

#if !defined WINDOWS
#define WINDOWS
#endif /* WINDOWS */

#if !defined _WINDOWS
#define _WINDOWS
#endif /* !_WINDOWS */

#if !defined WIN32
#define WIN32
#endif /* !WIN32 */

#if defined _DEBUG
#if !defined DEBUG
#define DEBUG
#endif
#elif defined CHECKED_BUILD
#if defined NO_ASSERTS
#undef NO_ASSERTS
#endif
#if defined NDEBUG
#undef NDEBUG // Defining NDEBUG causes system include file assert.h to define assert() to nothing
#endif
#else /* RELEASE */
#if !defined NDEBUG
#define NDEBUG
#endif
#endif /* _DEBUG */
#endif /* !TECPLOTKERNEL */

#if _MSC_VER >= 1400
#define VS_2005 /* Using VS2005 Compiler */
#endif

#if !defined TECPLOTKERNEL && defined VS_2005
/* Suppress the warnings about the
     deprecated c runtime functions. */

#if !defined _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#endif /* !TECPLOTKERNEL && VS_2005 */

#endif /* MSWIN */


#ifdef NDEBUG
# ifdef _DEBUG
#   error "Both NDEBUG and _DEBUG defined"
# endif
#elif defined TECPLOTKERNEL
# ifndef _DEBUG
#   define _DEBUG
# endif
#endif

#if !defined TECPLOTKERNEL
#if defined NDEBUG && !defined CHECKED_BUILD && !defined NO_ASSERTS
#define NO_ASSERTS
#endif
#endif

/* CORE SOURCE CODE REMOVED */

/* Now a requirement */
#define USE_3D_HARDWARE

#ifndef THREED
#  define THREED
#endif

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#if defined QUICKDEMO
#define DEMO
#endif

#if defined MicrosoftC
#define DOS
#endif

#if defined CRAYX
#define CRAY
#endif

#if defined IRISX
#define IRIS
#endif

#if defined HPX
#define HPUX
#define HP
#endif

#if defined IBMRS6000X
#define IBMRS6000
#endif

#if defined COMPAQALPHAX
#define COMPAQALPHA
#define COMPAQX
#define COMPAQ
#endif

#if defined DECALPHAX
#define DECALPHA
#define DECX
#endif

#if defined DECX
#define DEC
#endif

#if defined SUNSOLARISX || defined SUNSOLARIS86X
#define SUNX
#endif

#if defined SUNX
#define SUN
#endif

#if defined IRISX || defined CRAYX || defined HPX || defined SUNX || defined CONVEXX
#define UNIXX
#define SYSV
#endif

#if defined DECX || defined LINUX || defined IBMRS6000X || defined COMPAQX || defined DARWIN
#define UNIXX
#endif

/* CORE SOURCE CODE REMOVED */

#ifdef MSWIN
#if defined VS_2005
#define Widget LONG_PTR /* correct for 32 & 64 bit builds */
#else
#define Widget long
#endif

#endif /* MSWIN */


#if defined UNIXX
    typedef void *Widget;
#endif


#include <string.h>

#if !defined SYSV && !defined MSWIN
#include <strings.h>
#endif

#if defined (MicrosoftC)
#include <stdlib.h>
#define EXECOS
#ifndef FAR
#define FAR
#endif
#define VOID       void
#endif

#include <sys/types.h>
#include <stdlib.h>

#if defined UNIXX
#define FAR
#define NEAR
#include <unistd.h>
#endif

/* CORE SOURCE CODE REMOVED */

/* CORE SOURCE CODE REMOVED */
/*
 * If not building the tecplot kernel then at least
 * include the X Instrinsics.  This will make most
 * development for addons etc work.
 */

#if defined MSWIN
#include <windows.h>
#endif

/* CORE SOURCE CODE REMOVED */

/* Assume that if TRACE is not defined, then none of the TRACE macros are */
#if !defined (TRACE)
/* TRACE is not used by non-debug builds */
#if defined NDEBUG
#if defined MSWIN
#define TRACE                       __noop
#define TRACE0(s)                   __noop
#define TRACE1(S,a1)                __noop
#define TRACE2(s,a1,a2)             __noop
#define TRACE3(s,a1,a2,a3)          __noop
#define TRACE4(s,a1,a2,a3,a4)       __noop
#define TRACE5(s,a1,a2,a3,a4,a5)    __noop
#define TRACE6(s,a1,a2,a3,a4,a5,a6) __noop
#else
#define TRACE(str)                      ((void)0)
#define TRACE0(str)                     ((void)0)
#define TRACE1(str,a1)                  ((void)0)
#define TRACE2(str,a1,a2)               ((void)0)
#define TRACE3(str,a1,a2,a3)            ((void)0)
#define TRACE4(str,a1,a2,a3,a4)         ((void)0)
#define TRACE5(str,a1,a2,a3,a4,a5)      ((void)0)
#define TRACE6(str,a1,a2,a3,a4,a5,a6)   ((void)0)
#endif /* MSWIN */
#else /* DEBUG */
#if defined MSWIN
/* If the add-on is running in debug mode but does not
 * use MFC, then no TRACE macro is available. Thus, to make tracing available,
 * map TRACE to the win32 OutpuDebugString() function.
 */
# define TRACE(str)                    do {                                                 OutputDebugStringA(str); }    while (0)
# define TRACE1(str,a1)                do { char s[5000]; sprintf(s,str,a1);                OutputDebugStringA(s);   }    while (0)
# define TRACE2(str,a1,a2)             do { char s[5000]; sprintf(s,str,a1,a2);             OutputDebugStringA(s);   }    while (0)
# define TRACE3(str,a1,a2,a3)          do { char s[5000]; sprintf(s,str,a1,a2,a3);          OutputDebugStringA(s);   }    while (0)
# define TRACE4(str,a1,a2,a3,a4)       do { char s[5000]; sprintf(s,str,a1,a2,a3,a4);       OutputDebugStringA(s);   }    while (0)
# define TRACE5(str,a1,a2,a3,a4,a5)    do { char s[5000]; sprintf(s,str,a1,a2,a3,a4,a5);    OutputDebugStringA(s);   }    while (0)
# define TRACE6(str,a1,a2,a3,a4,a5,a6) do { char s[5000]; sprintf(s,str,a1,a2,a3,a4,a5,a6); OutputDebugStringA(s);   }    while (0)
# define TRACE0(str) TRACE(str)
#else
#define TRACE  printf
#define TRACE0 printf
#define TRACE1 printf
#define TRACE2 printf
#define TRACE3 printf
#define TRACE4 printf
#define TRACE5 printf
#define TRACE6 printf
#endif /* MSWIN */
#endif /* NDEBUG */
#endif /* !defined (TRACE) */


/*
  Platform independent way for add-ons to know how much space
  to allocate for a filename.
*/
#if !defined MAX_SIZEOFUTF8CHAR
#define MAX_SIZEOFUTF8CHAR 1
#endif

#if !defined (MaxCharsFilePath)
# if defined (MSWIN)
#   define MaxCharsFilePath (_MAX_PATH*MAX_SIZEOFUTF8CHAR+1) /* Includes traling '\0' */
# else
#   define MaxCharsFilePath 2047 /* ...not really a hard limit for Linux/Unix */
# endif /* MSWIN */
#endif /* !MaxCharsFilePath */

/* CORE SOURCE CODE REMOVED */

/* CORE SOURCE CODE REMOVED */

/*
 * Useful macro for finding out what version of GCC is being used.
 * To test for GCC > 4.1.2 do the following:
 *   #if defined __GNUC__ && GCC_VERSION > 40102
 *      ...do something
 *   #endif
 */
#if defined __GNUC__ && !defined GCC_VERSION
    #define GCC_VERSION (__GNUC__ * 10000 + \
                 __GNUC_MINOR__ * 100 + \
                 __GNUC_PATCHLEVEL__)
#endif

/* In windows min and max are being redefined in windef.h.
 * As we want to use the ones provided by the STL we undefined them
 */
#if defined MSWIN && defined max
# undef max
#endif

#if defined MSWIN && defined min
# undef min
#endif

#endif /* _MASTER_H_ */
