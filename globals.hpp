/* #pragma once */

#ifndef GLOBALS_HPP_
    #define GLOBALS_HPP_
    
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
    
    #define LIB_ULIX_VERSION_ "v0.0.1"
    #define LIB_UNIQX_VERSION_ "v0.1.2"
    #define LIB_NULLIX_VERSION_ "v1.1.1"
    
    DIAGNOSTICS_PUSH /* silence nullib_detail_ attrs_ */
    DIAGNOSTICS_WARNING ( "-Wunknown-attributes" )
    
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
    
    namespace
        std::fs
    {
        using namespace ::std::filesystem;
    } /* namespace std::fs */
    
    namespace [[
        
        ]] std
    {
        using src_loc = ::std::source_location;
    } /* namespace std */
    
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
        -> std::optional <std::string>
        {
            
            #define HOSTNAME_MAX 1<<8
            
            if
                (
                    std::array <char , ( HOSTNAME_MAX ) + 1L> hostNm_ { };
                    ::gethostname ( hostNm_.data ( ) , hostNm_.size ( ) ) == 0L
                ) [[likely]]
            {
                return
                    { hostNm_.data ( ) }
                ;
            }
            
            else [[unlikely]]
            {
                return { std::nullopt };
            }
            
        }
        
    } /* namespace unistd */
    
    DIAGNOSTICS_POP
    
#endif /* GLOBALS_HPP_ */
