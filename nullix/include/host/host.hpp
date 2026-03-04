/* #pragma once */

#ifndef HOST_HPP_NULLIX_TA9_GC_
    #define HOST_HPP_NULLIX_TA9_GC_
    
    #include <expected>
    #include <filesystem>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_TA9_GC_
    #
    #   include "nullix.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_TA9_GC_
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_TA9_GC_
    #
    #   include "uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_TA9_GC_
    
    #include "file_tree/FileTree.hpp"
    
    namespace
        nullix
    {
        
        class
            Host
            {
                /* variable to store the user name */
                public: const std::string kpm_str_userName { };
                
                /* variable to store the host name */
                public: const std::string kpm_str_hostName { };
                
                /* class to manager & oraganize the host base dirs */
                /*************************************************************|
                |* @brief: Manages the application environment &
                |* base directory state.
                |* * @details: Filetree acts as the 'Single Source of Truth'
                |* * for the filesystem layout.
                |* * It handles the mapping of standard folders
                |* * (config, cache, local) & the dotfile source repository.
                |*************************************************************/
                // public: class FileTree;
                public: uniqx::FileTree dirs;
                
                private: [[
                    nodiscard
                    ( "Important: returns k_username" )
                ]] auto static
                    _mtGet_str_userName
                    (
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    )
                -> std::expected <std::string , Err_t_>;
                
                private: [[
                    nodiscard
                    ( "Important: returns k_hostname" )
                ]] auto static
                    _mtGet_str_hostName
                    (
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    )
                -> std::expected <std::string, Err_t_>;
                
                public: auto
                    _mtGet_str_hostInfo
                    ( void /* v_ */ ) const
                -> std::string;
                
                public: auto static
                    mt_fsp_userHome
                    ( void /* v_ */ )
                -> std::fs::path;
                
                public: explicit
                    CLASS_CTOR Host
                    ( void /* v_ */ )
                ;
                
                public: explicit
                    CLASS_CTOR Host
                    ( const std::fs::path& /* k_ref_fsp_userHome_ */ )
                ;
                
                public:
                    CLASS_DTOR ~Host
                    ( void /* v_ */ )
                ;
                
            }
        ;
        
    } /* namespace nullix */
    
#endif /* HOST_HPP_NULLIX_TA9_GC_ */
