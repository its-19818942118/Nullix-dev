/* #pragma once */
/*****************************************************************************!
|* @file @b FileTree.hpp                                                      |
|=============================================================================|
|* @author: @b izumi Izumi [its_19818942118 , its-19818942118]                |
|* @version: Uniqx_ v0.0.1                                                    |
|* @lib: Uniqx_                                                               |
|=============================================================================|
|* @brief: A fluent, atomic filesystem and dotfile management engine.         |
|* * @details: This engine provides a "Path DSL" for C++ that allows for:     |
|* 1. Fluent path building via the '/' operator.                              |
|* 2. Atomic symlinking (create-then-swap) prevents configuration corruption. |
|* 3. Protected global state mutation using the "Safety Key" pattern.         |
|* 4. Automatic cleanup of broken links and self-healing directory structures.|
!*****************************************************************************/
 
#ifndef FILE_TREE_HPP_UNIQX_
    #define FILE_TREE_HPP_UNIQX_
    
    #include <optional>
    #include <filesystem>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    namespace
        uniqx
    {
        
        /*********************************************************************!
        @brief: A Unix like filesystem tree layout to easily manage files  
        * & perform operations                                               
        @details: FileTree acts as the 'Single Source of Truth' for the  
        * filesystem layout.                                               
        * It handles the mapping of standard folders (config, cache, local)
        * & the dotfile source repository (optional).                      
        !*********************************************************************/
        class
            FileTree
            {
                
                /*************************************************************!
                @brief: A short-lived fluent builder for filesystem operations.
                * * The PathProxy is returned by all entry points.
                * * It allows for:
                * - Chaining: `dirs.config() / "sub" / "file.txt"`
                * - Atomic Sync: `.link_to(target_path)`
                * - Maintenance: `.clean_broken()` to remove dead symlinks.
                * * @note: PathProxy holds a reference to FileTree.
                * * Do not store PathProxies globally or
                * * outlive the Host object. (not required anymore)
                !*************************************************************/
                public: class PathProxy;
                
                private: std::fs::path PM_fsp_actv_userHome;
                private: const std::fs::path kPM_fsp_fctr_userHome;
                
                /* permanent directory roots */
                private: std::fs::path PM_fsp_usrCacheRoot;
                private: std::fs::path PM_fsp_usrLocalRoot;
                private: std::fs::path PM_fsp_usrConfigRoot;
                /* source for uniqx's dotfiles */
                private: std::fs::path PM_fsp_usrSourceRoot;
                
                // /* proxy directories for ease of use */
                // private: PathProxy PM_fsp_usrCacheProxy;
                // private: PathProxy PM_fsp_usrLocalProxy;
                // private: PathProxy PM_fsp_usrConfigProxy;
                // /* source for uniqx's dotfiles */
                // private: PathProxy PM_fsp_usrSourceProxy;
                
                public: auto home
                    ( void /* v_ */ )
                -> PathProxy;
                
                public: auto config
                    ( void /* v_ */ )
                -> PathProxy;
                
                public: auto local
                    ( void /* v_ */ )
                -> PathProxy;
                
                public: auto cache
                    ( void /* v_ */ )
                -> PathProxy;
                
                public: auto source
                    (
                        const std::optional<std::string_view> /* opt_ovr_ */ =
                        { std::nullopt }
                    )
                -> PathProxy;
                
                public: auto sync_all
                    ( void /* v_ */ )
                -> void;
                
                public: explicit
                    CLASS_CTOR FileTree
                    ( void /* v_ */ )
                ;
                
                public: explicit
                    CLASS_CTOR FileTree
                    ( const std::fs::path& /* k_ref_fsp_userHome_ */ )
                ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* FILE_TREE_HPP_UNIQX_ */
