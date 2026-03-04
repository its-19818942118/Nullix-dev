/* #pragma once */

#ifndef PATH_PROXY_HPP_UNIQX_
    #define PATH_PROXY_HPP_UNIQX_
    
    #include <string>
    #include <expected>
    #include <filesystem>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "diagnostics/error.hpp"
    #include "file_tree/FileTree.hpp"
    
    namespace
        uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] system::FileTree::PathProxy
            {
                
                public: FileTree& parent;
                
                public: std::fs::path current;
                public: std::fs::path state_ref;
                
                public: auto linkTo
                    (
                        std::fs::path const& /* kr_fsp_target_ */ ,
                        std::src_loc const /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: auto unlink
                    (
                        std::src_loc const /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: auto prune
                    (
                        std::src_loc const /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: auto cleanBroken
                    (
                        std::src_loc const /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected <PathProxy , nullix::ErrInt_t_>;
                
                public: auto perma
                    ( uniqx::MutateGlobalState )
                -> PathProxy&;
                
                public: auto string
                    ( void /* v_ */ ) const
                -> std::string;
                
                public: auto operator /
                    ( std::string_view /* strv_sub_ */ ) const
                -> PathProxy;
                
                public: auto operator >>=
                    ( std::fs::path const& /* kr_fsp_linkTarget_ */ ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: operator
                    std::fs::path
                    ( void /* v_ */ ) const
                ;
                
                public: operator
                    std::string
                    ( void /* v_ */ ) const
                ;
                
                // public: explicit
                //     CLASS_CTOR PathProxy
                //     ( void /* v_ */ )
                // ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* PATH_PROXY_HPP_UNIQX_ */
