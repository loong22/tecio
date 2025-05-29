#pragma once

#include "ThirdPartyHeadersBegin.h"
#include <string>
#include <exception>
#include "ThirdPartyHeadersEnd.h"

#include "exception_Exports.h"

#if defined SWIG
    #define exception_API
#endif

#if defined MSWIN
#pragma warning(push)

#if _MSC_VER && _MSC_VER >= 1910
    //Warnings are being issued about the exported ExceptionBase class deriving from a non-exported
    //std::exception starting with VS2017.
    #pragma warning(disable: 4275) // non � DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#endif
#endif

namespace tecplot
{

/**
 */
exception_API std::string GetCurBacktrace(int MaxBacktraceDepth);

/**
 */
exception_API void PrintCurBacktrace(
    FILE *TargetFile,
    int   MaxBacktraceDepth);

/**
 * Base class for all exceptions throwable from the Tecplot Engine.
 */
class exception_API ExceptionBase : public std::exception
{
public:
    /**
     * Constructs a new exception with a detailed message.
     *
     * @param message
     *     Detailed message about the exception.
     * @param recordStackTrace
     *     Indicates if the stack track should be recorded at the point the
     *     exception was thrown.
     */
    ExceptionBase(
        std::string const& message,
        bool recordStackTrace = false);

    /**
     * Constructs a new exception with out a detailed message.
     */
    ExceptionBase();

    /**
     * Constructs a new exception with a detailed message and a cause.
     *
     * Note that the detail message associated with cause is not automatically
     * incorporated in this exception's detail message.
     *
     * @param message
     *     Detailed message about the exception.
     * @param cause
     *     Cause for the exception.
     * @param recordStackTrace
     *     Indicates if the stack track should be recorded at the point the
     *     exception was thrown.
     */
    ExceptionBase(
        std::string const& message,
        ExceptionBase const&  cause,
        bool        recordStackTrace = false);

    #if !defined SWIG
    /**
     * Assignment operator.
     */
    ExceptionBase& operator=(ExceptionBase const& other);
    #endif

    /**
     * Copy constructor.
     */
    ExceptionBase(ExceptionBase const& other);

    /**
     * Note that this has an exception specification to match std::exception (on some platforms).
     * Removing the exception spec will cause compilation errors.
     */
    virtual ~ExceptionBase() throw();

    /**
     * Returns the cause of this exception or NULL if the cause is non-existent
     * or unknown.
     */
    virtual ExceptionBase const* cause() const;

    /**
     * Returns the detail message string of this exception.
     */
    virtual std::string message() const;

    /**
     * If available, prints this exception's stack trace to the stderr.
     */
    virtual void printStackTrace() const;

    /**
     * If available, prints this exception's stack trace to the specified file.
     *
     * @param file
     *     Output file to receive the stack trace if it is available.
     */
    virtual void printStackTrace(FILE* file) const;

    /**
     */
    virtual const char* what() const throw();

private:
    void swap(ExceptionBase& left, 
              ExceptionBase& right) throw();

    std::string    m_message;
    std::string    m_stackTrace;
    ExceptionBase const* m_cause;
};

}

#if defined MSWIN
#pragma warning(pop)
#endif
