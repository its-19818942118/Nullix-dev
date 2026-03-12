/* #pragma once */

#ifndef FILE_TREE_HPP_UNIQX_
    #define FILE_TREE_HPP_UNIQX_
    
    #include <optional>
#define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "filesystem/Path.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] filesystem::FileTree
            {
                
                private: mutable uniqx::fs::Path PMm_ufP_actv_userHome;
                
                /* permanent directory roots */
                private: mutable uniqx::fs::Path PMm_ufP_usrCacheRoot;
                private: mutable uniqx::fs::Path PMm_ufP_usrLocalRoot;
                private: mutable uniqx::fs::Path PMm_ufP_usrConfigRoot;
                /* source for uniqx's dotfiles */
                private: mutable uniqx::fs::Path PMm_ufP_usrSourceRoot;
                
                private: using stringV_t_ = std::string_view;
                private: using optSv_t_ = std::optional<stringV_t_>;
                
                public: explicit
                    CLASS_CTOR FileTree
                    ( void /* v_ */ )
                    noexcept ( false )
                CLASS_CTOR;
                
                public: explicit
                    CLASS_CTOR FileTree
                    ( std::fs::path const& /* kr_fsp_userHome_ */ )
                    noexcept ( false )
                CLASS_CTOR;
                
                // public: explicit
                //     CLASS_CTOR FileTree
                //     ( filesystem::Path const& /* kr_ufP_userHome_ */ )
                //     noexcept ( false )
                // CLASS_CTOR;
                
                public: auto home
                    ( void /* v_ */ ) const
                    noexcept ( false )
                -> filesystem::Path;
                
                public: auto config
                    ( void /* v_ */ ) const
                    noexcept ( false )
                -> filesystem::Path;
                
                public: auto local
                    ( void /* v_ */ ) const
                    noexcept ( false )
                -> filesystem::Path;
                
                public: auto cache
                    ( void /* v_ */ ) const
                    noexcept ( false )
                -> filesystem::Path;
                
                public: auto source
                    (
                        std::optional<std::string_view> const /* k_opt_ovr_ */ =
                        { std::nullopt }
                    ) const
                    noexcept ( false )
                -> filesystem::Path;
                
                public: auto sync_all
                    ( void /* v_ */ )
                    noexcept ( false )
                -> void;
                
            } /* class filesystem::FileTree */
        ;
        
    } /* namespace uniqx */
    
#endif /* FILE_TREE_HPP_UNIQX_ */
