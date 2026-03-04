#include <array>
#include <cstdlib>
#include <print>

#include "sys_proc/UniqxPipe.hpp"

namespace
    uniqx
{
    
    auto SysProc::
        UniqxPipe::create
        ( void /* v_ */ )
    -> UniqxPipe
    {
        
        constexpr int K_i_pipeDes_RX { +0L };
        constexpr int K_i_pipeDes_TX { +1L };
        constexpr int K_i_pipeDesLim { +2L };
        
        std::array<int , K_i_pipeDesLim> _i_arr_pipeDes { };
        
        if
            ( ::pipe ( _i_arr_pipeDes.data ( ) ) == -1L )
        {
            std::println ( stderr , "[Err]: Unable to create pipe" );
            std::exit ( errno );
        }
        
        return
            {
                .pipeDes_RX
                {
                    UniqxPd
                    {
                        _i_arr_pipeDes.at ( K_i_pipeDes_RX )
                    }
                } ,
                .pipeDes_TX
                {
                    UniqxPd
                    {
                        _i_arr_pipeDes.at ( K_i_pipeDes_TX )
                    }
                }
            }
        ;
        
    }
    
} /* namespace uniqx */
