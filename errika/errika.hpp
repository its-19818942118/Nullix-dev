/* #pragma once */

#ifndef ERRIKA_HPP_ERRIKA_
    #define ERRIKA_HPP_ERRIKA_
    
    #include "globals.hpp"
    
    #include <source_location>
    
    namespace std {
        using src_loc = ::std::source_location;
    } /* namespace std */
    
    namespace errika::error
    {
        
        class Error;
        class ErrorCode;
        
        using Err_t_ = Error;
        using ErrCode_t_ = ErrorCode;
        
    } /* namespace errika */
    
    namespace [[
            /* nullAttr_ */
        ]] errika
    {
        
        /* type aliases for Error types */
        using Err_t_ = error::Err_t_;
        using ErrInt_t_ = error::ErrCode_t_;
        
    } /* namespace errika */
    
    /*************************************************************************!
    @brief: Prevents circular dependencies.
    @details: #define the macro to get only forward declared symbols of errika,
    * then #undef to allow the #ifndef condition to be true rid ourselves of
    * circular dependencies.
    !*************************************************************************!
    * Usage Example:
    * ```cpp
    *     #define ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
    *     #
    *     #   include "errika/errika.hpp"
    *     #
    *     #undef ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
    * ```
    !*************************************************************************/
    #if !defined(ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_)
    # /* IWYU pragma: begin_exports */
    #   include "error/error.hpp"
    # /* IWYU pragma: end_exports */
    #endif /* ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_ */
    
#endif /* ERRIKA_HPP_ERRIKA_ */

#if 0
    #define ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
    #
    #   include "errika/errika.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
#endif
