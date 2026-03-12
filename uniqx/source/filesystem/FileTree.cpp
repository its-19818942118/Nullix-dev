#include "host/host.hpp"
#include "filesystem/FileTree.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    CLASS_CTOR filesystem::
        FileTree::FileTree
            ( void /* v_ */ )
        : PMm_ufP_actv_userHome
        ( std::move ( nullix::Host::userHome ( ) ) )
    {
        this->sync_all ( );
    }
    
    CLASS_CTOR filesystem::FileTree::FileTree
            ( std::fs::path const& kr_fsp_userHome_ )
        : PMm_ufP_actv_userHome ( std::move ( kr_fsp_userHome_ ) )
    {
        this->sync_all ( );
    }
    
    // CLASS_CTOR filesystem::FileTree::FileTree
    //         ( uniqx::fs::Path const& kr_ufP_userHome_ )
    //     : PMm_ufP_actv_userHome ( std::move ( kr_ufP_userHome_ ) )
    // {
    //     this->sync_all ( );
    // }
    
    auto filesystem::
        FileTree::home
        ( void /* v_ */ ) const
        noexcept ( false )
    -> filesystem::Path
    {
        
        return ( std::move(this->PMm_ufP_actv_userHome) );
        
    }
    
    auto filesystem::
        FileTree::config
        ( void /* v_ */ ) const
        noexcept ( false )
    -> filesystem::Path
    {
        
        return ( std::move(this->PMm_ufP_usrConfigRoot) );
        
    }
    
    auto filesystem::
        FileTree::local
        ( void /* v_ */ ) const
        noexcept ( false )
    -> filesystem::Path
    {
        
        return ( std::move(this->PMm_ufP_usrLocalRoot) );
        
    }
    
    auto filesystem::
        FileTree::cache
        ( void /* v_ */ ) const
        noexcept ( false )
    -> filesystem::Path
    {
        
        return ( std::move(this->PMm_ufP_usrCacheRoot) );
        
    }
    
    auto filesystem::FileTree::source
        ( optSv_t_ const k_oSv_ovrSource_ ) const
        noexcept ( false )
    -> filesystem::Path
    {
        
        if
            ( k_oSv_ovrSource_.has_value ( ) )
        {
            
            return ( std::move(k_oSv_ovrSource_.value ( )) );
            
        }
        
        return ( std::move(this->PMm_ufP_usrSourceRoot) );
        
    }
    
    auto filesystem::
        FileTree::sync_all
        ( void /* v_ */ )
        noexcept ( false )
    -> void
    {
        
        this->PMm_ufP_usrCacheRoot = this->PMm_ufP_actv_userHome / ".cache";
        this->PMm_ufP_usrLocalRoot = this->PMm_ufP_actv_userHome / ".local";
        this->PMm_ufP_usrConfigRoot = this->PMm_ufP_actv_userHome / ".config";
        this->PMm_ufP_usrSourceRoot = this->PMm_ufP_actv_userHome / ".nullix";
        
    }
    
} /* namespace uniqx */
