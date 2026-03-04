#include <print>
#include <expected>
#include <filesystem>
#include <string_view>
#include <system_error>
#include <source_location>

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

#include "diagnostics/error.hpp"
#include "file_tree/FileTree.hpp"
#include "file_tree/PathProxy.hpp"

namespace
    uniqx
{
    
    auto FileTree::
        PathProxy::perma
        ( uniqx::MutateGlobalState )
    -> PathProxy&
    {
        
        this->state_ref = this->current;
        
        return { *this };
        
    }
    
    auto FileTree::
        PathProxy::linkTo
        (
            const std::fs::path& k_ref_fsp_target_ ,
            const std::src_loc k_sl_srcLoc_
        ) const
    -> std::expected <PathProxy , nullix::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( !std::fs::exists ( this->current , _ec_errCode ) || _ec_errCode )
        {
            return
                {
                    std::unexpected <nullix::ErrInt_t_>
                    (
                        nullix::ErrInt_t_
                        {
                            long { 404L } ,
                            nullix::ErrInt_t_::e_ErrType::Fatal ,
                            /* _ec_errCode.message ( ) , */
                            "Path doesn't exist! The quick brown fox jumps over the lazy dogs!... The quick brown fox jumps over the lazy dogs!!!..." ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
        }
        
        if
            (
                std::fs::exists ( k_ref_fsp_target_ ) ||
                std::fs::is_symlink ( k_ref_fsp_target_ )
            )
        {
            std::println
                (
                    stderr ,
                    "[uniqx]: skipping target_path {} because it already exists"
                    , k_ref_fsp_target_.string ( )
                )
            ;
            return { *this };
        }
        
        std::fs::create_directories
            ( k_ref_fsp_target_.parent_path ( ) , _ec_errCode )
        ;
        
        if
            ( _ec_errCode )
        {
            return
                {
                    std::unexpected <nullix::ErrInt_t_>
                    (
                        nullix::ErrInt_t_
                        {
                            long { _ec_errCode.value ( ) } ,
                            nullix::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
        }
        
        _ec_errCode.clear ( );
        
        std::fs::path _fsp_tmpLink { k_ref_fsp_target_ };
        _fsp_tmpLink.replace_extension ( ".uniqx-nullix_tmpLink" );
        
        if
            ( std::fs::is_directory ( this->current ) )
        {
            std::fs::create_directory_symlink
                ( this->current , _fsp_tmpLink , _ec_errCode )
            ;
        }
        
        else
        {
            std::fs::create_symlink
                ( this->current , _fsp_tmpLink , _ec_errCode )
            ;
        }
        
        if
            ( _ec_errCode )
        {
            return
                {
                    std::unexpected <nullix::ErrInt_t_>
                    (
                        nullix::ErrInt_t_
                        {
                            long { _ec_errCode.value ( ) } ,
                            nullix::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
        }
        
        _ec_errCode.clear ( );
        
        std::fs::rename ( _fsp_tmpLink , k_ref_fsp_target_ , _ec_errCode );
        
        if
            ( _ec_errCode )
        {
            return
                {
                    std::unexpected <nullix::ErrInt_t_>
                    (
                        nullix::ErrInt_t_
                        {
                            long { _ec_errCode.value ( ) } ,
                            nullix::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
        }
        
        std::println
            (
                stderr ,
                "[uniqx]: synced target_path {}" ,
                k_ref_fsp_target_.string ( )
            )
        ;
        
        return { *this };
        
    }
    
    auto FileTree::
        PathProxy::unlink
        ( const std::src_loc k_sl_srcLoc_ ) const
    -> std::expected<PathProxy , nullix::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        const auto&
            k_b_isLink
            { std::fs::is_symlink ( this->current , _ec_errCode ) }
        ;
        
        if
            ( _ec_errCode )
        {
            return
                {
                    std::unexpected <nullix::ErrInt_t_>
                    (
                        nullix::ErrInt_t_
                        {
                            long { _ec_errCode.value ( ) } ,
                            nullix::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
        }
        
        if
            ( !k_b_isLink )
        {
            std::println
                (
                    stderr ,
                    "[uniqx.skip]: target: {} is not a symlink"
                    , this->current.string ( )
                )
            ;
            
            return { *this };
        }
        
        _ec_errCode.clear ( );
        
        if
            ( !std::fs::remove ( this->current , _ec_errCode ) || _ec_errCode )
        {
            
            return
                {
                    std::unexpected <nullix::ErrInt_t_>
                    (
                        nullix::ErrInt_t_
                        {
                            long { _ec_errCode.value ( ) } ,
                            nullix::ErrInt_t_::e_ErrType::Fatal ,
                            _ec_errCode.message ( ) ,
                            k_sl_srcLoc_
                        }
                    )
                }
            ;
            
        }
        
        std::println
            (
                stderr ,
                "[uniqx]: unlinked target leaf: {}"
                , this->current.string ( )
            )
        ;
        
        return { *this };
        
    }
    
    auto FileTree::
        PathProxy::prune
        ( const std::src_loc k_sl_srcLoc_ ) const
    -> std::expected<PathProxy , nullix::ErrInt_t_>
    {
        
        std::error_code _ec_errCode { };
        
        if
            ( const auto& k_res { this->unlink ( ) }; !k_res )
        {
            return { k_res };
        }
        
        for
            (
                auto _fsp_path { this->current.parent_path ( ) };
                _fsp_path.has_relative_path ( );
                _fsp_path = _fsp_path.parent_path ( )
            )
        {
            
            if
                (
                    _fsp_path == parent.home ( ) ||
                    _fsp_path == parent.cache ( ) ||
                    _fsp_path == parent.local ( ) ||
                    _fsp_path == parent.config ( ) ||
                    _fsp_path == parent.source ( )
                )
            {
                std::println
                    (
                        stderr ,
                        "[uniqx.skip]: target: {}"
                        , _fsp_path.string ( )
                    )
                ;
                
                std::println ( stderr , "{}" , _fsp_path.string() );
                break;
            }
            
            if
                (
                    !std::fs::is_directory
                    ( _fsp_path , _ec_errCode ) ||
                    _ec_errCode
                )
            {
                // return
                //     {
                //         std::unexpected <nullix::ErrInt_t_>
                //         (
                //             nullix::ErrInt_t_
                //             {
                //                 long { _ec_errCode.value ( ) } ,
                //                 nullix::ErrInt_t_::e_ErrType::Fatal ,
                //                 _ec_errCode.message ( ) ,
                //                 k_sl_srcLoc_
                //             }
                //         )
                //     }
                // ;
                std::println ( stderr , "{}" , _fsp_path.string() );
                break;
            }
            
            if
                (
                    bool empty_ = !std::fs::is_empty
                    ( _fsp_path , _ec_errCode );
                    _ec_errCode || empty_
                )
            {
                // return
                //     {
                //         std::unexpected <nullix::ErrInt_t_>
                //         (
                //             nullix::ErrInt_t_
                //             {
                //                 long { _ec_errCode.value ( ) } ,
                //                 nullix::ErrInt_t_::e_ErrType::Fatal ,
                //                 _ec_errCode.message ( ) ,
                //                 k_sl_srcLoc_
                //             }
                //         )
                //     }
                // ;
                std::println ( stderr , "{}" , _fsp_path.string() );
                break;
            }
            
            if
                (
                    std::fs::remove ( _fsp_path , _ec_errCode );
                    _ec_errCode
                )
            {
                return
                    {
                        std::unexpected <nullix::ErrInt_t_>
                        (
                            nullix::ErrInt_t_
                            {
                                long { _ec_errCode.value ( ) } ,
                                nullix::ErrInt_t_::e_ErrType::Fatal ,
                                _ec_errCode.message ( ) ,
                                k_sl_srcLoc_
                            }
                        )
                    }
                ;
            }
            
            std::println
                (
                    stderr ,
                    "[uniqx]: pruned empty target branch: {}"
                    , _fsp_path.string ( )
                )
            ;
            
        }
        
        return { *this };
        
    }
    
    auto FileTree::
        PathProxy::cleanBroken
        ( const std::src_loc k_sl_srcLoc_ ) const
    -> std::expected <PathProxy , nullix::ErrInt_t_>
    {
        
        if ( !std::fs::is_directory ( this->current ) ) return { *this };
        
        for
            (
                const auto& k_fsdir_entry :
                std::fs::directory_iterator ( this->current )
            )
        {
            
            const std::fs::path& k_ref_fsp_link { k_fsdir_entry.path ( ) };
            
            if ( !std::fs::is_symlink ( k_ref_fsp_link ) ) continue;
            
            std::error_code _ec_errCode { };
            
            std::fs::path
                _fsp_target
                { std::fs::read_symlink ( k_ref_fsp_link , _ec_errCode ) }
            ;
            
            if
                ( _ec_errCode )
            {
                
                return
                    {
                        std::unexpected <nullix::ErrInt_t_>
                        (
                            nullix::ErrInt_t_
                            {
                                long { _ec_errCode.value ( ) } ,
                                nullix::ErrInt_t_::e_ErrType::Fatal ,
                                _ec_errCode.message ( ) ,
                                k_sl_srcLoc_
                            }
                        )
                    }
                ;
                
            }
            
            if
                (
                    std::fs::path
                        _fsp_resolved
                        {
                            _fsp_target.is_absolute ( )
                            ? _fsp_target
                            : k_ref_fsp_link.parent_path ( ) / _fsp_target
                        }
                    ;
                    std::fs::exists ( _fsp_resolved )
                )
            {
                continue;
            }
            
            _ec_errCode.clear ( );
            std::fs::remove(k_ref_fsp_link, _ec_errCode);
            
            if
                ( _ec_errCode )
            {
                return
                    {
                        std::unexpected <nullix::ErrInt_t_>
                        (
                            nullix::ErrInt_t_
                            {
                                long { _ec_errCode.value ( ) } ,
                                nullix::ErrInt_t_::e_ErrType::Fatal ,
                                _ec_errCode.message ( ) ,
                                k_sl_srcLoc_
                            }
                        )
                    }
                ;
            }
            
            std::println
                (
                    stderr ,
                    "[uniqx]: Removed broken link: {}"
                    , k_ref_fsp_link.string ( )
                )
            ;
            
        }
        
        return { *this };
        
    }
    
    auto FileTree::
        PathProxy::string
        ( void /* v_ */ ) const
    -> std::string
    {
        
        return { current.string ( ) };
        
    }
    
    auto FileTree::
        PathProxy::operator /
        ( std::string_view strv_sub_ ) const
    -> PathProxy
    {
        
        return { this->parent , this->current / strv_sub_ , this->state_ref };
        
    }
    
    auto FileTree::
        PathProxy::operator >>=
        ( const std::fs::path& k_ref_fsp_linkTarget_ ) const
    -> std::expected<PathProxy , nullix::ErrInt_t_>
    {
        
        return this->linkTo ( k_ref_fsp_linkTarget_ );
        
    }
    
    FileTree::PathProxy::
        operator std::fs::path
        ( void /* v_ */ ) const
    {
        
        return { this->current };
        
    }
    
    FileTree::PathProxy::
        operator std::string
        ( void /* v_ */ ) const
    {
        
        return { this->current.string() };
        
    }
    
} /* namespace uniqx */
