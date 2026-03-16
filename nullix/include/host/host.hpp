/* #pragma once */

#ifndef HOST_HPP_NULLIX_
    #define HOST_HPP_NULLIX_
    
    #include "globals.hpp"
#include <expected>
    #include <filesystem>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
    #
    #   include "nullix.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "error/error.hpp"
    #include "filesystem/FileTree.hpp"
    
    namespace
        nullix
    {
        
        class
            Host
            {
                /* variable to store the user name */
                public: std::string const pmk_str_userName { };
                
                /* variable to store the host name */
                public: std::string const pmk_str_hostName { };
                
                public: uniqx::fs::FileTree dirs;
                
                private: [[
                    nodiscard
                    ( "Important: returns kr_username" )
                ]] auto static
                    _mtGet_str_userName
                    (
                        std::srcLoc_t_ const /* k_sl_srcLoc_ */ =
                        { std::srcLoc_t_::current ( ) }
                    )
                -> std::expected <std::string , errika::Err_t_>;
                
                private: [[
                    nodiscard
                    ( "Important: returns k_hostname" )
                ]] auto static
                    _mtGet_str_hostName
                    (
                        std::srcLoc_t_ const /* k_sl_srcLoc_ */ =
                        { std::srcLoc_t_::current ( ) }
                    )
                -> std::expected <std::string, errika::Err_t_>;
                
                private: auto
                    _mtGet_str_hostInfo
                    ( void /* v_ */ ) const
                -> std::string;
                
                private: auto static
                    mt_fsp_userHome
                    ( void /* v_ */ )
                -> std::fs::path;
                
                public: auto userName
                    ( void /* v */ ) const
                    noexcept ( true )
                -> std::string;
                
                public: auto hostName
                    ( void /* v */ ) const
                    noexcept ( true )
                -> std::string;
                
                public: auto hostInfo
                    ( void /* v */ ) const
                    noexcept ( true )
                -> std::string;
                
                public: auto static userHome
                    ( void /* v */ )
                    noexcept ( true )
                -> std::fs::path;
                
                public: explicit
                    CLASS_CTOR Host
                    ( void /* v_ */ )
                ;
                
                public: explicit
                    CLASS_CTOR Host
                    ( std::fs::path const& /* kr_fsp_userHome_ */ )
                ;
                
                public:
                    CLASS_DTOR ~Host
                    ( void /* v_ */ )
                ;
                
            }
        ;
        
    } /* namespace nullix */
    
#endif /* HOST_HPP_NULLIX_ */
