#include <print>
#include <utility>
#include <expected>
#include <optional>
#include <functional>
#include <filesystem>
#include <system_error>

#include "globals.hpp"
#include "error/error.hpp"
#include "filesystem/Path.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{

    template <typename Tp_>
    using optRef_t_ = std::optional<std::reference_wrapper<Tp_>>;
    
    auto filesystem::
        Path::parent
        ( void /* v_ */ ) const
        noexcept ( true )
    -> uniqx::fs::Path
    {
        
        return { this->parent_path ( ) };
        
    }
    
    auto filesystem::
        Path::atomicLinkTo
        (
            std::fs::path const& kr_fsp_newDestPath_ ,
            std::srcLoc_t_ const k_sl_srcLoc_
        ) const
        noexcept ( true )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( !std::fs::exists ( *this , _ec_errCode ) || _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode ? _ec_errCode.value ( ) : 404L ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode ? _ec_errCode.message ( )
                            : "File not found" ,
                            k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        auto const& _fsp_tmpLinkPath { kr_fsp_newDestPath_.string ( ) + "ufP_ASLink_" };
        
        if
            ( std::fs::is_directory ( *this , _ec_errCode ) )
        {
            
            std::fs::create_directory_symlink
                ( *this , _fsp_tmpLinkPath , _ec_errCode )
            ;
            
        }
        
        else
        {
            
            std::fs::create_symlink ( *this , _fsp_tmpLinkPath , _ec_errCode );
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        std::fs::rename
            ( _fsp_tmpLinkPath , kr_fsp_newDestPath_ , _ec_errCode )
        ;
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        if
            ( std::fs::exists ( _fsp_tmpLinkPath , _ec_errCode ) )
        {
            
            std::fs::remove_all ( _fsp_tmpLinkPath , _ec_errCode );
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs::ASLink )\n"
                "{0:>4}Successfully synced target/s!"
                , ""
            )
        ;
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs ) Synced target paths!\n"
                "{0:>4}From: target path {1:?} to destination path {2:?}\n"
                "{0:>4}target path {1:?} -> destination path {2:?}"
                , "" , this->string ( ) , kr_fsp_newDestPath_.string ( )
            )
        ;
        
        return { *this };
        
    }
    
    auto filesystem::
        Path::backupAndLinkTo
        (
            std::fs::path const& kr_fsp_newDestPath_ ,
            // std::fs::path const& kr_fsp_backupPath_ ,
            std::srcLoc_t_ const k_sl_srcLoc_
        ) const
        noexcept ( true )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( !std::fs::exists ( *this , _ec_errCode ) || _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode ? _ec_errCode.value ( ) : 404L ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode ? _ec_errCode.message ( )
                            : "File not found" ,
                            k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        auto const& _fsp_tmpLinkPath { kr_fsp_newDestPath_.string ( ) + ".ufP_ASLink_" };
        
        if
            ( std::fs::is_directory ( *this , _ec_errCode ) )
        {
            
            if
                ( std::fs::is_symlink ( kr_fsp_newDestPath_ , _ec_errCode ) )
            {
                
                std::fs::remove ( kr_fsp_newDestPath_ , _ec_errCode );
                
            }
            
            _ec_errCode.clear ( );
            
            if
                ( _ec_errCode )
            {
                
                return
                    {
                        
                        std::unexpected<errika::ErrInt_t_>
                        {
                            errika::ErrInt_t_
                            {
                                _ec_errCode.value ( ) ,
                                errika::ErrInt_t_::e_ErrType::Fatal ,
                                _ec_errCode.message ( ) , k_sl_srcLoc_
                            }
                        }
                        
                    }
                ;
                
            }
            
            std::fs::create_directory_symlink
                ( *this , _fsp_tmpLinkPath , _ec_errCode )
            ;
            
        }
        
        else
        {
            
            std::fs::create_symlink
                ( *this , _fsp_tmpLinkPath , _ec_errCode )
            ;
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        bool _b_destExists { };
        
        auto _fsp_nlxBackup { kr_fsp_newDestPath_};
        _fsp_nlxBackup += ( "_ufP_BASLink_" );
        
        if
            ( std::filesystem::exists ( kr_fsp_newDestPath_ ) )
        {
            
            _b_destExists = true;
            
            std::fs::rename
                ( kr_fsp_newDestPath_ , _fsp_nlxBackup , _ec_errCode )
            ;
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        std::fs::rename ( _fsp_tmpLinkPath , kr_fsp_newDestPath_ , _ec_errCode );
        
        if
            ( _ec_errCode )
        {

            if
                ( _b_destExists )
            {
                
                std::fs::rename
                    ( _fsp_nlxBackup , kr_fsp_newDestPath_ , _ec_errCode )
                ;
                
                std::fs::remove ( _fsp_tmpLinkPath );
                
            }
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        std::println ( "{}" , _fsp_tmpLinkPath );
        std::fs::remove ( _fsp_tmpLinkPath );
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs::ASLink )\n"
                "{0:>4}Successfully synced target/s!"
                , ""
            )
        ;
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs ) Synced target paths!\n"
                "{0:>4}From: target path {1:?} to destination path {2:?}\n"
                "{0:>4}target path {1:?} -> destination path {2:?}"
                , "" , this->string ( ) , kr_fsp_newDestPath_.string ( )
            )
        ;
        
        return { *this };
        
    }
    
    auto filesystem::
        Path::unlink
        (
            std::srcLoc_t_ const k_sl_srcLoc_
        ) const
        noexcept ( true )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        auto const k_b_isSLink { std::fs::exists ( *this , _ec_errCode ) };
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        if
            ( !k_b_isSLink )
        {
            
            std::println
                (
                    stderr ,
                    "::[ warn ]: ( uniqx::fs )\n"
                    "{0:>4}Failed to unlink target {1:?}\n"
                    "{0:>4}Target is not a symlink!"
                    , "" , this->string ( )
                )
            ;
            
            return { *this };
            
        }
        
        _ec_errCode.clear ( );
        
        if
            ( !std::fs::remove ( *this , _ec_errCode ) || _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) , k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs::unlink() )\n"
                "{0:>4}Unlinked target {1:?} successfully!"
                , "" , this->string ( )
            )
        ;
        
        return { *this };
        
    }
    
    auto filesystem::
        Path::prune
        (
            iLfsp_t_ iLfsp_boundaries_ ,
            std::srcLoc_t_ const k_sl_srcLoc_
        ) const
        noexcept ( true )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( auto const k_res { this->unlink ( ) }; !k_res )
        {
            
            return
                {
                    std::unexpected<errika::ErrInt_t_>
                    { std::move ( k_res.error ( ) ) }
                }
            ;
            
        }
        
        for
            (
                auto kr_ufP_path_ { this->parent ( ) };
                kr_ufP_path_.has_relative_path ( );
                kr_ufP_path_ = kr_ufP_path_.parent ( )
            )
        {
            
            optRef_t_<std::fs::path const> _ofsp_match;
            bool k_b_isBoundary { };
            
            
            for
                ( auto const& kr_ufP_boundary_ : iLfsp_boundaries_ )
            {
                
                if
                    (
                        (
                            std::fs::equivalent
                            (
                                kr_ufP_path_ ,
                                kr_ufP_boundary_ , _ec_errCode
                            ) ||
                            kr_ufP_path_ == kr_ufP_boundary_
                        ) && !_ec_errCode
                    )
                {
                    
                    k_b_isBoundary = true;
                    _ofsp_match = std::cref ( kr_ufP_boundary_ );
                    
                    break;
                    
                }
                
                if ( _ec_errCode ) _ec_errCode.clear ( );
                
            }
            
            if
                ( _ec_errCode )
            {
                
                return
                    {
                        
                        std::unexpected<errika::ErrInt_t_>
                        {
                            errika::ErrInt_t_
                            {
                                _ec_errCode.value ( ) ,
                                errika::ErrInt_t_::e_ErrType::Fatal ,
                                _ec_errCode.message ( ) ,
                                k_sl_srcLoc_
                            }
                        }
                        
                    }
                ;
                
            }
            
            if
                ( k_b_isBoundary )
            {
                
                std::println
                    (
                        stderr ,
                        "::[ info ]: ( uniqx::fs::prune() )\n"
                        "{0:>4}Stopping prune at boundary {1:?}"
                        , "" , _ofsp_match->get ( ).string ( )
                    )
                ;
                
                break;
                
            }
            
            if
                (
                    !std::fs::is_directory ( kr_ufP_path_ , _ec_errCode ) ||
                    _ec_errCode
                )
            {
                break;
            }
            
            if
                (
                    !std::fs::is_empty ( kr_ufP_path_ , _ec_errCode ) ||
                    _ec_errCode
                )
            {
                break;
            }
            
            if
                (
                    std::fs::remove ( kr_ufP_path_ , _ec_errCode );
                    _ec_errCode
                )
            {
                
                return
                    {
                        
                        std::unexpected<errika::ErrInt_t_>
                        {
                            errika::ErrInt_t_
                            {
                                _ec_errCode.value ( ) ,
                                errika::ErrInt_t_::e_ErrType::Fatal ,
                                _ec_errCode.message ( ) ,
                                k_sl_srcLoc_
                            }
                        }
                        
                    }
                ;
                
            }
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    {
                        errika::ErrInt_t_
                        {
                            _ec_errCode.value ( ) ,
                            errika::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    }
                    
                }
            ;
            
        }
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs::prune )"
                ""
            )
        ;
        
        return { *this };
        
    }
    
    auto filesystem::
        Path::operator /
        ( std::fs::path const& kr_fsp_subDir_ ) const
        noexcept ( true )
    -> uniqx::fs::Path
    {
        
        Path _ufP_presentPath { *this };
        
        _ufP_presentPath /= kr_fsp_subDir_;
        
        return { _ufP_presentPath };
        
    }
    
    // auto filesystem::
    //     Path::operator /
    //     ( uniqx::fs::Path const& kr_ufP_subDir_ ) const
    //     noexcept ( false )
    // -> uniqx::fs::Path
    // {
        
    //     Path _ufP_presentPath { *this };
        
    //     _ufP_presentPath /= kr_ufP_subDir_.native ( );
        
    //     return { _ufP_presentPath };
        
    // }
    
    // auto filesystem::
    //     Path::operator /
    //     ( std::string_view Sv_subDir_ ) const
    //     noexcept ( false )
    // -> uniqx::fs::Path
    // {
        
    //     Path _ufP_presentPath { *this };
        
    //     _ufP_presentPath /= Sv_subDir_;
        
    //     return { _ufP_presentPath };
        
    // }
    
    auto filesystem::
        Path::operator >>=
        (
            std::fs::path const& kr_fsp_newDestPath_
        ) const
        noexcept ( true )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        if
            (
                auto const k_res = this->atomicLinkTo ( kr_fsp_newDestPath_ );
                !k_res
            )
        {
            
            return
                {
                    
                    std::unexpected<errika::ErrInt_t_>
                    { std::move ( k_res.error ( ) ) }
                    
                }
            ;
            
        }
        
        else
        {
            return { std::move ( k_res.value ( ) ) };
        }
        
    }
    
} /* namespace uniqx */
