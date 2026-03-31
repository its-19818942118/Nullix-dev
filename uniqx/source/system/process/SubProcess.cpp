#include <print>
#include <cstdio>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <sys/poll.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "globals.hpp"
#include "process/ProcPd.hpp"
#include "process/ProcPipe.hpp"
#include "process/ProcResult.hpp"
#include "process/SubProcess.hpp"

namespace [[
        /* nullAttr_ */
    ]]  uniqx
{
    
    using namespace system::process;
    
    CLASS_CTOR
        SubProcess::
            SubProcess
            (
                string_t const& kr_str_cmdName_ ,
                vecCxStr_t const& kr_vecStr_argv_
            )
        : PMm_str_command ( kr_str_cmdName_ )
        , PMm_vecStr_argv ( kr_vecStr_argv_ )
    {
        
        this->PMm_vecStr_argv.reserve
            ( this->PMm_vecStr_argv.size ( ) + ( +1ZU ) )
        ;
        
        this->PMm_vecStr_argv.emplace_back ( kr_str_cmdName_ );
        
        std::rotate
            (
                this->PMm_vecStr_argv.rbegin ( ) ,
                this->PMm_vecStr_argv.rbegin ( ) + ( +1ZU ) ,
                this->PMm_vecStr_argv.rend ( )
            )
        ;
        
        // this->mt_v_syncArgv ( );
        
        this->PMm_argNSync = true;
        
    }
    
    auto [[
        /* nullAtrr_  */
    ]] SubProcess::
        mt_v_syncArgv
        ( void /* v_ */ ) const
    -> void
    {
        
        this->PMm_vecStr_cArgv.clear ( );
        
        constexpr auto const _kK_argvTerminator { nullptr };
        auto const _k_zu_argc { this->PMm_vecStr_argv.size ( ) + ( +1ZU ) }; 
        
        this->PMm_vecStr_cArgv.reserve ( _k_zu_argc );
        
        for
            ( auto& arg_ : this->PMm_vecStr_argv )
        {
            
            this->PMm_vecStr_cArgv.emplace_back ( arg_.data ( ) );
            
        }
        
        this->PMm_vecStr_cArgv.emplace_back ( _kK_argvTerminator );
        
    }
    
    auto [[
        /* Execution engine */
    ]] SubProcess::
        mt_Res_execute
        ( opt_t const pOpt_IO_ ) const
    -> Result_t
    {
        
        /*! (Input pipe descriptor) [Rx , Tx] !*/
        /*! structed bindings to initialize stdin pipes !*/
         auto [ _inPipeDes_RX , _inPipeDes_TX ] { ProcPipe::create ( ) };
        
        /*! (Output pipe descriptor) [Rx , Tx] !*/
        /*! structed bindings to initialize stdout pipes !*/
        auto [ _outPipeDes_RX , _outPipeDes_TX ] { ProcPipe::create ( ) };
        
        /*! (Error Output pipe descriptor) [Rx , Tx] !*/
        /*! structed bindings to initialize stderr pipes !*/
        auto [ _errPipeDes_RX , _errPipeDes_TX ] { ProcPipe::create ( ) };
        
        if
            ( not ( pOpt_IO_ & opt_t::stdin ) )
        {
            _inPipeDes_RX.close ( );
            _inPipeDes_TX.close ( );
        }
        
        if
            ( not ( pOpt_IO_ & opt_t::stdout ) )
        {
            _outPipeDes_RX.close ( );
            _outPipeDes_TX.close ( );
        }
        
        if
            ( not ( pOpt_IO_ & opt_t::stderr ) )
        {
            _errPipeDes_RX.close ( );
            _errPipeDes_TX.close ( );
        }
        
        ::pid_t _pid { ::fork ( ) };
        
        if
            ( _pid < +0 )
        { }
        
        if
            ( _pid == +0 )
        {
            
            /* the child process transmits the data to us */
            
            /// close write/transmit (TX) unused pipe else it will hang the pipe
            _inPipeDes_TX.close ( );
            
            /// close read/reciver (RX) unused pipes else it will hang the pipe
            _outPipeDes_RX.close ( );
            _errPipeDes_RX.close ( );
            
            if
                ( ( pOpt_IO_ & opt_t::stdin ) )
            {
                /// Child process reads/recives (TX) data from stdin
                _inPipeDes_RX >>= STDIN_FILENO;
            }
            
            if
                ( ( pOpt_IO_ & opt_t::stdout ) )
            {
                /// Child process writes/transmits (TX) data to stdout
                _outPipeDes_TX >>= STDOUT_FILENO;
            }
            
            if
                ( ( pOpt_IO_ & opt_t::stderr ) )
            {
                /// Child process writes/transmits (TX) data to stderr
                _errPipeDes_TX >>= STDERR_FILENO;
            }
            
            // auto const
            //     _k_zu_argvMax
            //     { this->PMm_vecStr_argv.size ( ) + +3ZU }
            // ;
            
            // std::vector<char*> _args { };
            // _args.reserve ( _k_zu_argvMax );
            
            // for
            //     ( auto& arg_ : this->PMm_vecStr_argv )
            // {
            //     _args.emplace_back ( arg_.data ( ) );
            // }
            
            // _args.emplace_back ( nullptr );
            if
                ( this->PMm_argNSync )
            {
                
                this->mt_v_syncArgv ( );
                
                this->PMm_argNSync = false;
                
            }
            
            ::execvp
                (
                    this->PMm_vecStr_cArgv.front ( ) ,
                    this->PMm_vecStr_cArgv.data ( )
                )
            ;
            
            ::_exit ( +127 );
            
        }
        
        else
        {
            
            // we are not reading any data. so close the reading(RX) ends
            _inPipeDes_RX.close ( );
            
            // we are not transmitting any data. so close the write(TX) ends
            _outPipeDes_TX.close ( );
            _errPipeDes_TX.close ( );
            
            // set nonblocking to prevent freezing when RX/TX of large buffers
            _inPipeDes_TX.setNonBlock ( true );
            _outPipeDes_RX.setNonBlock ( true );
            _errPipeDes_RX.setNonBlock ( true );
            
            constexpr std::size_t const _kK_zu_bufSz { +64 << +10 };
            
            string_t _str_outResData; _str_outResData.reserve ( _kK_zu_bufSz );
            string_t _str_errResData; _str_errResData.reserve ( _kK_zu_bufSz );
            
            while
                (
                    (
                        ( pOpt_IO_ & opt_t::stdin ) &&
                        not _inPipeDes_TX.closed ( )
                    ) ||
                    (
                        ( pOpt_IO_ & opt_t::stdout ) &&
                        not _outPipeDes_RX.closed ( )
                    ) ||
                    (
                        ( pOpt_IO_ & opt_t::stderr ) &&
                        not _errPipeDes_RX.closed ( )
                    )
                )
            {
                
                [[maybe_unused]] bool const k_b_inRx
                    { _inPipeDes_TX << this->PMm_str_inputData }
                ;
                
                [[maybe_unused]] bool const k_b_outRx
                    { _outPipeDes_RX >> _str_outResData }
                ;
                
                [[maybe_unused]]
                bool const k_b_errRx
                    { _errPipeDes_RX >> _str_errResData }
                ;
                
                //! TODO: Implement read/write failure logic !//
                
            }
            
            int _i_status { };
            
            ::waitpid ( _pid , &_i_status , +0 );
            
            this->PMm_str_inputData.clear ( );
            
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
            string_t const& kr_str_cmdName_ ,
            vecCxStr_t const& kr_vecStr_argv_
        )
    -> SubProcess
    {
        
        return
            (
                SubProcess
                (
                    kr_str_cmdName_ ,
                    kr_vecStr_argv_
                )
            )
        ;
        
    }
    
    auto SubProcess::
        mt_v_printRecursive
        ( bool const is_first_caller ) const
    -> void
    {
        if
            ( this->PMmkp_sbp_prevCmd )
        {
            
            this->PMmkp_sbp_prevCmd->mt_v_printRecursive ( !is_first_caller );
            std::println ( stderr , "\n{:>12}| (stdout -> stdin)" , "" );
            std::println ( stderr , "{:>12}v" , "" );
            
        }
    
        std::println ( stderr , "[Command]: {:?}" , this->PMm_str_command );
        
        if
            ( this->PMm_vecStr_argv.size ( ) < 2ZU )
        {
            std::println
                (
                    stderr ,
                    "{0:>4}+ arg[{1:}]: {0:?}"
                    , "" , this->PMm_vecStr_argv.size ( ) - ( +1ZU )
                )
            ;
            std::println ( "{:>4}- No arguments passed!" , "" );
        }
        
        for
            ( size_t idx { 1ZU }; idx < PMm_vecStr_argv.size ( ); ++idx )
        {
            
            std::println
                (
                    stderr ,
                    "{:>4}+ arg[{}]: {:?}"
                    , "" , idx - ( +1ZU ) , this->PMm_vecStr_argv [ idx ]
                )
            ;
        }
        
    }
    
    auto SubProcess::printArgs
        ( void /* v_ */ ) const
    -> void
    {
        
        std::println ( stderr , "::[ DEBUG SESSION BEGIN ]::" );
        
        this->mt_v_printRecursive ( true );
        
        std::println ( stderr , "::[ DEBUG SESSION END ]::" );
        
        std::println ( stderr );
        std::println ( stderr , "{:=^80}" , "" );
        
    }
    
    auto SubProcess::capture
        ( opt_t const opt_ ) const
    -> SubProcess const&
    {
        if
            ( !opt_ )
        {
            return ( *this );
        }
        
        this->PMm_pOpt_opt |= opt_;
        
        return ( *this );
    }
    
    auto SubProcess::run
        ( void /* v_ */ ) const
    -> int
    {
        
        this->PMm_pRes_cmdResult =
            this->SubProcess::mt_Res_execute ( this->PMm_pOpt_opt )
        ;
        
        return
            (
                this->PMm_pRes_cmdResult.exit_code ( )
            )
        ;
        
    }
    
    auto SubProcess::operator ( )
        ( string_t const& kr_str_cmdName_ ) const
    -> SubProcess const&
    {
        
        if
            ( this->PMm_str_command == kr_str_cmdName_ )
        {
            return ( *this );
        }
        
        this->PMm_str_command = kr_str_cmdName_;
        
        this->PMm_vecStr_argv.front ( ) = kr_str_cmdName_;
        
        this->PMm_argNSync = true;
        
        return ( *this );
        
    }
    
    auto SubProcess::operator [ ]
        ( string_t const& kr_str_argv_ ) const
    -> SubProcess const&
    {
        
        if
            (
                auto const&
                    kr_aut_itr
                    {
                        std::find
                        (
                            this->PMm_vecStr_argv.begin ( ) + 1ZU ,
                            this->PMm_vecStr_argv.end ( ) ,
                            kr_str_argv_
                        )
                    }
                ; kr_aut_itr == this->PMm_vecStr_argv.end ( )
            )
        {
            
            this->PMm_vecStr_argv.emplace_back ( kr_str_argv_ );
            
            this->PMm_argNSync = true;
            
        }
        
        
        return ( *this );
        
    }
    
    auto SubProcess::operator |
        ( SubProcess const& kr_sbp_next_ ) const
    -> SubProcess const&
    {
        
        kr_sbp_next_.PMmkp_sbp_prevCmd = this;
        
        this->capture ( opt_t::stdout ).run ( );
        
        kr_sbp_next_.capture ( opt_t::stdin );
        
        kr_sbp_next_.PMm_str_inputData = this->result ( ).stdOut ( );
        
        return kr_sbp_next_;
        
    }
    
} /* namespace uniqx */
