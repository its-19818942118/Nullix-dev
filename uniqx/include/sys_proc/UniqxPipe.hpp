/* #pragma once */

#ifndef UNIQX_PIPE_HPP_UNIQX_
    #define UNIQX_PIPE_HPP_UNIQX_
    
    #include "sys_proc/SysProc.hpp"
    #include "sys_proc/UniqxPd.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        struct [[
            /* nullAttr_ */
        ]] SysProc::UniqxPipe
            {
                
                /// pipe descriptor reciever (RX)
                UniqxPd pipeDes_RX;
                /// pipe descriptor transmitter (TX)
                UniqxPd pipeDes_TX;
                
                auto static create
                    ( void /* v_ */ )
                -> UniqxPipe;
            }
        ;
        
        
    } /* namespace uniqx */
    
#endif /* UNIQX_PIPE_HPP_UNIQX_ */
