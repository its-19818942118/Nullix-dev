/* #pragma once */

#ifndef PROC_STREAM_HPP_UNIQX_
    #define PROC_STREAM_HPP_UNIQX_
    
    #include <cstdint>
    #include <sys/types.h>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "globals.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        using namespace system;
        
        struct [[
            /* nullAttr_ */
        ]] process::
            ProcStream
            {
                
                private: using Type_t_ = uint_fast16_t;
                
                private: enum class
                    PM_e_Details_ : Type_t_
                    {
                        
                        nul_ = ( +0x00 << +0x00 ) , /* [ nul ] -> nulltp */
                        src_ = ( +0x01 << +0x00 ) , /* [ src ] -> stdinp */
                        out_ = ( +0x01 << +0x01 ) , /* [ out ] -> stdout */
                        err_ = ( +0x01 << +0x02 ) , /* [ err ] -> stderr */
                        ret_ = ( +0x01 << +0x03 ) , /* [ ret ] -> status */
                        
                    }
                ;
                
                private: mutable Type_t_ PMm_Typ_bits
                    { static_cast<Type_t_> ( PM_e_Details_::nul_ ) }
                ;
                
                public: static ProcStream const stdin;
                public: static ProcStream const stdout;
                public: static ProcStream const stderr;
                public: static ProcStream const status;
                public: static ProcStream const global;
                public: static ProcStream const empty;
                
                private: constexpr explicit
                    STRUCT_CTOR ProcStream
                        ( PM_e_Details_ bits_ )
                        noexcept ( true )
                    : PMm_Typ_bits ( static_cast<Type_t_> ( bits_ ) )
                { }
                
                private: constexpr explicit
                    STRUCT_CTOR ProcStream
                        ( Type_t_ Typ_bits_ )
                        noexcept ( true )
                    : PMm_Typ_bits ( Typ_bits_ )
                { }
                
                public: constexpr
                    STRUCT_CTOR ProcStream
                    ( void /* v_ */ )
                noexcept = default;
                
                public: [[
                    nodiscard ( "Returns bool checked opt_ bit." )
                ]] auto _has
                    ( ProcStream const /* k_psr_other_ */ ) const
                    noexcept ( true )
                -> bool;
                
                constexpr auto _count
                    ( void /* v_ */ ) const
                    noexcept ( true )
                -> int
                {
                    // The "Built-in" way to count set bits
                    return
                        __builtin_popcount
                        ( static_cast<unsigned int> ( this->PMm_Typ_bits ) )
                    ;
                }
                
                public: [[
                    nodiscard ( "Operator | for bit comparison" )
                ]] auto operator |
                    ( ProcStream const /* k_prs_rhs_ */ ) const
                    noexcept ( true )
                -> ProcStream;
                
                public: [[
                    nodiscard ( "Operator & for bit comparison" )
                ]] auto operator &
                    ( ProcStream const /* k_prs_rhs_ */ ) const
                    noexcept ( true )
                -> bool;
                
                public: [[
                    // nodiscard ( "Operator |= for bitor assignment" )
                ]] auto operator |=
                    ( ProcStream const /* k_prs_rhs_ */ ) const
                    noexcept ( true )
                -> ProcStream&;
                
                public: [[
                    nodiscard ( "Operator != for comparison." )
                ]] auto
                    operator !=
                    ( PM_e_Details_ const /* k_rhs_ */ ) const
                    noexcept ( true )
                -> bool;
                
                public: [[
                    nodiscard ( "Converter to convert this* to bool." )
                ]]
                    operator bool
                    ( void /* v_ */ ) const
                    noexcept ( true )
                ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* PROC_STREAM_HPP_UNIQX_ */
