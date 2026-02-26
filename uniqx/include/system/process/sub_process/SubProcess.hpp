/* #pragma once */

#ifndef SYS_PROC_HPP_UNIQX_
    #define SYS_PROC_HPP_UNIQX_
    
    #include <print>
    #include <ranges>
    #include <string>
    #include <vector>
    #include <cstdint>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "globals.hpp"
    #include "sub_process/ProcResult.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] system::process::SubProcess
            {
                
                /// Pipe Descriptor Handler
                private: class UniqxPd;
                /// Pipe Descriptor Spawner
                private: struct UniqxPipe;
                
                public: enum class
                    ProcOpt : uint_fast16_t
                    {
                        null_ = ( 0 << 0 ) , /* null_ 0 << 0 = 0 */
                        stdin = ( 1 << 0 ) , /* stdin 1 << 0 = 1 */
                        stdout = ( 1 << 1 ) , /* stdout 1 << 1 = 2 */
                        stderr = ( 1 << 2 ) , /* stderr 1 << 2 = 4 */
                        status_ = ( 1 << 3 ) , /* status_ 1 << 3 = 8 */
                    }
                ;
                
                public: using opt_t = ProcOpt;
                public: using Result_t = ProcResult;
                private: using string_t = std::string;
                private: using vecStr_t = std::vector<std::string>;
                
                private: string_t PM_str_inputData;
                private: mutable opt_t PM_pOpt_opt { };
                private: mutable string_t PM_str_command;
                private: mutable vecStr_t mut_PM_vecStr_argv;
                private: mutable ProcResult mut_PM_pRes_cmdResult;
                
                // public: using Config_t = Config;
                // public: uint_fast8_t envVars { 1 << 0 };
                // public: uint_fast8_t noEnvVars { 1 << 1 };
                // public: uint_fast8_t pathSearch { 1 << 2 };
                // public: uint_fast8_t noPathSearch { 1 << 3 };
                
                private: auto
                    friend operator |
                    (
                        SubProcess::opt_t /* opt_lhs_ */ ,
                        SubProcess::opt_t /* opt_rhs_ */
                    )
                -> SubProcess::opt_t;
                
                private: auto
                    friend operator &
                    (
                        SubProcess::opt_t /* opt_lhs_ */ ,
                        SubProcess::opt_t /* opt_rhs_ */
                    )
                -> SubProcess::opt_t;
                
                private: auto
                    friend operator |=
                    (
                        SubProcess::opt_t& /* opt_lhs_ */ ,
                        SubProcess::opt_t /* opt_rhs_ */
                    )
                -> SubProcess::opt_t&;
                
                private: auto
                    friend operator &=
                    (
                        SubProcess::opt_t& /* opt_lhs_ */ ,
                        SubProcess::opt_t /* opt_rhs_ */
                    )
                -> SubProcess::opt_t&;
                
                /// Class Constructor
                public: explicit CLASS_CTOR SubProcess
                    (
                        const string_t& /* kr_str_binName_ */ ,
                        const vecStr_t& /* kr_vecStr_argv_ */
                    )
                ;
                
                /// Execution engine
                private: auto mt_Res_execute
                    ( const ProcOpt /* pOpt_IO_ */ = opt_t::status_ ) const
                    // ( uint_fast8_t /* k_i_flags_ */ ) const
                -> Result_t;
                
                public: auto
                    static create
                    (
                        const string_t& /* kr_str_binName_ */ ,
                        const vecStr_t& /* kr_vecStr_argv_ */ = { }
                    )
                -> SubProcess;
                
                public: auto printArgs
                    ( void /* v_ */ ) const
                -> void;
                
                public: auto run
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto capture
                    ( const ProcOpt opt_ ) const
                -> void{
                    this->PM_pOpt_opt |= opt_;
                }
                
                public: inline auto result
                    ( void /* v_ */ ) const
                -> const Result_t&
                {
                    return this->mut_PM_pRes_cmdResult;
                }
                
                public: auto operator ( )
                    ( const string_t& /* kr_str_argv_ */ ) const
                -> const SubProcess&;
                
                public: auto operator [ ]
                    ( const string_t& /* kr_str_argv_ */ ) const
                -> const SubProcess&;
                
                
                auto inline run
                ( ProcOpt opt_IO_ ) const
            -> const SubProcess&
            {
                
                PM_pOpt_opt |= opt_IO_;
                
                this->mut_PM_pRes_cmdResult =
                    this->SubProcess::mt_Res_execute ( this->PM_pOpt_opt )
                ;
                
                return ( *this );
                
            }
            
        
                
            }
        ;
        
        auto hasFlag
            ( const system::process::SubProcess::opt_t opt_IO_ )
        -> bool;
        
    } /* namespace uniqx */
    
#endif /* SYS_PROC_HPP_UNIQX_ */
