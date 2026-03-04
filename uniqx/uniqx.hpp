#ifndef UNIQX_HPP_
    #define UNIQX_HPP_
    
    #include "globals.hpp"
    
    namespace [[
        
        ]] uniqx
    {
        
        /*********************************************************************!
        @brief The Safety Interlock for global state changes.
        * Passing this struct to a .perma() call signals Explicit
        * intent to modify the persistent directory roots of the Host.
        !*********************************************************************/
        struct MutateGlobalState;
        
        struct
            MutateGlobalState
            {
                public: explicit
                    STRUCT_CTOR MutateGlobalState
                    ( void /* v_ */ ) = default
                ;
            }
        ;
        
        /*********************************************************************!
        @brief: A Unix like filesystem tree layout to easily manage files
        * & perform operations
        @details: FileTree acts as the 'Single Source of Truth' for the
        * * filesystem layout.
        * * It handles the mapping of standard folders (config, cache, local)
        * * & the dotfile source repository (optional).
        !*********************************************************************/
        class FileTree;
        
        class SysProc;
        
    } /* namespace uniqx */
    
    /*************************************************************************!
    @details: Prevents circular dependencies.
    * define this macro to get only forward declared symbols of uniqx ,
    * then undef to allow the ifndef condition to be true &
    * rid ourselves of circular dependencies.
    !*************************************************************************!
    * Usage Example:
    * ```cpp
    *     #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    *     #
    *     #   include "uniqx.hpp"
    *     #
    *     #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    * ```
    !*************************************************************************/
    #if !defined(ONLY_FORWARD_SYMBOL_DECLS_UNIQX_)
    #   include "sys_proc/SysProc.hpp"
    #   include "sys_proc/UniqxPd.hpp"
    #   include "sys_proc/UniqxPipe.hpp"
    #   include "file_tree/FileTree.hpp"
    #   include "file_tree/PathProxy.hpp"
    #endif /* ONLY_FORWARD_SYMBOL_DECLS_UNIQX_ */
    
#endif /* UNIQX_HPP_ */

#if 0
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
#endif
