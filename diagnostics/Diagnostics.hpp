#pragma once

#ifndef DIAGNOSTICS_HPP_
    #define DIAGNOSTICS_HPP_
    
    #ifdef DIAGNOSTICS_H
        #error "Fatal: 'diagnostics.h' is already included."
        #error "Conflict: 'diagnostics.h' detected."
        #pragma message "Use only 'Diagnostics.hpp'."
    #endif /* DIAGNOSTICS_H */
    
    #define DIAGNOSTICS_H /* pollute the 'diagnostics.h macro namespace */
    
    #define STRINGIFY_(...) #__VA_ARGS__
    #define DIAGNOSTICS_STRINGIFY_(...) STRINGIFY_(__VA_ARGS__)
    
    #ifdef __clang__
        #define _compiler__ clang
        #define _compiler_id__ 0b11L
    #elifdef __GNUC__
        #define _compiler__ GCC
        #define _compiler_id__ 0b01L
    #else
        #define _compiler_id__ 0b00L
        #error "Fatal: unknown compiler"
    #endif /* __clang__ || __GNUC__ */
    
        #if _compiler_id__ > 0b00LL
        #define PRAGMA_DIAGNOSTICS_(...) \
        _Pragma \
        ( \
            DIAGNOSTICS_STRINGIFY_ \
            ( \
                _compiler__ diagnostic __VA_ARGS__ \
            ) \
        )
        #undef _
    
        #define POP_(...) PRAGMA_DIAGNOSTICS_(__VA_ARGS__)
        #define PUSH_(...) PRAGMA_DIAGNOSTICS_(__VA_ARGS__)
        #define ERROR_(...) PRAGMA_DIAGNOSTICS_(error __VA_ARGS__)
        #define IGNORE_(...) PRAGMA_DIAGNOSTICS_(ignored __VA_ARGS__)
        #define WARNING_(...) PRAGMA_DIAGNOSTICS_(warning __VA_ARGS__)
    
        #define DIAGNOSTICS_POP POP_(pop)
        #define DIAGNOSTICS_PUSH PUSH_(push)
    
        #define DIAGNOSTICS_ERROR(...) ERROR_(__VA_ARGS__)
        #define DIAGNOSTICS_IGNORE(...) IGNORE_(__VA_ARGS__)
        #define DIAGNOSTICS_IGNORED(...) IGNORE_(__VA_ARGS__)
        #define DIAGNOSTICS_WARNING(...) WARNING_(__VA_ARGS__)
    #else
        #define DIAGNOSTICS_POP
        #define DIAGNOSTICS_PUSH
    
        #define DIAGNOSTICS_ERROR(...)
        #define DIAGNOSTICS_IGNORE(...)
        #define DIAGNOSTICS_IGNORED(...)
        #define DIAGNOSTICS_WARNING(...)
    #endif
    
    DIAGNOSTICS_PUSH
    DIAGNOSTICS_IGNORE ( "-Weverything" )
        // #pragma GCC poison _
        #pragma GCC poison POP_
        #pragma GCC poison PUSH_
        #pragma GCC poison ERROR_
        #pragma GCC poison IGNORE_
        #pragma GCC poison WARNING_
    
        #pragma GCC poison _compiler__
        #pragma GCC poison _compiler_id__
    
        // #pragma GCC poison STRINGIFY_
        // #pragma GCC poison DIAGNOSTICS_STRINGIFY_
    
    /* poison the 'diagnostics.h' macro namespace */
        #pragma GCC poison DIAGNOSTICS_H /* DO NOT USE DIAGNOSTICS.H */
    DIAGNOSTICS_POP
    
    DIAGNOSTICS_PUSH
    DIAGNOSTICS_WARNING("-W#warnings")
        #warning "Warn: Header '<diagnostics.h>' has been supressed Ignore this warning if you are not using header '<diagnostics.h>'"
    DIAGNOSTICS_POP
#endif /* DIAGNOSTICS_HPP_ */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#pragma clang diagnostic pop
