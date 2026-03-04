#include <cerrno>
#include <cstddef>
#include <utility>
#include <unistd.h>

#include "globals.hpp"
#include "sys_proc/SysProc.hpp"
#include "sys_proc/UniqxPd.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    STRUCT_CTOR [[
        
    ]] SysProc::
        UniqxPd::UniqxPd
            ( const int k_i_pd_ )
        : PM_i_pd ( k_i_pd_ )
    { }
    
    STRUCT_CTOR [[
        
    ]] SysProc::
        UniqxPd::UniqxPd
            ( UniqxPd&& Upd_otherPd_ ) noexcept
        : PM_i_pd ( std::exchange ( Upd_otherPd_.PM_i_pd , -1 ) )
    { }
    
    auto SysProc::
        UniqxPd::operator =
        ( UniqxPd&& Upd_otherPd_ ) noexcept
    -> UniqxPd&
    {
        
        if
            ( this != &Upd_otherPd_ )
        {
            if ( this->PM_i_pd >= +0 ) ::close ( this->PM_i_pd );
            this->PM_i_pd = std::exchange ( Upd_otherPd_.PM_i_pd , -1 );
        }
        
        return ( *this );
        
    }
    
    auto SysProc::
        UniqxPd::release
        ( void /* v_ */ )
    -> int
    {
        
        return ( std::exchange ( this->PM_i_pd , -1 ) );
        
    }
    
    auto SysProc::
        UniqxPd::redirectTo
        ( const int i_targetPd_ )
    -> void
    {
        
        if ( this->PM_i_pd < +0 ) return;
        
        if
            (
                this->PM_i_pd != i_targetPd_ &&
                ::dup2 ( this->PM_i_pd , i_targetPd_ ) != -1
            ) [[ likely ]]
        {
            // success. so we can safely close it
            this->close ( );
            
        }
        
        else [[ unlikely ]]
        {
            // forget the descriptor
            this->PM_i_pd = -1L;
            
        }
        
    }
    
    auto SysProc::
        UniqxPd::getData
        ( void /* v_ */ )
    -> std::string
    {
        
        if ( this->PM_i_pd < +0 ) [[unlikely]] return { };
        
        // small string optimization
        constexpr size_t k_zu_chunkSize { +4096 };
        
        std::string str_result { };
        str_result.reserve ( k_zu_chunkSize );
        
        std::array<char , k_zu_chunkSize> _buffer { };
        
        
        while
            ( true )
        {
            
            if
                (
                    const ssize_t
                        k_z_bytesRead
                        {
                            ::read
                            (
                                this->PM_i_pd ,
                                _buffer.data ( ) ,
                                _buffer.size ( )
                            )
                        }
                    ; k_z_bytesRead > +0L
                ) [[ likely ]]
            {
                str_result.append
                    (
                        _buffer.data ( ) ,
                        size_t ( k_z_bytesRead )
                    )
                ;
            }
            
            else if ( k_z_bytesRead == +0 ) break;
            
            else if ( errno == EINTR ) [[ unlikely ]] continue;
            
            else break;
            
        }
        
        return ( str_result );
        
    }
    
    auto SysProc::
        UniqxPd::close
        ( void /* v_ */ )
    -> void
    {
        
        if
            ( this->PM_i_pd >= +0 )
        {
            
            ::close ( this->PM_i_pd );
            
            this->PM_i_pd = -1L;
            
        }
        
    }
    
    SysProc::
        UniqxPd::operator int
        ( void /* v_ */) const
    {
        
        return ( this->PM_i_pd );
        
    }
    
    STRUCT_DTOR [[
        
    ]] SysProc::
        UniqxPd::~UniqxPd
        ( void /* v_ */ )
    {
        
        if ( this->PM_i_pd >= +0 ) ::close ( this->PM_i_pd );
        
    }
    
} /* namespace uniqx */
