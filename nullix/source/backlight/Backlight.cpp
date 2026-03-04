#if \
    !defined BACKLIGHT_TA9_GC_HEADER_ONLY || \
    !defined BACKLIGHT_IMPLEMENTATION_TA9_GC || \
    !defined BACKLIGHT_TA9_HEADER_ONLY_DEFINED
    
    // #undef BACKLIGHT_IMPLEMENTATION_TA9_GC
    // #define BACKLIGHT_IMPLEMENTATION_TA9_GC
    
    #include <cstdint>
    #include <fstream>
    #include <algorithm>
    #include <stdexcept>
    #include <diagnostics/Diagnostics.hpp>
    
    #include "backlight/Backlight.hpp"
    
    // DIAGNOSTICS_IGNORED("-Weverything")
    
    namespace
        gctl::scripts
    {
        
        auto
            Backlight::
            mt_i64_readInt
            ( const fs::path& k_fsp_ref_path_ ) const
        -> int64_t
        {
            
            std::ifstream _ifs_sysFile { k_fsp_ref_path_ };
            
            if
                ( !_ifs_sysFile )
            {
                
                throw
                    std::runtime_error
                    (
                        "[Err]: Failed to open file " +
                        k_fsp_ref_path_.string ( )
                    )
                ;
                
            }
            
            int64_t _i64_value { };
            
            _ifs_sysFile >> _i64_value;
            
            return _i64_value;
            
        }
        
        auto
            Backlight::
            mt_i64_writeInt
            ( const fs::path& k_fsp_ref_path_ , int64_t i64_value_ )
        -> void
        {
            
            std::ofstream _ofs_sysFile { k_fsp_ref_path_ };
            
            if
                ( !_ofs_sysFile )
            {
                
                throw
                    std::runtime_error
                    (
                        "[Err]: Failed to open file " +
                        k_fsp_ref_path_.string ( )
                    )
                ;
                
            }
            
            _ofs_sysFile << i64_value_;
            
        }
        
        auto
            Backlight::
            mtGet_brightness
            ( void /* v_ */ ) const
        -> PairInt64_t
        {
            
            int64_t
                k_i64_brightness
                {
                    this->mt_i64_readInt
                    ( this->mt_fsp_brightnessPath ( ) )
                }
            ;
            
            int64_t
                k_i64_maxBrightess
                {
                    this->mt_i64_readInt
                    ( this->mt_fsp_maxBrightnessPath ( ) )
                }
            ;
            
            return { k_i64_brightness , k_i64_maxBrightess };
            
        }
        
        auto
            Backlight::
            mt_adjustBrightness
            ( const int64_t k_i64_delta_ )
        -> void
        {
            
            // structured binding not lamda function
            const auto
                [ k_i64_brightness , k_i64_maxBrightess ]
                {
                    this->mtGet_brightness ( )
                }
            ;
            
            const auto
                k_i64_newBrightness
                {
                    std::clamp
                    (
                        ( k_i64_brightness + k_i64_delta_ ) , 0L ,
                        k_i64_maxBrightess
                    )
                }
            ;
            
            this->mt_i64_writeInt
                (
                    this->mt_fsp_brightnessPath ( ) ,
                    k_i64_newBrightness
                )
            ;
            
        }
        
        auto
            Backlight::
            mt_adjustBrightnessPercent
            ( const int64_t k_i64_delta_percent_ )
        -> void
        {
            
            const auto
                [ k_i64_brightness , k_i64_maxBrightness ]
                {
                    this->mtGet_brightness ( )
                }
            ;
            
            // convert the raw k_i64_brightness to percent %
            // constexpr double cxp_percent { 100.0 };
            constexpr double K_percent { 100.0 };
            const double
                k_d_brightnessPercent
                {
                    static_cast <double> ( k_i64_brightness ) *
                    // cxp_percent / k_i64_maxBrightness
                    K_percent / k_i64_maxBrightness
                }
            ;
            
            double
                _d_deltaBrightnessPrecent
                {
                    k_d_brightnessPercent +
                    static_cast <double> ( k_i64_delta_percent_ ) 
                }
            ;
            
            _d_deltaBrightnessPrecent =
                std::clamp
                ( _d_deltaBrightnessPrecent , 0.0 , 100.0 )
            ;
            
            const int64_t
                k_i64_newBrightnessPercent
                {
                    
                    static_cast <int64_t>
                    (
                        _d_deltaBrightnessPrecent *
                        k_i64_maxBrightness / 100.0 + 0.5
                    )
                    
                }
            ;
            
            this->mt_i64_writeInt ( this->mt_fsp_brightnessPath ( ) , k_i64_newBrightnessPercent );
            
        }
        
        CLASS_CTOR
            ScreenBacklight::
            ScreenBacklight
            ( const fs::path& k_fsp_ref_sysDevPath_ )
        {
            this->mut_fsp_Pm_devicePath = k_fsp_ref_sysDevPath_;
        }
        
        auto
            ScreenBacklight::
            mt_fsp_brightnessPath
            ( void /* v_ */ ) const
        -> fs::path
        {
            
            return { this->mut_fsp_Pm_devicePath / "brightness" };
            
        }
        
        auto
            ScreenBacklight::
            mt_fsp_maxBrightnessPath
            ( void /* v_ */ ) const
        -> fs::path
        {
            
            return { this->mut_fsp_Pm_devicePath / "max_brightness" };
            
        }
        
        CLASS_CTOR
            KeyboardBacklight::
            KeyboardBacklight
            ( const fs::path& k_fsp_ref_sysDevPath_ )
        {
            this->mut_fsp_Pm_devicePath = k_fsp_ref_sysDevPath_;
        }
        
        auto
            KeyboardBacklight::
            mt_fsp_brightnessPath
            ( void /* v_ */ ) const
        -> fs::path
        {
            
            return { this->mut_fsp_Pm_devicePath / "brightness" };
            
        }
        
        auto
            KeyboardBacklight::
            mt_fsp_maxBrightnessPath
            ( void /* v_ */ ) const
        -> fs::path
        {
            
            return { this->mut_fsp_Pm_devicePath / "max_brightness" };
            
        }
        
    }
    
#endif /* BACKLIGHT_TA9_GC_HEADER_ONLY || \
        BACKLIGHT_IMPLEMENTATION_TA9_GC || \
        BACKLIGHT_TA9_HEADER_ONLY_DEFINED
       */
    #
#
