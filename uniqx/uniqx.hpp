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
        
        namespace [[
                /* nullAttr_ */
            ]] system
        {
            
            class FileTree;
            
            namespace [[
                    /* nullAttr_ */
                ]] process
            {
                
                constexpr std::size_t argvLimit { 1 << 10 };
                
                struct [[ /* Process Config */ ]] ProcRules;
                struct [[ /* Process DataIO */ ]] ProcStream;
                class [[ /* Process Manager */ ]] SubProcess;
                class [[ /* SubProcess Piper */ ]] SubProcPipe;
                class [[ /* Pipe Descriptor Handler */ ]] ProcPd;
                struct [[ /* Pipe Descriptor Spawner */ ]] ProcPipe;
                struct [[ /* Process's Result Storage */ ]] ProcResult;
                
            }
            
        } /* namespace system */
        
        namespace [[
                /* nullAttr_ */
            ]] filesystem
        {
            
            class Path;
            
            class FileTree;
            
        } /* namespace filesystem */
        
        namespace [[
                /* nullAttr_ */
            ]] fs
        {
            
            using namespace uniqx::filesystem;
            
        } /* namespace fs */
        
    } /* namespace uniqx */
    
    /*************************************************************************!
    @brief: Prevents circular dependencies.
    @details: #define the macro to get only forward declared symbols of uniqx ,
    * then #undef to allow the #ifndef condition to be true rid ourselves of
    * circular dependencies.
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
    # /* IWYU pragma: begin_exports */
    #   include "filesystem/Path.hpp"
    #   include "filesystem/FileTree.hpp"
    #   include "system/process/ProcPd.hpp"
    #   include "system/process/ProcPipe.hpp"
    #   include "system/process/ProcResult.hpp"
    #   include "system/process/SubProcess.hpp"
    # /* IWYU pragma: end_exports */
    #endif /* ONLY_FORWARD_SYMBOL_DECLS_UNIQX_ */
    
#endif /* UNIQX_HPP_ */

#if 0
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
#endif
