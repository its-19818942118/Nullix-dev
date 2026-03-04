#include <sys/wait.h>

#include "sys_proc/Result.hpp"
#include "sys_proc/SysProc.hpp"

namespace [[
    
    ]] uniqx
{
    
    auto SysProc::
        Result::exited
        ( void /* v_ */ ) const
    -> bool
    {
        return ( WIFEXITED ( this->_status ) );
    }
    
    auto SysProc::
        Result::signaled
        ( void /* v_ */ ) const
    -> bool
    {
        return ( WIFSIGNALED ( this->_status ) );
    }
    
    auto SysProc::
        Result::stopped
        ( void /* v_ */ ) const
    -> bool
    {
        return ( WIFSTOPPED ( this->_status ) );
    }
    
    auto SysProc::
        Result::continued
        ( void /* v_ */ ) const
    -> bool
    {
        #ifdef WIFCONTINUED
            return ( WIFCONTINUED ( this->_status ) );
        #else
            return ( false );
        #endif
    }
    
    auto SysProc::
        Result::core_dumped
        ( void /* v_ */ ) const
    -> bool
    {
        #if defined WCOREDUMP
            return ( this->signaled ( ) && WCOREDUMP ( this->_status ) );
        #else
            return ( false );
        #endif
    }
    
    auto SysProc::
        Result::stop_signal
        ( void /* v_ */ ) const
    -> int
    {
        return ( this->stopped ( ) ? WSTOPSIG ( this->_status ) : -1L );
    }
    
    auto SysProc::
        Result::term_signal
        ( void /* v_ */ ) const
    -> int
    {
        return ( this->signaled ( ) ? WTERMSIG ( this->_status ) : -1L );
    }
    
    auto SysProc::
        Result::exit_code
        ( void /* v_ */ ) const
    -> int
    {
        return ( this->exited ( ) ? WEXITSTATUS ( this->_status ) : -1L );
    }
    
    auto SysProc::
        Result::success
        ( void /* v_ */ ) const
    -> bool
    {
        return ( ( this->exited ( ) && this->exit_code ( ) ) == +0L );
    }
    
    template<std::predicate<const SysProc::Result&> Rule>
    auto SysProc::
        Result::success
        ( Rule&& p_rule_ ) const
    -> bool
    {
        return ( p_rule_ ( *this ) );
    }

} /* namespace uniqx */
