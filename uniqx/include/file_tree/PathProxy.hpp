/* #pragma once */

#ifndef PATH_PROXY_HPP_UNIQX_
    #define PATH_PROXY_HPP_UNIQX_
    
    #include <string>
    #include <expected>
    #include <filesystem>
    #include <source_location>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "diagnostics/error.hpp"
    #include "file_tree/FileTree.hpp"
    
    namespace
        uniqx
    {
        
        class FileTree::PathProxy
            {
                
                public: FileTree& parent;
                
                public: std::fs::path current;
                public: std::fs::path state_ref;
                
                public: auto linkTo
                    (
                        const std::fs::path& /* fsp_target_ */ ,
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: auto unlink
                    (
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: auto prune
                    (
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                -> std::expected<PathProxy , nullix::ErrInt_t_>;
                
                public: auto cleanBroken
                    (
                        const std::src_loc /* k_sl_srcLoc_ */ =
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
                    ( const std::fs::path& /* k_ref_fsp_linkTarget_ */ ) const
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
