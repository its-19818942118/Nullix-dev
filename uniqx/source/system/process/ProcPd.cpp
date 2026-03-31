#include <print>
#include <cerrno>
#include <ranges>
#include <cstddef>
#include <fcntl.h>
#include <utility>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <asm-generic/ioctls.h>

#include "globals.hpp"
#include "process/ProcPd.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    using namespace system;
    
    CLASS_CTOR [[
        /* nullAttr_ */
    ]] process::
        ProcPd::ProcPd
            ( int const k_i_pd_ )
        : PMm_i_pd ( k_i_pd_ )
    { }
    
    CLASS_CTOR [[
        /* nullAttr_ */
    ]] process::
        ProcPd::ProcPd
            ( ProcPd&& Ppd_otherPd_ )
            noexcept ( true )
        : PMm_i_pd ( std::exchange ( Ppd_otherPd_.PMm_i_pd , -1 ) )
    { }
    
    auto process::
        ProcPd::operator =
        ( ProcPd&& Ppd_otherPd_ ) const
        noexcept ( true )
    -> ProcPd&
    {
        
        if
            ( this != &Ppd_otherPd_ )
        {
            this->close ( );
            this->PMm_i_pd = std::exchange ( Ppd_otherPd_.PMm_i_pd , -1 );
        }
        
        return ( const_cast<ProcPd&> ( *this ) );
        
    }
    
    auto process::
        ProcPd::operator >>=
        ( int const i_targetPd_ ) const
        noexcept ( true )
    -> void
    {
        
        if ( this->closed ( ) ) return;
        
        if
            (
                this->PMm_i_pd != i_targetPd_ &&
                ::dup2 ( this->PMm_i_pd , i_targetPd_ ) != -1
            ) [[ likely ]]
        {
            // success. so we can safely close it
            this->close ( );
            
        }
        
        else [[ unlikely ]]
        {
            // forget the descriptor
            std::println ( stderr , "failed to redirect fd" );
            // this->close ( );
            
        }
        
    }
    
    auto process::
        ProcPd::setNonBlock
        ( bool const k_b_enabled_ ) const
        noexcept ( true )
    -> void
    {
        
        if ( this->closed ( ) ) [[ unlikely ]] return;
        
        if
            ( k_b_enabled_ ) [[ likely ]]
        {
            
            ::fcntl
                (
                    this->PMm_i_pd ,
                    F_SETFL ,
                    ::fcntl ( this->PMm_i_pd , F_GETFL , +0 ) | O_NONBLOCK
                )
            ;
            
        }
    }
    
    auto process::
        ProcPd::pollState
        ( int const k_i_tmoutMS_ ) const
        noexcept ( true )
    -> PollStatus
    {
        
        using enum PollStatus;
        
        constexpr nfds_t const _kK_nfd_pdLim { +1 };
        
        if ( this->closed ( ) ) [[ unlikely ]] return _em_Closed;
        
        pollfd
            _pfd_pd
            {
                .fd = this->PMm_i_pd ,
                .events = POLLIN ,
                .revents = 0
            }
        ;
        
        if
            ( ::poll ( &_pfd_pd , _kK_nfd_pdLim , k_i_tmoutMS_ ) <= +0 )
        {
            return _em_Sleep;
        }
        
        if ( _pfd_pd.revents & POLLIN ) return _em_Data;
        if ( _pfd_pd.revents & POLLHUP ) return _em_Closed;
        
        return _em_Sleep;
    }
    
    auto process::
        ProcPd::operator >>
        ( std::string& r_str_resBufData_TX_ ) const
        noexcept ( true )
    -> bool
    {
        using enum PollStatus;
        
        if ( this->closed ( ) ) [[ unlikely ]] return false;
        
        PollStatus const k_pst_state { this->pollState ( +0 ) };
        
        if
            ( k_pst_state == _em_Closed ) [[ unlikely ]]
        {
            
            this->close ( );
            return false;
            
        }
        
        if ( k_pst_state != _em_Data ) [[ unlikely ]] return false;
        
        // 64kb buffer
        constexpr size_t const _kK_zu_pipeBufSize { +64 << +10 };
        size_t const K_zu_bufOffset { r_str_resBufData_TX_.size ( ) };
        
        r_str_resBufData_TX_.resize_and_overwrite
            (
                K_zu_bufOffset + _kK_zu_pipeBufSize ,
                [ & ] ( char* c_str_ , std::size_t zu_capacity_ )
                {
                    std::span<char>
                        _spn_bufView
                        {
                            std::views::counted
                            ( c_str_ , long ( zu_capacity_ ) )
                        }
                    ;
                    
                    auto _buf { _spn_bufView.subspan ( K_zu_bufOffset ) };
                    
                    ssize_t const
                        k_z_bytesRXd
                        {
                            ::read
                            (
                                this->PMm_i_pd ,
                                _buf.data ( ) ,
                                _buf.size ( )
                            )
                        }
                    ;
                    
                    return
                        (
                            ( k_z_bytesRXd > 0 )
                            ? ( K_zu_bufOffset + size_t ( k_z_bytesRXd ) )
                            : K_zu_bufOffset
                        )
                    ;
                    
                }
                
            )
        ;
        
        return ( r_str_resBufData_TX_.size ( ) > K_zu_bufOffset );
        
    }
    
    auto process::
        ProcPd::operator <<
        ( std::string const& kr_str_resBufData_RX_ ) const
        noexcept ( true )
    -> bool
    {
        
        if ( this->closed ( ) ) return false;
        if ( kr_str_resBufData_RX_.empty ( ) ) return false;
        
        if
            (
                this->PM_zu_RX_Offset >= kr_str_resBufData_RX_.size ( )
            )
        {
            
            this->close ( );
            
            return false;
            
        }
        
        std::span<char const> _spn_remBufView { kr_str_resBufData_RX_ };
        
        auto _remBufCnt { _spn_remBufView.subspan ( this->PM_zu_RX_Offset ) };
        
        ssize_t const
            k_z_bytesTXd
            {
                ::write
                (
                    this->PMm_i_pd ,
                    _remBufCnt.data ( ) ,
                    _remBufCnt.size ( )
                )
            }
        ;
        
        if ( k_z_bytesTXd == -1 && errno == EAGAIN ) return false;
        
        if
            ( k_z_bytesTXd == -1 )
        {
            
            this->close ( );
            return false;
            
        }
        
        this->PM_zu_RX_Offset += size_t ( k_z_bytesTXd );
        
        if
            ( this->PM_zu_RX_Offset >= kr_str_resBufData_RX_.size ( ) )
        {
            this->close ( );
        }
        
        return true;
        
    }
    
    auto process::
        ProcPd::close
        ( void /* v_ */ ) const
        noexcept ( true )
    -> void
    {
        
        if
            ( this->PMm_i_pd >= +0 )
        {
            
            ::close ( this->PMm_i_pd );
            
            this->PMm_i_pd = -1L;
            
        }
        
    }
    
    auto process::
        ProcPd::closed
        ( void /* v_ */ ) const
        noexcept ( true )
    -> bool
    {
        return this->PMm_i_pd < +0;
    }
    
    process::
        ProcPd::operator int
        ( void /* v_ */) const
        noexcept ( true )
    {
        
        return ( this->PMm_i_pd );
        
    }
    
    CLASS_DTOR [[
        /* nullAttr_ */
    ]] process::
        ProcPd::~ProcPd
        ( void /* v_ */ )
        noexcept ( true )
    {
        
        this->close ( );
        
    }
    
} /* namespace uniqx */
