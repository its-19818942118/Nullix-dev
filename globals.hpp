/* #pragma once */

#ifndef GLOBALS_HPP_
    #define GLOBALS_HPP_
    
    #include <sys/types.h>
#include <unistd.h>
    #include <filesystem> /* IWYU pragma: export */
    #include <source_location>
    
    #include "diagnostics/Diagnostics.hpp"
    
    #define Null_Ctor_ /* NULL Ctor Types */
    #define Null_Dtor_ /* NULL Dtor Types */
    
    #define CLASS_CTOR Null_Ctor_
    #define CLASS_DTOR Null_Dtor_
    
    #define STRUCT_CTOR Null_Ctor_
    #define STRUCT_DTOR Null_Dtor_
    
    #define LIB_UNIQX_VERSION_ "v0.0.3"
    #define LIB_NULLIB_VERSION_ "v0.0.4"
    #define LIB_NULLIX_VERSION_ "v0.0.5"
    
    DIAGNOSTICS_PUSH /* silence nullib_detail_ attrs_ */
    DIAGNOSTICS_IGNORED ( "-Wunknown-attributes" )
    
    inline namespace [[
            
        ]] nullib_detail_
    {
        
        auto
            inline null_
            ( void /* v_ */ )
        ;
        
        auto
          inline annotate_
            ( char const* )
        -> void;
        
        auto inline
            documentation_
            ( char const* )
        -> void;
        
    }
    
    namespace [[
            /* nullAttr_ */
        ]] std
    {
        
        using size_t_ = signed long;
        
        using str_t_ = ::std::string;
        using strv_t_ = ::std::string_view;
        using srcLoc_t_ = ::std::source_location;
        
    } /* namespace std */
    
    namespace
        std::fs
    {
        using namespace ::std::filesystem;
    } /* namespace std::fs */
    
    /**************************************************************************
    @brief: wrapping unistd header functions, cause why not? namespaces are
    * cool
    **************************************************************************/
    inline namespace [[
            nullib_detail_::documentation_
            ( "Wrap the gethostname unistd function to `unistd::` namespace" )
        ]] unistd
    {
        
        auto inline
            gethostname
            ( void /* v_ */ )
        -> std::optional<std::string>
        {
            
            #define HOSTNAME_MAX +1 << +8
            
            if
                (
                    std::array<char , ( HOSTNAME_MAX ) + ( +1L )> hstNm_ { };
                    ::gethostname ( hstNm_.data ( ) , hstNm_.size ( ) ) == +0L
                ) [[ likely ]]
            {
                return
                    { hstNm_.data ( ) }
                ;
            }
            
            else [[ unlikely ]]
            {
                return { std::nullopt };
            }
            
        }
        
    } /* namespace unistd */
    
    DIAGNOSTICS_POP
    
#endif /* GLOBALS_HPP_ */
