/* #pragma once */

#ifndef RESULT_HPP_UNIQX_
    #define RESULT_HPP_UNIQX_
    
    #include <concepts>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        struct [[
            /* nullAttr_ */
        ]] system::process::ProcResult
            {
                long _status { };
                std::string _stdout { };
                std::string _stderr { };
                
                // private: friend class SysProc;
                
                public: auto exited
                    ( void /* v_ */ ) const
                -> bool;
                
                public: auto signaled
                    ( void /* v_ */ ) const
                -> bool;
                
                public: auto stopped
                    ( void /* v_ */ ) const
                -> bool;
                
                public: auto continued
                    ( void /* v_ */ ) const
                -> bool;
                
                public: auto core_dumped
                    ( void /* v_ */ ) const
                -> bool;
                
                public: auto exit_code
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto stop_signal
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto term_signal
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto status_code
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto stdOut
                    ( void /* v_ */ ) const
                -> std::string;
                
                public: auto stdErr
                    ( void /* v_ */ ) const
                -> std::string;
                
                public: auto success
                    ( void /* v_ */ ) const
                -> bool;
                
                public: template<std::predicate<ProcResult const&> Rule>
                    auto success
                    ( Rule&& /* p_rule_ */ ) const
                -> bool;
                
                operator int ( void /* v_ */ ) const;
                operator bool ( void /* v_ */ ) const;
                operator long ( void /* v_ */ ) const;
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* RESULT_HPP_UNIQX_ */
