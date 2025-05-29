/*
******************************************************************
******************************************************************
*******                                                   ********
******  (C) 1988-2008 Tecplot, Inc.                        *******
*******                                                   ********
******************************************************************
******************************************************************
*/

#pragma once

/*
 * The old style re-entry guards are needed to suppress compiler error "C2995 function template has already been defined"
 * for the checked_numeric_cast template function defined in this header.
 * The '#pragma once' instruction was not sufficient to fix it, but adding old style re-entry guards
 * fixed it, for some reason. The idea of using the old style re-entry guards came from gleaning into
 * STL headers where both '#pragma once' and re-entry guards are used simultaneously.
 */
#ifndef CODE_CONTRACT_H
#define CODE_CONTRACT_H

#include "ThirdPartyHeadersBegin.h"
    #if !defined NO_ASSERTS && defined CHECKED_BUILD
        #include <stdio.h>
        #include <stdlib.h>
    #endif
    #include <cstdlib>
    #include <iostream>
    #include <limits>
#include "ThirdPartyHeadersEnd.h"

/*
 * Provide four levels of assertion control. Assertions provide a mechanism
 * to enforce a contract between a client and service provider. The assertions
 * are listed in order of highest to lowest priority. Assertions can be turned
 * off individually by defining the appropriate name (see preprocessor
 * definitions below), however, lower priority assertions should be turned
 * off prior to higher ones. As confidence in the code increases all assertions
 * can be turned off by defining NO_ASSERTS.
 *
 * The assertions defined below have the following meanings:
 *
 *     INVARIANT - Asserts that a property's state is invariant throughout the
 *                 life of the property's scope. Stating invariant properties
 *                 of an application provides a deeper understanding of the
 *                 application's state.  These statements are usually
 *                 positioned just ahead of the preconditions and just after
 *                 the postconditions.
 *
 *     REQUIRE   - Asserts that a method's preconditions are within their
 *                 valid domains. Preconditions are conditions placed upon
 *                 any state information relied upon for the call. These
 *                 statements should be as close to the top of the method
 *                 as possible (except for assertions on invariant properties).
 *
 *     ENSURE    - Asserts that a method's postconditions are within their
 *                 valid ranges. Postconditions are conditions placed upon
 *                 any state information modified by the call. These
 *                 statements should be as close to the bottom of the method
 *                 (presumably there is only one exit point) as possible
 *                 (except for assertions on invariant properties).
 *
 *     CHECK     - Any other assertion not covered by the above assertions.
 *                 These are often added within a method body to specify
 *                 something that may not be immediately obvious to the reader
 *                 or to validate your assumptions about a call to a 3rd party
 *                 method that does not use runtime assertions for its
 *                 preconditions or postconditions. Obviously if the 3rd party
 *                 method uses assertions then there is no need for the CHECK.
 */

#define VALID_REF(p)      ((p)  != 0)
#define VALID_FN_REF(pf)  ((pf) != 0)

#if defined NO_ASSERTS
    #define ASSERT(EXPR)

    /**
     * If a project is compiled with "warnings treated as errors" we need a way to to remove
     * parameters or declarations that are only used for assertions if assertions are turned off.
     *
     * This macro is used in the implementation as follows for parameters:
     *
     *     void someFunction(int const ASSERT_ONLY(paramOnlyUsedInAssertions))
     *     {
     *        ...
     *     }
     *
     * or for as follows for declarations:
     *
     *     ASSERT_ONLY(int const someResult =)
     *         someFunctionCall(arg);
     *     CHECK(someResult == expectedValue);
     */
    #define ASSERT_ONLY(EXPR)
#else   // Asserted build
    /*
     * See note above for this macro.
     */
    #define ASSERT_ONLY(EXPR) EXPR

    #if defined CHECKED_BUILD
        #if defined(_MSC_VER)
            #pragma warning (push, 0)
            // Disable warning about a function not being used in a module that does not use assertions.
            #pragma warning (disable:4505)
        #endif
        #include <assert.h>
        static bool tpReportAssertion(
            char const* message,
            char const* srcFileName,
            int         srcLineNum)
        {
            char const* const logFileName = ::getenv("TP_ASSERT_LOG_FILENAME");
            if (logFileName != 0)
            {
                FILE* logFile = ::fopen(logFileName, "a");
                if (logFile != 0)
                {
                    ::fprintf(logFile, "Assertion failure:\n"
                                     "    File:   %s\n"
                                     "    Line:   %d\n"
                                     "    Reason: %s\n",
                            srcFileName, srcLineNum, message);
                    ::fclose(logFile);
                }
                else
                {
                    assert(!"Failed to log checked build assertion. Check permissions");
                }
                return true;
            }
            else
            {
                return false;
            }
        }
        #if defined(_MSC_VER)
            #pragma warning (pop)
        #endif
        #define CHECKED_ASSERT(EXPR) ((EXPR) ? (void)(0) : (tpReportAssertion(#EXPR,__FILE__,__LINE__) ? (void)(0) : assert(EXPR)))
        #define ASSERT(EXPR) CHECKED_ASSERT(EXPR)
    #elif !defined NDEBUG //Debug
        #include <assert.h>
        #define ASSERT(EXPR) assert(EXPR)
    #endif
#endif

#define INVARIANT(EXPR) ASSERT(EXPR)
#define REQUIRE(EXPR)   ASSERT(EXPR)
#define ENSURE(EXPR)    ASSERT(EXPR)
#define CHECK(EXPR)     ASSERT(EXPR)

#if defined NO_ASSERTS
    #define VERIFY(EXPR) ((void)(EXPR))
#elif defined CHECKED_BUILD
    #define VERIFY(EXPR) CHECKED_ASSERT(EXPR)
#elif !defined NDEBUG //Debug and Checked
    #define VERIFY(EXPR) assert(EXPR)
#endif

/* convenience macros for implication, P -> Q, and equivalence, P <-> Q. */
#define IMPLICATION(P,Q) (!(P) || (Q))
#define EQUIVALENCE(P,Q) ((!!(P)) == (!!(Q)))

#define VALID_MAP_KEY(key,map) (map.find(key) != map.end())

/* Used for legacy code only */
#define VALID_REF_OR_NULL(p)       (VALID_REF(p) || p == 0)
#define VALID_BOOLEAN(b)           ((b) == 1 || (b) == 0)
#define VALID_ENUM(value, type)    (0 <= (value) && (value) < END_##type)
#define VALID_NON_ZERO_LEN_STR(str) (VALID_REF(str) && str[0] != '\0')
#define VALID_CLASS_ENUM(e) (static_cast<std::decay<decltype((e))>::type>(0) <= (e) && (e) < std::decay<decltype((e))>::type::END_ENUM)
#ifdef __cplusplus
    #if __cplusplus > 199711L // C++11 and higher
        namespace tecplot {
        template<typename Dst, typename Src>
        inline Dst numeric_cast(Src value)
        {
            #if defined(_MSC_VER)
                #pragma warning (push, 0)
                #pragma warning (disable:4018)
            #elif defined LINUX && defined __GNUC__ && __cplusplus >= 201103L
                #define GCC_VERSION (__GNUC__ * 10000 \
                                     + __GNUC_MINOR__ * 100 \
                                     + __GNUC_PATCHLEVEL__)
                /* Test for GCC >= 4.6.0, the first version that allowed in-function pragma GCC diagnostics */
                #if GCC_VERSION >= 40600
                    #pragma GCC diagnostic push
                    #pragma GCC diagnostic ignored "-Wsign-compare"
                    #pragma GCC diagnostic ignored "-Wbool-compare"
                #endif
            #endif
            const bool positive_overflow_possible = std::numeric_limits<Dst>::max() < std::numeric_limits<Src>::max();
            const bool negative_overflow_possible =
                std::numeric_limits<Src>::is_signed ||
                (std::numeric_limits<Dst>::lowest() > std::numeric_limits<Src>::lowest());

            // unsigned <-- unsigned
            if ((!std::numeric_limits<Dst>::is_signed) && (!std::numeric_limits<Src>::is_signed)) {
                if (positive_overflow_possible && (value > std::numeric_limits<Dst>::max())) {
                    REQUIRE(!"positive overflow");
                }
            }
            // unsigned <-- signed
            else if ((!std::numeric_limits<Dst>::is_signed) && std::numeric_limits<Src>::is_signed) {
                if (positive_overflow_possible && (value > std::numeric_limits<Dst>::max())) {
                    REQUIRE(!"positive overflow");
                }
                else if (negative_overflow_possible && (value < 0)) {
                    REQUIRE(!"negative overflow");
                }

            }
            // signed <-- unsigned
            else if (std::numeric_limits<Dst>::is_signed && (!std::numeric_limits<Src>::is_signed)) {
                if (positive_overflow_possible && (value > std::numeric_limits<Dst>::max())) {
                    REQUIRE(!"positive overflow");
                }
            }
            // signed <-- signed
            else if (std::numeric_limits<Dst>::is_signed && std::numeric_limits<Src>::is_signed) {
                if (positive_overflow_possible && (value > std::numeric_limits<Dst>::max())) {
                    REQUIRE(!"positive overflow");
                }
                else if (negative_overflow_possible && (value < std::numeric_limits<Dst>::lowest())) {
                    REQUIRE(!"negative overflow");
                }
            }

            // limits have been checked, therefore safe to cast
            #if defined(_MSC_VER)
                #pragma warning (pop)
            #elif defined LINUX && defined __GNUC__ && __cplusplus >= 201103L && GCC_VERSION >= 40600
                #pragma GCC diagnostic pop
            #endif
            return static_cast<Dst>(value);
        }
        }
    #endif

    template <typename TargetType, typename SourceType> inline
    TargetType checked_numeric_cast(SourceType const& value)
    {
        #if __cplusplus > 199711L // C++11 and higher
        ASSERT_ONLY(
            tecplot::numeric_cast<TargetType>(value); // ...this asserts
        );
        #endif

        return static_cast<TargetType>(value);
    }

    namespace {
    /**
     * Private template class for housing a union of the types used for the alias cast.
     * @tparam TargetType
     *     represents the target value type
     * @tparam SourceType
     *     represents the value type from which the alias cast is performed
     */
    template<typename TargetType, typename SourceType>
    struct alias_cast_t
    {
        union
        {
            SourceType source;
            TargetType result;
        };
    };
    }

    /**
     * Use alias_cast when you wish to break strict aliasing rules and cast one type into another
     * without any conversion. Traditionally code would be written as follows:
     *     ItemAddress64::SubzoneAddress  nodeSubzone;
     *     ....
     *     uint64_t bytesToWrite = *(reinterpret_cast<uint64_t*>(&nodeSubzone));
     *     ...
     * however in GCC 9.3.1 this will issue a warning:
     *     "error: dereferencing type-punned pointer will break strict-aliasing rules [-Werror=strict-aliasing]"
     *
     * Generally type-punning should be avoided but, as the code above implies it is exactly what we
     * want to do when writing bytes to a stream. The alias_cast template solves the problem by
     * employing a union thereby solving the compiler warning and communicating the intent through
     * the use of this special cast.
     *
     * The above code should be re-written as follows:
     *     ItemAddress64::SubzoneAddress  nodeSubzone;
     *     ....
     *     uint64_t bytesToWrite = alias_cast<uint64_t>(nodeSubzone);
     *     ...
     *
     * @tparam TargetType
     *     represents the target value type
     * @tparam SourceType
     *     represents the value type from which the alias cast is performed
     * @precondition
     *     sizeof(TargetType) and sizeof(SourceType) must be the same
     * @param value
     *     value with with to perform an alias cast
     */
    template<typename TargetType, typename SourceType>
    TargetType alias_cast(SourceType value)
    {
        #if __cplusplus > 199711L // C++11 and higher
            static_assert(sizeof(TargetType) == sizeof(SourceType), "Cannot cast types of different sizes");
            alias_cast_t<TargetType, SourceType> ac {{value}};
        #else
            REQUIRE(sizeof(TargetType) == sizeof(SourceType));
            alias_cast_t<TargetType, SourceType> ac;
            ac.source = value;
        #endif
        return ac.result;
    }
#endif
#endif //CODE_CONTRACT_H
