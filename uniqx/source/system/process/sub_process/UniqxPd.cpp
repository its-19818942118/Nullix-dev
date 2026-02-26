// #include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <sys/poll.h>
#include <utility>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <asm-generic/ioctls.h>

#include "globals.hpp"
#include "sub_process/UniqxPd.hpp"
#include "sub_process/SubProcess.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    using namespace system::process;
    
    CLASS_CTOR [[
        
    ]] SubProcess::
        UniqxPd::UniqxPd
            ( const int k_i_pd_ )
        : PM_i_pd ( k_i_pd_ )
    { }
    
    CLASS_CTOR [[
        
    ]] SubProcess::
        UniqxPd::UniqxPd
            ( UniqxPd&& Upd_otherPd_ ) noexcept
        : PM_i_pd ( std::exchange ( Upd_otherPd_.PM_i_pd , -1 ) )
    { }
    
    auto SubProcess::
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
    
    auto SubProcess::
        UniqxPd::release
        ( void /* v_ */ ) noexcept ( true )
    -> int
    {
        
        return ( std::exchange ( this->PM_i_pd , -1 ) );
        
    }
    
    auto SubProcess::
        UniqxPd::redirectTo
        ( const int i_targetPd_ ) noexcept ( true )
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
            this->PM_i_pd = -1;
            
        }
        
    }
    
    auto SubProcess::
        UniqxPd::setNonBlock
        ( const bool k_b_enabled_ ) noexcept ( true )
    -> void
    {
        
        if ( this->closed ( ) ) [[ unlikely ]] return;
        
        if
            ( k_b_enabled_ ) [[ likely ]]
        {
            
            ::fcntl
                (
                    this->PM_i_pd ,
                    F_SETFL ,
                    ::fcntl ( this->PM_i_pd , F_GETFL , +0 ) | O_NONBLOCK
                )
            ;
            
        }
    }
    
    auto SubProcess::
        UniqxPd::pollState
        ( const int k_i_tmoutMS_ ) noexcept ( true )
    -> PollStatus
    {
        
        using enum PollStatus;
        
        constexpr nfds_t k_nfd_pdLim { +1 };
        
        if ( this->closed ( ) ) [[ unlikely ]] return _em_Closed;
        
        pollfd
            _pfd_pd
            {
                .fd = this->PM_i_pd ,
                .events = POLLIN ,
                .revents = 0
            }
        ;
        
        if
            ( ::poll ( &_pfd_pd , k_nfd_pdLim , k_i_tmoutMS_ ) <= +0 )
        {
            return _em_Sleep;
        }
        
        if ( _pfd_pd.revents & POLLIN ) return _em_Data;
        if ( _pfd_pd.revents & POLLHUP ) return _em_Closed;
        
        return _em_Sleep;
    }
    
    auto SubProcess::
        UniqxPd::hasData
        ( void /* v_ */ ) const noexcept ( true )
    -> bool
    {
        
        int _i_bytesWaiting { };
        
        if
            ( ::ioctl ( this->PM_i_pd , FIONREAD , &_i_bytesWaiting ) == -1 )
        {
            return false;
        }
        
        return _i_bytesWaiting > 0;
        
    }
    
    auto SubProcess::
        UniqxPd::getData
        ( void /* v_ */ ) noexcept ( false )
    -> std::string
    {
        
        if ( this->closed ( ) ) [[ unlikely ]] return { };
        
        std::string _str_result { };
        std::size_t _zu_bytesAvailable { };
        
        if
            (
                ::ioctl
                ( this->PM_i_pd , FIONREAD , &_zu_bytesAvailable ) != -1 &&
                this->hasData ( )
            ) [[ likely ]]
        {
            
            _str_result.resize ( _zu_bytesAvailable + _str_result.size ( ) );
            
            if
                (
                    const ssize_t
                        k_z_bytesRead
                        {
                            ::read
                            (
                                this->PM_i_pd ,
                                _str_result.data ( ) ,
                                _zu_bytesAvailable
                            )
                        }
                    ; k_z_bytesRead >= +0L &&
                    std::size_t ( k_z_bytesRead ) < _str_result.size ( )
                ) [[ likely ]]
            {
                _str_result.resize ( std::size_t ( k_z_bytesRead ) );
            }
            
        }
        
        return ( _str_result );
        
    }
    
    // auto SubProcess::
    //     UniqxPd::writeDataTo
    //     ( std::string& ref_str_buffer_ ) noexcept ( true )
    // -> void
    // {
        
    //     if ( this->closed ( ) ) [[ unlikely ]] return;
        
    //     std::size_t _zu_bytesAvailable { };
        
    //     ref_str_buffer_.resize ( +4096 );
        
    //     if
    //         (
    //             ::ioctl
    //             ( this->PM_i_pd , FIONREAD , &_zu_bytesAvailable ) != -1 &&
    //             this->hasData ( )
    //         ) [[ likely ]]
    //     {
            
    //         ref_str_buffer_.resize
    //             ( _zu_bytesAvailable + ref_str_buffer_.size ( ) )
    //         ;
            
    //         if
    //             (
    //                 const ssize_t
    //                     k_z_bytesRead
    //                     {
    //                         ::read
    //                         (
    //                             this->PM_i_pd ,
    //                             ref_str_buffer_.data ( ) ,
    //                             _zu_bytesAvailable
    //                         )
    //                     }
    //                 ; k_z_bytesRead >= +0L &&
    //                 std::size_t ( k_z_bytesRead ) < ref_str_buffer_.size ( )
    //             ) [[ likely ]]
    //         {
    //             ref_str_buffer_.resize ( std::size_t ( k_z_bytesRead ) );
    //         }
            
    //     }
        
    // }
    
    auto SubProcess::
        UniqxPd::writeDataTo
        ( std::string& ref_str_buffer_ ) noexcept ( true )
    -> void
    {
        if ( this->closed ( ) ) [[ unlikely ]] return;
    
        int _i_bytesWaiting { };
        
        if
            (
                ::ioctl
                ( this->PM_i_pd , FIONREAD , &_i_bytesWaiting ) == -1 ||
                _i_bytesWaiting <= +0
            ) [[unlikely]]
        {
            return;
        }
    
        const std::size_t
            k_zu_bytesToRead
            { static_cast<std::size_t> ( _i_bytesWaiting ) }
        ;
        
        const std::size_t k_zu_oldSize { ref_str_buffer_.size ( ) };
    
        ref_str_buffer_.resize ( k_zu_oldSize + k_zu_bytesToRead );
    
        const ssize_t
            k_z_bytesRead
            {
                ::read
                (
                    this->PM_i_pd ,
                    &ref_str_buffer_ [ k_zu_oldSize ] ,
                    k_zu_bytesToRead
                )
            }
        ;
    
        if
            ( k_z_bytesRead >= +0 ) [[ likely ]]
        {
            if
                (
                    static_cast<std::size_t>
                    ( k_z_bytesRead ) < k_zu_bytesToRead
                )
            {
                ref_str_buffer_.resize
                    (
                        k_zu_oldSize + static_cast<std::size_t>
                        ( k_z_bytesRead )
                    )
                ;
            }
            
        }
        
        else 
        {
            ref_str_buffer_.resize ( k_zu_oldSize );
        }
        
    }
    
    // auto SubProcess::
    //     UniqxPd::flushDataTo
    //     ( std::string& str_resBufData_ ) noexcept ( true )
    // -> bool
    // {
        
    //     using enum PollStatus;
        
    //     str_resBufData_.reserve ( +4096 );
        
    //     if ( this->closed ( ) ) [[ unlikely ]] return { };
        
    //     const auto k_pst_state { this->pollState ( 0 ) };
        
    //     if
    //         ( k_pst_state == PollStatus::_em_Data ) [[ likely ]]
    //     {
            
    //         // str_resBufData_ += std::move ( this->writeDataTo ( ) );
            
    //         this->writeDataTo ( str_resBufData_ );
            
    //         return ( true );
            
    //     }
        
    //     if ( k_pst_state == PollStatus::_em_Closed ) [[ unlikely ]]
    //     {
    //         this->close ( );
    //         return ( false );
            
    //     }
        
    //     return ( true );
        
    // }
    
    auto SubProcess::
        UniqxPd::flushDataTo
        ( std::string& ref_str_resBufData_ ) noexcept ( true )
    -> bool
{
    using enum PollStatus;
    
    if ( this->closed ( ) ) [[ unlikely ]] return false;

    const auto k_pst_state { this->pollState ( +0 ) };

    if
        ( k_pst_state == _em_Data ) [[ likely ]]
    {
        
        int _i_bytesWaiting { };
        
        if
            (
                ::ioctl
                ( this->PM_i_pd , FIONREAD , &_i_bytesWaiting ) != -1 &&
                _i_bytesWaiting > +0
            ) 
        {
            
            const std::size_t k_zu_oldSize { ref_str_resBufData_.size ( ) };
            
            const std::size_t
                k_zu_toRead
                { static_cast<std::size_t> ( _i_bytesWaiting ) }
            ;
            
            ref_str_resBufData_.resize ( k_zu_oldSize + k_zu_toRead );
            
            if
                (
                    const ssize_t
                        k_z_bytesRead
                        {
                            ::read
                            (
                                this->PM_i_pd ,
                                std::move(&ref_str_resBufData_ [ k_zu_oldSize ]) ,
                                k_zu_toRead
                            )
                        }
                    ; k_z_bytesRead < ssize_t ( k_zu_toRead )
                ) 
            {
                ref_str_resBufData_.resize
                    (
                        k_zu_oldSize +
                        ( k_z_bytesRead > +0 ? size_t ( k_z_bytesRead ) : +0 )
                    )
                ;
            }
        }
        return true;
    }
    
    if
        ( k_pst_state == _em_Closed ) [[ unlikely ]]
    {
        
        this->close ( );
        return false;
        
    }
    
    return true;
}
    
    auto SubProcess::
        UniqxPd::close
        ( void /* v_ */ ) noexcept ( true )
    -> bool
    {
        
        if
            ( this->PM_i_pd >= +0 )
        {
            
            ::close ( this->PM_i_pd );
            
            this->PM_i_pd = -1L;
            
            return true;
            
        }
        
        return false;
        
    }
    
    auto SubProcess::
        UniqxPd::closed
        ( void /* v_ */ ) noexcept ( true )
    -> bool
    {
        return this->PM_i_pd < +0;
    }
    
    SubProcess::
        UniqxPd::operator int
        ( void /* v_ */) const noexcept ( true )
    {
        
        return ( this->PM_i_pd );
        
    }
    
    CLASS_DTOR [[
        
    ]] SubProcess::
        UniqxPd::~UniqxPd
        ( void /* v_ */ ) noexcept ( true )
    {
        
        if ( this->PM_i_pd >= +0 ) ::close ( this->PM_i_pd );
        
    }
    
} /* namespace uniqx */
