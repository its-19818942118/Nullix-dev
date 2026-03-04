#include <format>
#include <cstdlib>
#include <expected>

#define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
#
#   include "nullix.hpp"
#
#undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_

#include "host/host.hpp"
#include "diagnostics/error.hpp"
#include "diagnostics/Diagnostics.hpp"
#include "system/file_tree/FileTree.hpp"


namespace
    nullix
{
    
    [[
        nodiscard
        ( "Important: returns k_username" )
    ]] auto
        Host::_mtGet_str_userName
        ( std::src_loc const k_sl_srcLoc_ )
    -> std::expected <std::string , Err_t_>
    {
        
        if
            ( auto const& k_username = std::getenv ( "USER" ) )
            [[likely]]
        {
            return { k_username };
        }
        
        else [[unlikely]]
        {
            
            return
                {
                    std::unexpected
                    (
                        Err_t_
                        {
                            Err_t_::e_ErrType::Fatal ,
                            "function `std::getenv(USER)` "
                            "provided by header <cstdlib> FAILED." ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
            
        }
    }
    
    [[
        nodiscard
        ( "Important: returns k_hostname" )
    ]] auto
        Host::_mtGet_str_hostName
        ( std::src_loc const k_sl_srcLoc_ )
    -> std::expected <std::string, Err_t_>
    {
        
        if
            ( auto const& k_hostname = unistd::gethostname ( ) )
        {
            
            return { *k_hostname };
            
        }
        
        return
            {
                
                std::unexpected
                (
                    Err_t_
                    {
                        Err_t_::e_ErrType::Fatal ,
                        "function `unistd::gethostname()` "
                        "provided by header <unistd.h> FAILED." ,
                        k_sl_srcLoc_
                    }
                )
                
            }
        ;
        
    }
    
    [[
        nodiscard
        ( "Important: returns formatted hostinfo string" )
    ]] auto
        Host::_mtGet_str_hostInfo
        ( void /* v_ */ ) const
    -> std::string
    {
        
        if
            ( pmk_str_userName.empty ( ) && pmk_str_hostName.empty ( ) )
        {
            
            return
                {
                    std::format
                    (
                        "{}@{}"
                        , this->_mtGet_str_userName ( ).error
                        ( ).mt_str_fallback ( "unknown_user" )
                        , this->_mtGet_str_hostName ( ).error
                        ( ).mt_str_fallback ( "unknown_host" )
                    )
                }
            ;
            
        }
        
        return
            {
                std::format
                (
                    "{}@{}"
                    , this->pmk_str_userName
                    , this->pmk_str_hostName
                )
            }
        ;
        
    }
    
    auto Host::
        mt_fsp_userHome
        ( void /* v_ */ )
    -> std::fs::path
    {
        
        if
            ( auto const& _userHome { std::getenv ( "HOME" ) } )
        {
            return { _userHome };
        }
        
        auto const&
            _user
            {
                Host::_mtGet_str_userName ( ).error ( ).mt_str_errLog_or
                ( "unknown_nullixUser" )
            }
        ;
        
        auto const& _fallback { std::fs::path { "/home" } / _user };
        
        return
            {
                std::fs::exists ( _fallback ) ? _fallback
                : std::fs::temp_directory_path ( ) / "unknown_nullixUser"
            }
        ;
        
    }
    
    CLASS_CTOR
        Host::Host
        ( void /* v_ */ )
        : pmk_str_userName
        (
            [ username { this->_mtGet_str_userName ( ) } ]
            {
                
                return
                    (
                        username
                        ? *username
                        : username.error ( ).mt_str_errLog_or
                        ( "unknown-user" )
                    )
                ;
                
            } ( )
        )
        , pmk_str_hostName
        (
            [ hostname { this->_mtGet_str_hostName ( ) } ]
            {
                return
                    (
                        hostname
                        ? *hostname
                        : hostname.error
                        ( ).mt_str_errLog_or
                        ( "unknown-host" )
                    )
                ;
                
            } ( )
        )
        , dirs ( uniqx::system::FileTree ( this->mt_fsp_userHome ( ) ) )
    { }
    
    CLASS_CTOR
        Host::Host
        ( std::fs::path const& kr_fsp_userHome_ )
        : pmk_str_userName
        (
            [ username { this->_mtGet_str_userName ( ) } ]
            {
                
                return
                    (
                        username
                        ? *username
                        : username.error ( ).mt_str_errLog_or
                        ( "unknown-user" )
                    )
                ;
                
            } ( )
        )
        , pmk_str_hostName
        (
            [ hostname { this->_mtGet_str_hostName ( ) } ]
            {
                return
                    (
                        hostname
                        ? *hostname
                        : hostname.error
                        ( ).mt_str_errLog_or
                        ( "unknown-host" )
                    )
                ;
                
            } ( )
        )
        , dirs ( kr_fsp_userHome_ )
    { }
    
    Host::~Host ( ) = default;
    
} /* namespace nullix */
