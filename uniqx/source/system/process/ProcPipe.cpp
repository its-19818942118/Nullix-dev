#include <array>
#include <print>
#include <cerrno>
#include <cstdlib>

#include "process/ProcPipe.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    using namespace system;
    
    auto process::
        ProcPipe::create
        ( void /* v_ */ )
    -> ProcPipe
    {
        
        constexpr int const _kK_i_pipeDes_RX { +0 };
        constexpr int const _kK_i_pipeDes_TX { +1 };
        constexpr int const _kK_i_pipeDesLim { +2 };
        
        std::array<int , _kK_i_pipeDesLim> _i_arr_pipeDes { };
        
        if
            ( ::pipe ( _i_arr_pipeDes.data ( ) ) == -1 )
        {
            std::println ( stderr , "[Err]: Unable to create pipe" );
            std::exit ( errno );
        }
        
        return
            {
                
                .pm_pipeDes_RX
                {
                    _i_arr_pipeDes.at ( _kK_i_pipeDes_RX )
                } ,
                
                .pm_pipeDes_TX
                {
                    _i_arr_pipeDes.at ( _kK_i_pipeDes_TX )
                }
                
            }
        ;
        
    }
    
} /* namespace uniqx */
