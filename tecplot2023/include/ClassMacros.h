#pragma once

/*
 * We need to first undefine UNCOPYABLE_CLASS because RS builds against a frozen version of the SDK
 * that supplies ClassMacros.h and common/src/general which, depending on the include order, drags
 * in the ClassMacros.h file in that directory. Note that this doesn't really fix the real problem
 * because there are many other header files where there are duplicates between the SDK and the
 * common project but this is the only one that was producing a redefinition warning.
 */
#if defined UNCOPYABLE_CLASS
    #undef UNCOPYABLE_CLASS
#endif

/**
 * Convenience macro to declare that a class is not copyable.
 */
#if __cplusplus >= 201103L || (defined _MSC_VER && __cplusplus > 199711L)
    #define UNCOPYABLE_CLASS(CLASS_NAME) \
        CLASS_NAME(CLASS_NAME const&) = delete;\
        CLASS_NAME& operator=(CLASS_NAME const&) = delete
#else
    #define UNCOPYABLE_CLASS(CLASS_NAME) \
        private:\
            CLASS_NAME(CLASS_NAME const&);\
            CLASS_NAME& operator=(CLASS_NAME const&)
#endif

#if defined MSWIN
    #define DECLARE_IMPL() \
        /* Disable the warning about having to have the Impl declared with dll export. \
           Impls are private symbols and do not need to be made visible to clients of \
           a library. VS doesn't pay attention to them being declared in the 'private' \
           section and issues the warning.*/ \
        __pragma(warning (push)) \
        __pragma(warning (disable:4251)) \
            struct Impl; \
            std::unique_ptr<Impl> const m_impl; \
        __pragma(warning (pop))
#else
    #define DECLARE_IMPL() \
            struct Impl; \
            std::unique_ptr<Impl> const m_impl;
#endif

