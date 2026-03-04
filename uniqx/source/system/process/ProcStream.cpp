#include "process/ProcStream.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    using namespace system::process;
    
    ProcStream const
        ProcStream::stdin
        { ProcStream::PM_e_Details_::src_ }
    ;
    
    ProcStream const
        ProcStream::stdout
        { ProcStream::PM_e_Details_::out_ }
    ;
    
    ProcStream const
        ProcStream::stderr
        { ProcStream::PM_e_Details_::err_ }
    ;
    
    ProcStream const
        ProcStream::status
        { ProcStream::PM_e_Details_::ret_ }
    ;
    
    ProcStream const
        ProcStream::global
        {
            static_cast<Type_t_> ( ProcStream::PM_e_Details_::ret_ ) |
            static_cast<Type_t_> ( ProcStream::PM_e_Details_::out_ ) |
            static_cast<Type_t_> ( ProcStream::PM_e_Details_::err_ ) |
            static_cast<Type_t_> ( ProcStream::PM_e_Details_::ret_ )
        }
    ;
    
    ProcStream const
        ProcStream::empty
        { ProcStream::PM_e_Details_::nul_ }
    ;
    
    auto
        ProcStream::_has
        ( ProcStream const k_prs_other_ ) const
        noexcept ( true )
    -> bool
    {
        
        return
            (
                (
                    this->PMm_Typ_bits & k_prs_other_.PMm_Typ_bits
                ) == k_prs_other_.PMm_Typ_bits
            )
        ;
        
    }
    
    auto
        ProcStream::operator |
        ( ProcStream const k_prs_rhs_ ) const
        noexcept ( true )
    -> ProcStream
    {
        
        return
            (
                ProcStream
                {
                    static_cast<Type_t_>
                    (
                        this->PMm_Typ_bits |
                        k_prs_rhs_.PMm_Typ_bits
                    )
                }
            )
        ;
        
    }
    
    auto
        ProcStream::operator &
        ( ProcStream const k_prs_rhs_ ) const
        noexcept ( true )
    -> bool
    {
        
        return ( this->_has ( k_prs_rhs_ ) );
        
    }
    
    auto
        ProcStream::operator |=
        ( ProcStream const k_prs_rhs_ ) const
        noexcept ( true )
    -> ProcStream&
    {
        
        this->PMm_Typ_bits |= k_prs_rhs_.PMm_Typ_bits;
        
        return ( const_cast<ProcStream&> ( *this ) );
        
    }
    
    auto
        ProcStream::operator !=
        ( PM_e_Details_ const k_rhs_ ) const
        noexcept ( true )
    -> bool
    {
        
        return ( this->PMm_Typ_bits != static_cast<Type_t_> ( k_rhs_ ) );
        
    }
    

        ProcStream::operator bool
        ( void /* v_ */ ) const
        noexcept ( true )
    {
        
        return ( *this != ProcStream::PM_e_Details_::nul_ );
        
    }
    
} /* namespace uniqx */
