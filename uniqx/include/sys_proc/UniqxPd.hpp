/* #pragma once */

#ifndef UNIQX_PD_HPP_UNIQX_
    #define UNIQX_PD_HPP_UNIQX_
    
    #include "globals.hpp"
    #include "sys_proc/SysProc.hpp"
    
    namespace [[
        
        ]] uniqx
    {
        
        struct [[
            
        ]] SysProc::UniqxPd
            {
                
                private: int PM_i_pd { -1 };
                
                /// default constructor
                public: STRUCT_CTOR UniqxPd
                    ( void /* v_ */ )
                = default;
                
                /// delete copy constructor
                public: STRUCT_CTOR UniqxPd
                    ( const UniqxPd& )
                = delete ( "disable copy" );
                
                /// delete copy operations
                public: auto operator =
                    ( const UniqxPd& )
                -> UniqxPd& = delete ( "disable copy" );
                
                /// explicit constructor with initializer
                public: explicit
                    STRUCT_CTOR UniqxPd
                    ( const int /* k_i_pd_ */ )
                ;
                
                /// move constructor
                public: STRUCT_CTOR UniqxPd
                    ( UniqxPd&& /* Upd_otherPd_ */ ) noexcept
                ;
                
                /// move operations
                public: auto operator =
                    ( UniqxPd&& /* Upd_otherPd_ */ ) noexcept
                -> UniqxPd&;
                
                /// release the owned pd to prevent destructor from firing
                auto
                    release
                    ( void /* v_ */ )
                -> int;
                
                /// redirect owned pd
                auto
                    redirectTo
                    ( const int /* i_targetPd_ */ )
                -> void;
                
                /// read data from the owned pd
                auto
                    getData
                    ( void /* v_ */ )
                -> std::string;
                
                /// close the owned pd
                auto
                    close
                    ( void /* v_ */ )
                -> void;
                
                /// conversion operator int
                operator int
                    ( void /* v_ */ ) const
                ;
                
                /// destructor for clean up
                public: STRUCT_DTOR ~UniqxPd
                    ( void /* v_ */ )
                ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* UNIQX_PD_HPP_UNIQX_ */
