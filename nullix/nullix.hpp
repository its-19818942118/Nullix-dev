#ifndef NULLIX_HPP_NULLIX_
    #define NULLIX_HPP_NULLIX_
    
    #include <array>
    #include <string>
    #include <optional>
    #include <filesystem>
    #include <source_location>
    
    /* NULL Ctor Types */
    #define Null_Ctor_
    /* NULL Dtor Types */
    #define Null_Dtor_
    
    #define CLASS_CTOR Null_Ctor_
    #define CLASS_DTOR Null_Dtor_
    
    #define STRUCT_CTOR Null_Ctor_
    #define STRUCT_DTOR Null_Dtor_
    
    #define VERSION "v0.0.1"
    
    namespace std::fs {
        using namespace ::std::filesystem;
    } /* namespace std::fs */
    
    namespace std {
        using src_loc = ::std::source_location;
    } /* namespace std */
    
    namespace
        nullix::err
    {
        
        namespace [[
            deprecated
            ( "Please use namespace `nullix::err::v2` instead" )
        ]] v1
        {
            
            struct [[
                deprecated
                ( "Please use `nullix::err::v2::Error` instead" )
            ]] Error;
            
            struct [[
                deprecated
                ( "Please use `nullix::err::v2::ErrorCode` instead" )
            ]] ErrorInt;
            
        } /* namespace v1 */
        
    } /* namespace nullix::err */
    
    namespace
        nullix::err
    {
        
        namespace
            v2
        {
            
            class Error;
            class ErrorCode;
            
            using Err_t_ = Error;
            using ErrCode_t_ = ErrorCode;
            
        } /* namespace v2 */
        
    } /* namespace nullix::err */
    
    namespace
        nullix
    {
        
        /* base class for nullix related functionality */
        class Base;
        /* host class for user environment related functionality */
        class Host;
        
        /* type aliases for Error types */
        using Err_t_ = err::v2::Error;
        using ErrInt_t_ = err::v2::ErrorCode;
        
    } /* namespace nullix */
    
    ///> deprecated
    /*************************************************************************!
    @brief: Prevents circular dependencies.
    @details Define this macro to get only forward declared symbols of nullix ,
    * then undef to allow the ifndef condition to be true &
    * rid ourselves of circular dependencies.
    !*************************************************************************!
    * Usage Example:
    * ```cpp
    *     #define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
    *     #
    *     #   include "nullix.hpp"
    *     #
    *     #undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
    * ```
    !*************************************************************************/
    #if !defined(ONLY_FORWARD_SYMBOL_DECLS_NULLIX_)
    #   include "host/host.hpp"
    #   include "diagnostics/error.hpp"
    #   include "diagnostics/Diagnostics.hpp"
    #endif /* ONLY_FORWARD_SYMBOL_DECLS_NULLIX_ */
    
#endif /* NULLIX_HPP_NULLIX_ */

#if 0
#define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
#
#   include "nullix.hpp"
#
#undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
#endif
