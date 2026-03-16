#include <format>
#include <cstdlib>
#include <expected>

#define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_
#
#   include "nullix.hpp"
#
#undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_

#include "globals.hpp"
#include "host/host.hpp"
#include "error/error.hpp"
#include "filesystem/FileTree.hpp"
#include "diagnostics/Diagnostics.hpp"


namespace
    nullix
{
    
    [[
        nodiscard
        ( "Important: returns kr_username" )
    ]] auto
        Host::_mtGet_str_userName
        ( std::srcLoc_t_ const k_sl_srcLoc_ )
    -> std::expected <std::string , errika::Err_t_>
    {
        
        if
            ( auto const& kr_username = std::getenv ( "USER" ) )
            [[likely]]
        {
            return { kr_username };
        }
        
        else [[unlikely]]
        {
            
            return
                {
                    std::unexpected
                    (
                        errika::Err_t_
                        {
                            errika::Err_t_::e_ErrType::Fatal ,
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
        ( "Important: returns kr_hostname" )
    ]] auto
        Host::_mtGet_str_hostName
        ( std::srcLoc_t_ const k_sl_srcLoc_ )
    -> std::expected <std::string, errika::Err_t_>
    {
        
        if
            ( auto const& kr_hostname = unistd::gethostname ( ) )
        {
            
            return { *kr_hostname };
            
        }
        
        return
            {
                
                std::unexpected
                (
                    errika::Err_t_
                    {
                        errika::Err_t_::e_ErrType::Fatal ,
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
                        ( ).fallback ( "unknown_user" )
                        , this->_mtGet_str_hostName ( ).error
                        ( ).fallback ( "unknown_host" )
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
                Host::_mtGet_str_userName ( ).error
                ( ).logReport_and ( "unknown_nullixUser" )
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
    
    auto Host::userName
        ( void /* v_ */ ) const
        noexcept ( true )
    -> std::string
    {
        
        return
            {
                this->_mtGet_str_userName ( )
                ? *this->_mtGet_str_userName ( )
                : this->_mtGet_str_userName ( ).error
                ( ).logReport_and ( "unknown-user" )
            }
        ;
        
    }
    
    auto Host::hostName
        ( void /* v_ */ ) const
        noexcept ( true )
    -> std::string
    {
        
        return
            {
                this->_mtGet_str_hostName ( )
                ? *this->_mtGet_str_hostName ( )
                : this->_mtGet_str_hostName ( ).error
                ( ).logReport_and ( "unknown-host" )
            }
        ;
        
    }
    
    auto Host::hostInfo
        ( void /* v_ */ ) const
        noexcept ( true )
    -> std::string
    {
        
        return { this->_mtGet_str_hostInfo ( ) };
        
    }
    
    auto Host::userHome
        ( void /* v_ */ )
        noexcept ( true )
    -> std::fs::path
    {
        
        return { mt_fsp_userHome ( ) };
        
    }
    
    CLASS_CTOR
        Host::Host
        ( void /* v_ */ )
        : pmk_str_userName ( this->userName ( ) )
        , pmk_str_hostName ( this->hostName ( ) )
        , dirs ( this->userHome ( ) )
    { }
    
    CLASS_CTOR
        Host::Host
        ( std::fs::path const& kr_fsp_userHome_ )
        : pmk_str_userName ( this->userName ( ) )
        , pmk_str_hostName ( this->hostName ( ) )
        , dirs ( kr_fsp_userHome_ )
    { }
    
    Host::~Host ( ) = default;
    
} /* namespace nullix */
