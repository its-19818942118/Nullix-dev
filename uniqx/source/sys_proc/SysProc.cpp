#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "sys_proc/Result.hpp"
#include "sys_proc/SysProc.hpp"
#include "sys_proc/UniqxPd.hpp"
#include "sys_proc/UniqxPipe.hpp"

namespace [[
        /* nullAttr_ */
    ]]  uniqx
{
    
    auto [[
        /* Execution engine */
    ]] SysProc::
        mt_Res_execute
        ( const std::string& binName_ ) const
    -> Result_t
    {
        
        auto [[
            /* (Output pipe descriptor) Rx , Tx */
        ]] [ _outPipeDes_RX , _outPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
        auto [[
            /* (Error Output pipe descriptor) Rx , Tx */
        ]] [ _errPipeDes_RX , _errPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
        ::pid_t _pid { ::fork ( ) };
        
        if
            ( _pid < +0L )
        { }
        
        if
            ( _pid == +0L )
        {
            
            /* the chaild process transmits the data to us */
            
            /// close read/reciver (RX) unused pipes else it will hang the pipe
            _outPipeDes_RX.close ( );
            _errPipeDes_RX.close ( );
            
            /// Child process writes/transmits (TX) data to stdout
            _outPipeDes_TX.redirectTo ( STDOUT_FILENO );
            /// Child process writes/transmits (TX) data to stderr
            _errPipeDes_TX.redirectTo ( STDERR_FILENO );
            
            std::vector<char*>&& _args { };
            _args.reserve ( this->PM_vecStr_args.size ( ) +2L );
            
            for
                ( auto arg_ : this->PM_vecStr_args )
            {
                _args.emplace_back ( arg_.data ( ) );
            }
            
            _args.emplace_back ( nullptr );
            
            ::execvp ( binName_.data ( ) , _args.data ( ) );
            
        }
        
        else
        {
            
            // we are not transmitting any data. so close the write(TX) ends
            _outPipeDes_TX.close ( );
            _errPipeDes_TX.close ( );
            
            const std::string k_str_stdOut { _outPipeDes_RX.getData ( ) };
            const std::string k_str_stdErr { _errPipeDes_RX.getData ( ) };
            
            int _i_status { };
            
            ::waitpid ( _pid , &_i_status , +0L );
            
            return
                {
                    ._status =
                    (
                        WIFEXITED ( _i_status )
                        ? WEXITSTATUS(_i_status)
                        : -1
                     ) ,
                    ._stdout { k_str_stdOut } ,
                    ._stderr { k_str_stdErr } ,
                }
            ;
            
        }
        
        return { };
        
    }
    
} /* namespace uniqx */
