#ifndef TECPLOT_EXCEPTIONMACROS_H
#define TECPLOT_EXCEPTIONMACROS_H

#include "ExceptionBase.h"
// Use EXCEPTIONHEADER for library exceptions, LOCAL_EXCEPTIONHEADER for local exceptions
// (local exceptions are defined in a local source file rather than a library source file).
// Use EXCEPTIONIMPLEMENTATION in both cases
// 9/10/10, dto

#define EXCEPTIONHEADER(ExceptionName) \
namespace tecplot \
{ \
class exception_API ExceptionName : public virtual ExceptionBase \
{ \
public: \
    /** \
     */ \
    ExceptionName(std::string message); \
    /** \
     */ \
    ExceptionName(); \
    /** \
     */ \
    virtual ~ExceptionName() throw(); \
}; \
}

// Same as EXCEPTIONHEADER except "exception_API" is not used
#define LOCAL_EXCEPTIONHEADER(ExceptionName) \
namespace tecplot \
{ \
class ExceptionName : public virtual ExceptionBase \
{ \
public: \
    /** \
     */ \
    ExceptionName(std::string message); \
    /** \
     */ \
    ExceptionName(); \
    /** \
     */ \
    virtual ~ExceptionName() throw(); \
}; \
}

// only report thrown stack traces for debug builds
#if defined NDEBUG
  #define RECORD_EXCEPTION_STACK_TRACE false
#else
  #define RECORD_EXCEPTION_STACK_TRACE true
#endif

#define EXCEPTIONIMPLEMENTATION(ExceptionName) \
namespace tecplot \
{ \
ExceptionName::ExceptionName(std::string message) \
    : ExceptionBase(message, RECORD_EXCEPTION_STACK_TRACE) \
    { \
    } \
\
ExceptionName::ExceptionName() \
    : ExceptionBase() \
    { \
    } \
\
ExceptionName::~ExceptionName() throw() \
{ \
} \
}

#endif
