#include <filesystem>

#define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
#
#   include "uniqx.hpp"
#
#undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_

#include "host/host.hpp"
#include "file_tree/FileTree.hpp"
#include "file_tree/PathProxy.hpp"

namespace
    uniqx
{
    
    auto
        system::FileTree::home
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_actv_userHome , this->PM_fsp_actv_userHome }
        ;
        
    }
    
    auto
        system::FileTree::config
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_usrConfigRoot , this->PM_fsp_usrConfigRoot }
        ;
        
    }
    
    auto
        system::FileTree::cache
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_usrCacheRoot , this->PM_fsp_usrCacheRoot }
        ;
        
    }
    
    auto
        system::FileTree::local
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_usrLocalRoot , this->PM_fsp_usrLocalRoot }
        ;
        
    }
    
    auto
        system::FileTree::source
        (
            [[maybe_unused]]
            std::optional<std::string_view> const k_opt_ovr_
        )
    -> PathProxy
    {
        
        if
            (
                // const std::fs::path& kr_fsp_src_ { k_opt_ovr_.value ( ) };
                k_opt_ovr_.has_value ( )
            )
        {
            return
                { *this , k_opt_ovr_.value( ) , k_opt_ovr_.value( ) }
            ;
        }
        
        return
            { *this , this->PM_fsp_usrSourceRoot , this->PM_fsp_usrSourceRoot }
        ;
        
    }
    
    auto
        system::FileTree::sync_all
        ( void /* v_ */ )
    -> void
    {
        
        this->PM_fsp_usrCacheRoot = this->PM_fsp_actv_userHome / ".cache";
        this->PM_fsp_usrLocalRoot = this->PM_fsp_actv_userHome / ".local";
        this->PM_fsp_usrConfigRoot = this->PM_fsp_actv_userHome / ".config";
        this->PM_fsp_usrSourceRoot = this->PM_fsp_actv_userHome / ".nullix";
        
    }
    
    // will implment later
    CLASS_CTOR
        system::FileTree::FileTree
            ( void /* v_ */ )
        : PM_fsp_actv_userHome ( nullix::Host::mt_fsp_userHome ( ) )
        , kPM_fsp_fctr_userHome ( nullix::Host::mt_fsp_userHome ( ) )
    {
        
        this->sync_all ( );
        
    }
    
    CLASS_CTOR
        system::FileTree::FileTree
            ( std::fs::path const& kr_fsp_userHome_ )
        : PM_fsp_actv_userHome ( kr_fsp_userHome_ )
        , kPM_fsp_fctr_userHome ( kr_fsp_userHome_ )
    {
        
        this->sync_all ( );
        
    }
    
} /* namespace nullix */
