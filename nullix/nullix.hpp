#ifndef NULLIX_HPP_NULLIX_
    #define NULLIX_HPP_NULLIX_
    
    /* IWYU pragma: begin_exports */
    #include <filesystem>
    #include <source_location>
    /* IWYU pragma: end_exports */
    
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
    
    namespace
        nullix
    {
        
        /* base class for nullix related functionality */
        class Base;
        /* host class for user environment related functionality */
        class Host;
        
    } /* namespace nullix */
    
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
    # /* IWYU pragma: begin_exports */
    #   include "host/host.hpp"
    #   include "diagnostics/Diagnostics.hpp"
    # /* IWYU pragma: end_exports */
    #endif /* ONLY_FORWARD_SYMBOL_DECLS_NULLIX_ */
    
#endif /* NULLIX_HPP_NULLIX_ */

#if 0
#define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
#
#   include "nullix.hpp"
#
#undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
#endif
