#include <print>
#include <chrono>
#include <optional>
#include <functional>
#include <filesystem>
#include <system_error>
#include <source_location>

#include "filesystem/Path.hpp"
#include "globals.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{

    template <typename Tp_>
    using optRef_t_ = std::optional<std::reference_wrapper<Tp_>>;
    
    // namespace [[
    //         /* nullAttr_ */
    //     ]] filesystem
    // {
        
    //     auto safe_tmp_filename
    //         ( std::fs::path const& /* kr_fps_filePath_ */ )
    //         noexcept ( true )
    //     -> std::fs::path
    //     {
            
    //         return { };
            
    //     }
        
    // } /* namespace filesystem */
    
    auto filesystem::
        Path::parent
        ( void /* v_ */ ) const
        noexcept ( false )
    -> uniqx::fs::Path
    {
        
        return { std::move ( this->parent_path ( ) ) };
        
    }
    
    auto filesystem::
        Path::atomicLinkTo
        (
            std::fs::path const& kr_fsp_newDestPath_ ,
            std::src_loc const k_sl_srcLoc_
        ) const
        noexcept ( false )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( !std::fs::exists ( std::move ( *this ) , _ec_errCode ) || _ec_errCode )
        {
            
            return
                std::unexpected<errika::ErrInt_t_>
                {
                    errika::ErrInt_t_
                    {
                        long { 404L } ,
                        errika::ErrInt_t_::e_ErrType::Fatal ,
                        _ec_errCode.message ( ) ,
                        k_sl_srcLoc_
                    }
                }
            ;
            
        }
        
        if
            (
                std::fs::exists ( kr_fsp_newDestPath_ ) ||
                std::fs::is_symlink ( kr_fsp_newDestPath_ )
            )
        {
            
            std::println
                (
                    stderr ,
                    "::[ warn ]: ( uniqx::fs::ASLink )\n"
                    "{0:>4}Skipped syncing targetPath {1:?}.\n"
                    "{0:>4}Destination path {2:?} already exists!"
                    , "" , this->string ( ) , kr_fsp_newDestPath_.string ( )
                )
            ;
            
            return ( std::move ( *this ) );
            
        }
        
        
        if
            (
                bool const k_b_created
                    {
                        std::fs::create_directories
                        ( kr_fsp_newDestPath_.parent_path ( ) , _ec_errCode )
                    }
                ; _ec_errCode || k_b_created
            )
        {
            
            return
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
            ;
            
        }
        
        auto _fsp_tmpLink { kr_fsp_newDestPath_ };
        
        // _fsp_tmpLink.replace_extension ( ".ufP_ASLink" );
        // _fsp_tmpLink += ".ufP_ASLink";
        
        using namespace std::chrono;
        
        auto const
            k_khrClock_epochTimeSeed
            {
                static_cast<nanoseconds>
                ( steady_clock::now ( ).time_since_epoch ( ) )
            }
        ;
        
        _fsp_tmpLink += std::format
            ( ".ufP_ASLink.{:X}" , k_khrClock_epochTimeSeed.count ( ) )
        ;
        
        if
            ( std::fs::is_directory ( std::move ( *this ) , _ec_errCode ) )
        {
            
            std::fs::create_directory_symlink
                ( std::move ( *this ) , _fsp_tmpLink , _ec_errCode )
            ;
            
        }
        
        else
        {
            
            std::fs::create_symlink
                ( std::move ( *this ) , _fsp_tmpLink , _ec_errCode )
            ;
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
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
            ;
            
        }
        
        std::fs::rename ( _fsp_tmpLink , kr_fsp_newDestPath_ , _ec_errCode );
        
        if
            ( _ec_errCode )
        {
            
            return
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
        
        return ( std::move ( *this ) );
        
    }
    
    auto filesystem::
        Path::unlink
        (
            std::src_loc const k_sl_srcLoc_
        ) const
        noexcept ( false )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        auto const k_b_isSLink { std::fs::exists ( std::move ( *this ) , _ec_errCode ) };
        
        if
            ( _ec_errCode )
        {
            
            return
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
            
            return ( std::move ( *this ) );
            
        }
        
        _ec_errCode.clear ( );
        
        if
            ( !std::fs::remove ( std::move ( *this ) , _ec_errCode ) || _ec_errCode )
        {
            
            return
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
        
        return ( std::move ( *this ) );
        
    }
    
    auto filesystem::
        Path::prune
        (
            iLfsp_t_ iLfsp_boundaries_ ,
            std::src_loc const k_sl_srcLoc_
        ) const
        noexcept ( false )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( auto const& k_res { this->unlink ( ) }; !k_res )
        {
            
            return ( k_res );
            
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
                ;
                
            }
            
        }
        
        if
            ( _ec_errCode )
        {
            
            return
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
            ;
            
        }
        
        std::println
            (
                stderr ,
                "::[ info ]: ( uniqx::fs::prune )"
                ""
            )
        ;
        
        return ( std::move ( *this ) );
        
    }
    
    auto filesystem::
        Path::operator /
        ( std::fs::path const& kr_fsp_subDir_ ) const
        noexcept ( false )
    -> uniqx::fs::Path
    {
        
        Path _ufP_presentPath { std::move ( std::move ( *this ) ) };
        
        _ufP_presentPath /= std::move ( kr_fsp_subDir_ );
        
        return ( _ufP_presentPath );
        
    }
    
    // auto filesystem::
    //     Path::operator /
    //     ( uniqx::fs::Path const& kr_ufP_subDir_ ) const
    //     noexcept ( false )
    // -> uniqx::fs::Path
    // {
        
    //     Path _ufP_presentPath { std::move ( *this ) };
        
    //     _ufP_presentPath /= kr_ufP_subDir_.native ( );
        
    //     return ( _ufP_presentPath );
        
    // }
    
    // auto filesystem::
    //     Path::operator /
    //     ( std::string_view Sv_subDir_ ) const
    //     noexcept ( false )
    // -> uniqx::fs::Path
    // {
        
    //     Path _ufP_presentPath { std::move ( *this ) };
        
    //     _ufP_presentPath /= Sv_subDir_;
        
    //     return ( _ufP_presentPath );
        
    // }
    
    auto filesystem::
        Path::operator >>=
        (
            std::fs::path const& kr_ufP_newDestPath_
        ) const
        noexcept ( false )
    -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>
    {
        
        return ( this->atomicLinkTo ( std::move ( kr_ufP_newDestPath_ ) ) );
        
    }
    
} /* namespace uniqx */
