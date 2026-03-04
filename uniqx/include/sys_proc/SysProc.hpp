/* #pragma once */

#ifndef SYS_PROC_HPP_UNIQX_
    #define SYS_PROC_HPP_UNIQX_
    
    #include <string>
    #include <vector>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] SysProc
            {
                
                /// Result type
                private: struct Result;
                /// Config type
                // private: struct Config;
                
                /// Pipe Descriptor Handler
                private: struct UniqxPd;
                /// Pipe Descriptor Spawner
                private: struct UniqxPipe;
                
                private: std::string PM_str_command;
                private: std::string PM_str_inputData;
                private: std::vector<std::string> PM_vecStr_args;
                
                public: using Result_t = Result;
                // public: using Config_t = Config;
                // public: uint_fast8_t envVars { 1 << 0 };
                // public: uint_fast8_t noEnvVars { 1 << 1 };
                // public: uint_fast8_t pathSearch { 1 << 2 };
                // public: uint_fast8_t noPathSearch { 1 << 3 };
                
                /// Execution engine
                private: auto mt_Res_execute
                    // ( uint_fast8_t /* k_i_flags_ */ ) const
                    ( const std::string& /* binName_ */ ) const
                -> Result;
                
                public: auto static create
                    // ( uint_fast8_t /* k_i_flags_ */ )
                    ( void /* v_ */ )
                -> Result;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* SYS_PROC_HPP_UNIQX_ */
