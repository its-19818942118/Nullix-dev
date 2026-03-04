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
        FileTree::home
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_actv_userHome , this->PM_fsp_actv_userHome }
        ;
        
    }
    
    auto
        FileTree::config
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_usrConfigRoot , this->PM_fsp_usrConfigRoot }
        ;
        
    }
    
    auto
        FileTree::cache
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_usrCacheRoot , this->PM_fsp_usrCacheRoot }
        ;
        
    }
    
    auto
        FileTree::local
        ( void /* v_ */ )
    -> PathProxy
    {
        
        return
            { *this , this->PM_fsp_usrLocalRoot , this->PM_fsp_usrLocalRoot }
        ;
        
    }
    
    auto
        FileTree::source
        (
            [[maybe_unused]]
            const std::optional<std::string_view> opt_ovr_
        )
    -> PathProxy
    {
        
        if
            (
                // const std::fs::path& k_ref_fsp_src_ { opt_ovr_.value ( ) };
                opt_ovr_.has_value ( )
            )
        {
            return
                { *this , opt_ovr_.value( ) , opt_ovr_.value( ) }
            ;
        }
        
        return
            { *this , this->PM_fsp_usrSourceRoot , this->PM_fsp_usrSourceRoot }
        ;
        
    }
    
    auto
        FileTree::sync_all
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
        FileTree::FileTree
            ( void /* v_ */ )
        : PM_fsp_actv_userHome ( nullix::Host::mt_fsp_userHome ( ) )
        , kPM_fsp_fctr_userHome ( nullix::Host::mt_fsp_userHome ( ) )
    {
        
        this->sync_all ( );
        
    }
    
    CLASS_CTOR
        FileTree::FileTree
            ( const std::fs::path& k_ref_fsp_userHome_ )
        : PM_fsp_actv_userHome ( k_ref_fsp_userHome_ )
        , kPM_fsp_fctr_userHome ( k_ref_fsp_userHome_ )
    {
        
        this->sync_all ( );
        
    }
    
} /* namespace nullix */
