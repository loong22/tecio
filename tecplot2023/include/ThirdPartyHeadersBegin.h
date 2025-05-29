// this can not be replaced by #pragma once since
// this is undef'd in ThirdPartHeadersEnd.h and not
// at the end of this file

// Avoid warnings in headers that are out of our control
#if defined(_MSC_VER)
  #pragma warning (push, 0)
  // Explicitly disable warnings that we've turned on in HelperMacros.cmake
  #pragma warning (disable:4191)
  #pragma warning (disable:4242)
  #pragma warning (disable:4263)
  #pragma warning (disable:4264)
  #pragma warning (disable:4265)
  #pragma warning (disable:4266)
  #pragma warning (disable:4302)
  //boost-1_69\boost\crc.hpp(984) : warning C4701: potentially uninitialized local variable 'result' used
  #pragma warning (disable:4701)
  #pragma warning (disable:4702)
  #pragma warning (disable:4715)
  //Disabling warning about buffer overrun because gmock was causing it for Release builds.
  #pragma warning (disable:4789)
  #pragma warning (disable:4905)
  #pragma warning (disable:4906)
  #pragma warning (disable:4928)

  #if (_MSC_VER > 1600)
  // Disable code analysis warnings for third party headers.
    #include <codeanalysis\warnings.h>
    #pragma warning (push)
    #pragma warning (disable : ALL_CODE_ANALYSIS_WARNINGS)
  #endif

#elif defined(__clang__)

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wall"
  #pragma clang diagnostic ignored "-Wextra"

#elif defined(__GNUC__)
  #if ((__GNUC__  >= 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ >= 5) // gcc version >= 4.6

    #pragma GCC diagnostic push

    /// gcc version 4.x does not support suppressing all warnings using pragma
    /// directives. For example, one can't merely ignore -Wall like with clang
    /// But maybe some day... In the mean time, we enumerate all warnings that
    /// are attached to -Wall and -Wextra. These may or may not honor the ignored
    /// pragmas below.
    #pragma GCC diagnostic ignored "-Wall"
    #pragma GCC diagnostic ignored "-Wextra"

    /// All of these are equivalent to ignoring -Wall
    #pragma GCC diagnostic ignored "-Waddress"
  //#pragma GCC diagnostic ignored "-Warray-bounds=1"
  //#pragma GCC diagnostic ignored "-Wbool-compare"
    #pragma GCC diagnostic ignored "-Wc++11-compat"
  //#pragma GCC diagnostic ignored "-Wc++14-compat"
    #pragma GCC diagnostic ignored "-Wchar-subscripts"
    #pragma GCC diagnostic ignored "-Wcomment"
    #pragma GCC diagnostic ignored "-Wenum-compare"
    #pragma GCC diagnostic ignored "-Wformat"
    #pragma GCC diagnostic ignored "-Winit-self"
  //#pragma GCC diagnostic ignored "-Wlogical-not-parentheses"
    #pragma GCC diagnostic ignored "-Wmain"
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
  //#pragma GCC diagnostic ignored "-Wmemset-transposed-args"
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #pragma GCC diagnostic ignored "-Wmissing-braces"
    #pragma GCC diagnostic ignored "-Wnarrowing"
    #pragma GCC diagnostic ignored "-Wnonnull"
  //#pragma GCC diagnostic ignored "-Wopenmp-simd"
    #pragma GCC diagnostic ignored "-Wparentheses"
    #pragma GCC diagnostic ignored "-Wreorder"
    #pragma GCC diagnostic ignored "-Wreturn-type"
    #pragma GCC diagnostic ignored "-Wsequence-point"
    #pragma GCC diagnostic ignored "-Wsign-compare"
    #pragma GCC diagnostic ignored "-Wsizeof-pointer-memaccess"
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
  //#pragma GCC diagnostic ignored "-Wstrict-overflow=1"
    #pragma GCC diagnostic ignored "-Wswitch"
  //#pragma GCC diagnostic ignored "-Wtautological-compare"
    #pragma GCC diagnostic ignored "-Wtrigraphs"
    #pragma GCC diagnostic ignored "-Wuninitialized"
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wunused-label"
    #pragma GCC diagnostic ignored "-Wunused-value"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wvolatile-register-var"

    /// All of these are equivalent to ignoring -Wextra
    #pragma GCC diagnostic ignored "-Wclobbered"
    #pragma GCC diagnostic ignored "-Wempty-body"
    #pragma GCC diagnostic ignored "-Wignored-qualifiers"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wsign-compare"
    #pragma GCC diagnostic ignored "-Wtype-limits"
    #pragma GCC diagnostic ignored "-Wuninitialized"
  //#pragma GCC diagnostic ignored "-Wshift-negative-value"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

    /// not covered by -Wall or -Wextra
    #pragma GCC diagnostic ignored "-Wliteral-suffix"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wunused-local-typedefs"

    /*
     * Boost's own code in boost/geometry and boost/math/tools produce warnings. These defines
     * suppress the warnings when we include boost headers. Keep track of the original settings so
     * we can put them back in ThirdPartyHeadersEnd.h.
     */
    #if defined BOOST_GEOMETRY_DISABLE_DEPRECATED_03_WARNING
        #define BOOST_GEOMETRY_DISABLE_DEPRECATED_03_WARNING_was_set
    #else
        #define BOOST_GEOMETRY_DISABLE_DEPRECATED_03_WARNING
    #endif

    #if defined BOOST_MATH_DISABLE_DEPRECATED_03_WARNING
        #define BOOST_MATH_DISABLE_DEPRECATED_03_WARNING_was_set
    #else
        #define BOOST_MATH_DISABLE_DEPRECATED_03_WARNING
    #endif
  #endif // gcc version >= 4.6

#endif
