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
    
    #include "globals.hpp"
    #include "process/ProcStream.hpp"
    #include "process/ProcResult.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] system::process::SubProcess
            {
                
                private: using opt_t = ProcStream;
                private: using Result_t = ProcResult;
                private: using string_t = std::string;
                private: using vecStr_t = std::vector<std::string>;
                
                private: mutable opt_t PMm_pOpt_opt;
                private: mutable string_t PMm_str_command;
                private: mutable vecStr_t PMm_vecStr_argv;
                private: mutable string_t PMm_str_inputData;
                private: mutable ProcResult PMm_pRes_cmdResult;
                private: mutable SubProcess const* PMmkp_sbp_prevCmd { };
                
                /// Class Constructor
                public: explicit CLASS_CTOR SubProcess
                    (
                        string_t const& /* kr_str_binName_ */ ,
                        vecStr_t const& /* kr_vecStr_argv_ */
                    )
                ;
                
                /// Execution engine
                private: auto mt_Res_execute
                    ( opt_t const /* pOpt_IO_ */ = opt_t::status ) const
                -> Result_t;
                
                public: auto
                    static create
                    (
                        string_t const& /* kr_str_binName_ */ = { } ,
                        vecStr_t const& /* kr_vecStr_argv_ */ = { }
                    )
                -> SubProcess;
                
                private: auto mt_v_printRecursive
                    ( bool const ) const
                -> void;
                
                public: auto printArgs
                    ( void /* v_ */ ) const
                -> void;
                
                public: auto run
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto capture
                    ( opt_t const opt_ ) const
                -> SubProcess const&;
                
                public: inline auto result
                    ( void /* v_ */ ) const
                -> Result_t const&
                {
                    return this->PMm_pRes_cmdResult;
                }
                
                public: auto operator ( )
                    ( string_t const& /* kr_str_argv_ */ ) const
                -> SubProcess const&;
                
                public: auto operator [ ]
                    ( string_t const& /* kr_str_argv_ */ ) const
                -> SubProcess const&;
                
                public: auto operator |
                    ( SubProcess const& /* kr_sbp_next_ */ ) const
                -> SubProcess const&;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* SYS_PROC_HPP_UNIQX_ */
