/*******************************************************************************
# @author: izumi (its_19818942118)
# @address: https://github.com/its-19818942118
# @contributors: 
*******************************************************************************/

/* #pragma once */

#ifndef BACKLIGHT_HPP_TA9_GC
    #define BACKLIGHT_HPP_TA9_GC
    
    #include <cstdint>
    #include <utility>
    #include <filesystem>
    
    namespace
        gctl::scripts
    {
        
        #define NULL_ /* NULL */
        
        #define CONSTRUCTOR_ NULL_ /* A NULL _Constructor_ Type */
        #define DESTRUCTOR_ NULL_ /* A NULL Destrcutor Type */
        
        #define CLASS_CTOR CONSTRUCTOR_ /* call the NULL _CONSTRUCTOR_ */
        #define CLASS_DTOR DESTRUCTOR_ /* call the NULL _DESTRUCTOR_ */
        
        using PairInt64_t = std::pair <int64_t , int64_t>;
        
        namespace fs = std::filesystem;
        
        class Backlight
            {
                
                protected: fs::path
                    mut_fsp_Pm_devicePath
                ;
                
                protected: virtual auto
                    mt_fsp_brightnessPath
                    ( void ) const -> fs::path = 0
                ;
                
                protected: virtual auto
                    mt_fsp_maxBrightnessPath
                    ( void ) const -> fs::path = 0
                ;
                
                protected: virtual auto
                    mt_i64_readInt
                    ( const fs::path& /* cst_fsp_ref_path_ */ ) const -> int64_t
                ;
                
                protected: virtual auto
                    mt_i64_writeInt
                    (
                        const fs::path& /* cst_fsp_ref_path_ */ ,
                        int64_t /* i64_value_ */
                    ) -> void
                ;
                
                public: virtual CLASS_DTOR
                    ~Backlight
                    ( void /* v_ */ ) = default
                ;
                
                public: auto
                    mtGet_brightness
                    ( void /* v_ */ ) const -> PairInt64_t
                ;
                
                public: auto
                    mt_adjustBrightness
                    ( const int64_t /* cst_i64_delta_ */ ) -> void
                ;
                
                public: auto
                    mt_adjustBrightnessPercent
                    ( const int64_t /* cst_i64_delta_percent_ */ ) -> void
                ;
                
            }
        ;
        
        class ScreenBacklight : public Backlight
            {
                
                public: CLASS_CTOR
                    ScreenBacklight
                    ( const fs::path& /* cst_fsp_ref_sysDevPath_ */ )
                ;
                
                protected: auto
                    mt_fsp_brightnessPath
                    ( void /* v_ */ ) const -> fs::path override
                ;
                
                protected: auto
                    mt_fsp_maxBrightnessPath
                    ( void /* v_ */ ) const -> fs::path override
                ;
                
            }
        ;
        
        class KeyboardBacklight : public Backlight
            {
                
                public: CLASS_CTOR
                    KeyboardBacklight
                    ( const fs::path& /* cst_fsp_ref_sysDevPath_ */ )
                ;
                
                protected: auto
                    mt_fsp_brightnessPath
                    ( void /* v_ */ ) const -> fs::path override
                ;
                
                protected: auto
                    mt_fsp_maxBrightnessPath
                    ( void /* v_ */ ) const -> fs::path override
                ;
                
                protected: auto
                    mt_adjustBrightnessPercent
                    ( const int64_t /* cst_i64_delta_percent_ */ ) const
                -> void = delete;
                
            }
        ;
        
    }
    
    #ifdef BACKLIGHT_TA9_GC_HEADER_ONLY
        #define BACKLIGHT_TA9_HEADER_ONLY_DEFINED
        
        #include <fstream>
        #include <stdexcept>
        
        namespace
            gctl::scripts
        {
            
            auto inline
                Backlight::
                mt_i64_readInt
                ( const fs::path& cst_fsp_ref_path_ ) const
            -> int64_t
            {
                
                std::ifstream _ifs_sysFile { cst_fsp_ref_path_ };
                
                if
                    ( !_ifs_sysFile )
                {
                    
                    throw
                        std::runtime_error
                        (
                            "[Err]: Failed to open file " +
                            cst_fsp_ref_path_.string ( )
                        )
                    ;
                    
                }
                
                int64_t _i64_value { };
                
                _ifs_sysFile >> _i64_value;
                
                return _i64_value;
                
            }
            
            auto inline
                Backlight::
                mt_i64_writeInt
                ( const fs::path& cst_fsp_ref_path_ , int64_t i64_value_ ) const
            -> void
            {
                
                std::ofstream _ofs_sysFile { cst_fsp_ref_path_ };
                
                if
                    ( !_ofs_sysFile )
                {
                    
                    throw
                        std::runtime_error
                        (
                            "[Err]: Failed to open file " +
                            cst_fsp_ref_path_.string ( )
                        )
                    ;
                    
                }
                
                _ofs_sysFile << i64_value_;
                
            }
            
            auto inline
                Backlight::
                mtGet_brightness
                ( void /* v_ */ ) const
            -> PairInt64_t
            {
                
                int64_t
                    cst_i64_brightness
                    {
                        this->mt_i64_readInt
                        ( this->mt_fsp_brightnessPath ( ) )
                    }
                ;
                
                int64_t
                    cst_i64_maxBrightess
                    {
                        this->mt_i64_readInt
                        ( this->mt_fsp_maxBrightnessPath ( ) )
                    }
                ;
                
                return { cst_i64_brightness , cst_i64_maxBrightess };
                
            }
            
            auto inline
                Backlight::
                mt_adjustBrightness
                ( const int64_t cst_i64_delta_ ) const
            -> void
            {
                
                // structured binding not lamda function
                const auto
                    [ cst_i64_brightness , cst_i64_maxBrightess ]
                    {
                        this->mtGet_brightness ( )
                    }
                ;
                
                const auto
                    cst_i64_newBrightness
                    {
                        std::clamp
                        (
                            ( cst_i64_brightness + cst_i64_delta_ ) , 0L ,
                            cst_i64_maxBrightess
                        )
                    }
                ;
                
                this->mt_i64_writeInt
                    (
                        this->mt_fsp_brightnessPath ( ) ,
                        cst_i64_newBrightness
                    )
                ;
                
            }
            
            auto inline
                Backlight::
                mt_adjustBrightnessPercent
                ( const int64_t cst_i64_delta_percent_ ) const
            -> void
            {
                
                const auto
                    [ cst_i64_brightness , cst_i64_maxBrightness ]
                    {
                        this->mtGet_brightness ( )
                    }
                ;
                
                // convert the raw cst_i64_brightness to percent %
                constexpr double cxp_percent { 100.0 };
                const double
                    cst_d_brightnessPercent
                    {
                        static_cast <double> ( cst_i64_brightness ) *
                        cxp_percent / cst_i64_maxBrightness
                    }
                ;
                
                double
                    _d_deltaBrightnessPrecent
                    {
                        cst_d_brightnessPercent +
                        static_cast <double> ( cst_i64_delta_percent_ ) 
                    }
                ;
                
                _d_deltaBrightnessPrecent =
                    std::clamp
                    ( _d_deltaBrightnessPrecent , 0.0 , 100.0 )
                ;
                
                const int64_t
                    cst_i64_newBrightnessPercent
                    {
                        
                        static_cast <int64_t>
                        (
                            _d_deltaBrightnessPrecent *
                            cst_i64_maxBrightness / 100.0 + 0.5
                        )
                        
                    }
                ;
                
                this->mt_i64_writeInt ( this->mt_fsp_brightnessPath ( ) , cst_i64_newBrightnessPercent );
                
            }
            
            CLASS_CTOR inline
                ScreenBacklight::
                ScreenBacklight
                ( const fs::path& cst_fsp_ref_sysDevPath_ )
            {
                this->mut_fsp_Pm_devicePath = cst_fsp_ref_sysDevPath_;
            }
            
            auto inline
                ScreenBacklight::
                mt_fsp_brightnessPath
                ( void /* v_ */ ) const
            -> fs::path
            {
                
                return { this->mut_fsp_Pm_devicePath / "brightness" };
                
            }
            
            auto inline
                ScreenBacklight::
                mt_fsp_maxBrightnessPath
                ( void /* v_ */ ) const
            -> fs::path
            {
                
                return { this->mut_fsp_Pm_devicePath / "max_brightness" };
                
            }
            
            CLASS_CTOR inline
                KeyboardBacklight::
                KeyboardBacklight
                ( const fs::path& cst_fsp_ref_sysDevPath_ )
            {
                this->mut_fsp_Pm_devicePath = cst_fsp_ref_sysDevPath_;
            }
            
            auto inline
                KeyboardBacklight::
                mt_fsp_brightnessPath
                ( void /* v_ */ ) const
            -> fs::path
            {
                
                return { this->mut_fsp_Pm_devicePath / "brightness" };
                
            }
            
            auto inline
                KeyboardBacklight::
                mt_fsp_maxBrightnessPath
                ( void /* v_ */ ) const
            -> fs::path
            {
                
                return { this->mut_fsp_Pm_devicePath / "max_brightness" };
                
            }
            
        }
        
    #endif /* BACKLIGHT_IMPLEMENTATION_TA9_GC */
    
#endif /* BACKLIGHT_HPP_TA9_GC */
