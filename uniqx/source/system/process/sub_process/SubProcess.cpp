#include <fcntl.h>
#include <algorithm>
#include <string>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <sys/poll.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "globals.hpp"
#include "sub_process/UniqxPd.hpp"
#include "sub_process/UniqxPipe.hpp"
#include "sub_process/ProcResult.hpp"
#include "sub_process/SubProcess.hpp"

namespace [[
        /* nullAttr_ */
    ]]  uniqx
{
    
    using namespace system::process;
    
    auto system::process::operator |
        ( SubProcess::opt_t opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t
    {
        using T = std::underlying_type_t<SubProcess::opt_t>;
        return
            (
                static_cast<SubProcess::opt_t>
                (
                    static_cast<T> ( opt_lhs_ ) | static_cast<T> ( opt_rhs_ )
                )
            )
        ;
    }
    
    auto system::process::operator &
        ( SubProcess::opt_t opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t
    {
        using T = std::underlying_type_t<SubProcess::opt_t>;
        return
            (
                static_cast<SubProcess::opt_t>
                (
                    static_cast<T> ( opt_lhs_ ) & static_cast<T> ( opt_rhs_ )
                )
            )
        ;
    }
    
    auto system::process::operator |=
        ( SubProcess::opt_t& opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t&
    {
        opt_lhs_ = opt_lhs_ | opt_rhs_;
        return ( opt_lhs_ );
    }
    
    auto system::process::operator &=
        ( SubProcess::opt_t& opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t&
    {
        opt_lhs_ = opt_lhs_ & opt_rhs_;
        return ( opt_lhs_ );
    }
    
    
    auto hasFlag
        ( const SubProcess::opt_t opt_IO_ )
    -> bool
    {
        using T = std::underlying_type_t<SubProcess::opt_t>;
        return
            (
                static_cast<T> ( opt_IO_ ) != +0
            )
        ;
    }
    
    CLASS_CTOR
        SubProcess::SubProcess
            (
                const string_t& kr_str_cmdName_ ,
                const vecStr_t& kr_vecStr_argv_
            )
        : PM_str_command ( kr_str_cmdName_ )
        , mut_PM_vecStr_argv ( kr_vecStr_argv_ )
    {
        this->mut_PM_vecStr_argv.reserve
            ( this->mut_PM_vecStr_argv.size ( ) +32ZU )
        ;
        
        this->mut_PM_vecStr_argv.emplace_back
            ( kr_str_cmdName_ )
        ;
        
        std::rotate
            (
                this->mut_PM_vecStr_argv.rbegin ( ) ,
                this->mut_PM_vecStr_argv.rbegin ( ) + 1ZU ,
                this->mut_PM_vecStr_argv.rend ( )
            )
        ;
        
    }
    
    auto [[
        /* Execution engine */
    ]] SubProcess::
        mt_Res_execute
        ( const ProcOpt pOpt_IO_ ) const
    -> Result_t
    {
        
        auto [[
            /* (Output pipe descriptor) Rx , Tx */
        ]] [ _outPipeDes_RX ,
            _outPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
        auto [[
            /* (Error Output pipe descriptor) Rx , Tx */
        ]] [ _errPipeDes_RX ,
            _errPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
        ::pid_t _pid { ::fork ( ) };
        
        if
            ( _pid < +0 )
        { }
        
        if
            ( _pid == +0 )
        {
            
            /* the chaild process transmits the data to us */
            
            /// close read/reciver (RX) unused pipes else it will hang the pipe
            _outPipeDes_RX.close ( );
            _errPipeDes_RX.close ( );
            
            /// Child process writes/transmits (TX) data to stdout
            _outPipeDes_TX.redirectTo ( STDOUT_FILENO );
            /// Child process writes/transmits (TX) data to stderr
            _errPipeDes_TX.redirectTo ( STDERR_FILENO );
            
            const auto k_zu_argvMax { this->mut_PM_vecStr_argv.size ( ) + 1ZU };
            
            std::vector<char* > _args { };
            _args.reserve ( k_zu_argvMax );
            
            for
                ( const auto& arg_ : this->mut_PM_vecStr_argv )
            {
                _args.emplace_back ( const_cast<char* > ( arg_.c_str ( ) ) );
            }
            
            _args.emplace_back ( nullptr );
            
            ::execvp ( _args.front ( ) , _args.data ( ) );
            
            ::_exit ( 127 );
            
        }
        
        else
        {
            
            // we are not transmitting any data. so close the write(TX) ends
            _outPipeDes_TX.close ( );
            _errPipeDes_TX.close ( );
            
            // set nonblocking to prevent freezing when reading large buffers
            _outPipeDes_RX.setNonBlock ( true );
            _errPipeDes_RX.setNonBlock ( true );
            
            string_t _str_outResData; _str_outResData.reserve((+4096*+2));
            string_t _str_errResData; _str_errResData.reserve((+4096*+2));
            
            while
                ( !_outPipeDes_RX.closed ( ) || !_errPipeDes_RX.closed ( ) )
            {
                
                [[ maybe_unused ]] bool b_active { };
                
                if
                    ( hasFlag ( pOpt_IO_ & ProcOpt::stderr ) ) [[ likely ]]
                {
                    b_active |= _errPipeDes_RX.flushDataTo ( _str_errResData );
                }
                
                else
                {
                    b_active |= _errPipeDes_RX.flushDataTo ( _str_errResData );
                    _str_errResData.clear ( );
                }
                
                if
                    ( hasFlag ( pOpt_IO_ & ProcOpt::stdout ) ) [[ likely ]]
                {
                    b_active |= _outPipeDes_RX.flushDataTo ( _str_outResData );
                }
                
                else
                {
                    b_active |= _outPipeDes_RX.flushDataTo ( _str_outResData );
                    _str_outResData.clear ( );
                }
                
                if
                    (
                        !b_active &&
                        (
                            !_outPipeDes_RX.closed ( ) ||
                            !_errPipeDes_RX.closed ( )
                        )
                    )
                {
                    ::poll ( nullptr , +0 , +10 );
                }
                
            }
            
            int _i_status { };
            
            ::waitpid ( _pid , &_i_status , +0 );
            
            return
                {
                    ._status =
                    (
                        WIFEXITED ( _i_status )
                        ? WEXITSTATUS ( _i_status )
                        : -1
                    ) ,
                    ._stdout
                    {
                        not _str_outResData.empty ( )
                        ? std::move ( _str_outResData )
                        : ""
                    } ,
                    ._stderr
                    {
                        not _str_errResData.empty ( )
                        ? std::move ( _str_errResData )
                        : ""
                    }
                }
            ;
            
        }
        
    }
    
    auto SubProcess::create
        (
            const string_t& kr_str_cmdName_ ,
            const vecStr_t& kr_vecStr_argv_
        )
    -> SubProcess
    {
        
        return
            (
                SubProcess
                (
                    std::move(kr_str_cmdName_) ,
                    std::move(kr_vecStr_argv_)
                )
            )
        ;
        
    }
    
    auto SubProcess::printArgs
        ( void /* v_ */ ) const
    -> void
    {
        
        std::println
            (
                stderr ,
                "\nCommand: {:?}" ,
                this->mut_PM_vecStr_argv.front ( )
            )
        ;
        
        std::print ( stderr , "Argv: " );
        
        for
            (
                size_t idx { +1ZU } ,
                size = this->mut_PM_vecStr_argv.size ( );
                idx < size; ++idx
            )
        {
            
            if ( idx == size - 1ZU )
            {
                
                std::println
                    (
                        stderr ,
                        "[ {:?} ]" ,
                        this->mut_PM_vecStr_argv [ idx ]
                    )
                ;
                
            }
            else
            {
                
                std::print
                    (
                        stderr ,
                        "[ {:?} ] " ,
                        this->mut_PM_vecStr_argv [ idx ]
                    )
                ;
                
            }
        }
        
        std::println ( );
    }
    
    auto SubProcess::run
        ( void /* v_ */ ) const
    -> int
    {
        
        this->mut_PM_pRes_cmdResult =
            this->SubProcess::mt_Res_execute ( this->PM_pOpt_opt )
        ;
        
        return
            (
                this->mut_PM_pRes_cmdResult.exit_code ( )
            )
        ;
        
    }
    
    auto SubProcess::
        operator ( )
        ( const string_t& kr_str_cmdName_ ) const
    -> const SubProcess&
    {
        
        if
            ( this->PM_str_command == kr_str_cmdName_ )
        {
            return ( *this );
        }
        
        this->PM_str_command = kr_str_cmdName_;
        
        this->mut_PM_vecStr_argv.front ( ) = kr_str_cmdName_;
        
        return ( *this );
        
    }
    
    auto SubProcess::
        operator [ ]
        ( const string_t& kr_str_argv_ ) const
    -> const SubProcess&
    {
        
        if
            (
                const auto&
                    kr_aut_itr
                    {
                        std::find
                        (
                            this->mut_PM_vecStr_argv.begin ( ) + 1ZU ,
                            this->mut_PM_vecStr_argv.end ( ) ,
                            kr_str_argv_
                        )
                    }
                ; kr_aut_itr == this->mut_PM_vecStr_argv.end ( )
            )
        {
            this->mut_PM_vecStr_argv.emplace_back ( kr_str_argv_ );
        }
        
        
        return ( *this );
        
    }
    
} /* namespace uniqx */
