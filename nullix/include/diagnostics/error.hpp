/* #pragma once */

#ifndef ERROR_HPP_NULLIX_TA9_GC
    #define ERROR_HPP_NULLIX_TA9_GC
    
    #include <string>
    #include <source_location>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_NULLIX_TA9_GC_
    #
    #   include "nullix.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_NULLIX_TA9_GC_
    
    namespace
        nullix::err
    {
        namespace [[
            deprecated
            ( "Please use `nullix::err:v2` instead" )
        ]] v1
        {
            struct [[
                deprecated
                ( "Please use `nullix::err::v2::Error` instead" )
            ]] Error
                {
                    
                    public: std::string _errWhat { };
                    public: std::source_location _errWhere { };
                    
                    public: explicit
                        CLASS_CTOR Error
                        (
                            std::string const& /* kr_str_errWhat_ */ ,
                            std::source_location const& /* kr_sl_errWhere_ */
                        )
                    ;
                    
                    constexpr auto
                        mt_str_value_or_log
                        ( std::string const& /* kr_s_fallback_ */ ) const
                    -> std::string;
                    
                }
            ;
            
            struct
            [[
                deprecated
                ( "Please use `nullix::err::v2::ErrorCode` instead" )
            ]]
            ErrorInt :
                public Error
                {
                    // using `long` here to satisfy
                    // the compiler's (-Wpadded) warnings
                    public: long _errCode { };
                    
                    public: explicit
                        CLASS_CTOR ErrorInt
                        (
                            long const /* k_errCode_ */ ,
                            std::string const& /* kr_str_errWhat_ */ ,
                            std::source_location const& /* kr_sl_errWhere_ */
                        )
                    ;
                    
                    public: constexpr auto
                        mt_str_value_or_log
                        ( std::string const& fallback_ ) const
                    -> std::string;
                    
                }
            ;
        }
    }
    
    namespace
        nullix::err::v2::detail
    {
        
        [[
            nodiscard ( "Important: returns sanitized kr_str_errWhat_" )
        ]] auto sanErr
            ( std::string_view errWhatRaw_ )
        -> std::string;
        
    } /* namespace nullix::err::v2::detail */
    
    namespace
        nullix::err::v2
    {
        
        class
            Error
            {
                
                // use long to satisfy compiler's (-Wpadded) warnings
                public: enum class
                    e_ErrType : long
                    {
                        Fatal ,
                        Warning ,
                        Unknown ,
                    }
                ;
                
                /* The error type like e_ErrType::Fatal */
                protected: e_ErrType const Pmk_errType { };
                /* The error reason variable to store the error reason */
                protected: std::string const Pmk_errWhat { };
                /* The formatted error reason variable sanitized */
                protected: std::string const Pmkr_str_errWhat_ { };
                /* The variable to store callsite error */
                protected: std::source_location const Pmk_errWhereCaller { };
                /* The variable to store function that made the error */
                protected: std::source_location const Pmk_errWhereOrigin { };
                
                // not marking all these `public:` (i forgot to) costed me 2hrs
                // of debugging. always set accessor explicitly
                public: CLASS_CTOR
                    Error ( Error&& ) = default
                ;
                public: CLASS_CTOR
                    Error ( Error const& ) = default
                ;
                
                public: auto
                    operator =
                    ( Error&& )
                -> Error& = delete ( "disable mv/cp" );
                public: auto
                    operator =
                    ( Error const& )
                -> Error& = delete ( "disable mv/cp" );
                
                /* The constructor to initialize the values */
                public: explicit
                    CLASS_CTOR Error
                    (
                        e_ErrType const /* errType_ */ ,
                        std::string const /* kr_str_errWhat_ */ ,
                        std::source_location const /* k_sl_errWhereCaller_ */ ,
                        std::source_location const /* k_sl_errWhereOrigin_ */ =
                        { std::source_location::current ( ) }
                    )
                ;
                
                public: [[ nodiscard
                    ( "Important: returns a formatted log string" )
                ]] virtual auto
                    mt_str_errLog
                    ( void /* v_ */ ) const
                -> std::string;
                
                public: [[ nodiscard
                    ( "Important: returns a fallback_ value" ) ,
                ]] auto
                    mt_str_fallback
                    ( std::string const& fallback_ ) const
                -> std::string;
                
                public: [[ nodiscard
                    ( "Important: prints error & returns a fallback_ value" ) ,
                ]] auto
                    mt_str_errLog_or
                    ( std::string const& fallback_ ) const
                -> std::string;
                
                // will implment later
                virtual CLASS_DTOR ~Error ( );
                
            }
        ;
        
        class
            ErrorCode :
            public Error
            {
                
                // use long to satisfy compiler's (-Wpadded) warnings
                long const pmk_errCode_ { };
                
                public: explicit
                    CLASS_CTOR ErrorCode
                    (
                        long const /* errCode_ */ ,
                        e_ErrType const /* errType_ */ ,
                        std::string const /* kr_str_errWhat_ */ ,
                        std::source_location const /* k_sl_errWhereCaller_ */ ,
                        std::source_location const /* k_sl_errWhereOrigin_ */ =
                        { std::source_location::current ( ) }
                    )
                ;
                
                public: [[ nodiscard
                    ( "Important: returns a formatted log string" )
                ]] auto
                    mt_str_errLog
                    ( void /* v_ */ ) const
                -> std::string override;
                public: constexpr
                    explicit operator int
                    ( void /* v_ */ ) const
                noexcept ( true );
                
                public: constexpr
                    explicit operator long
                    ( void /* v_ */ ) const
                noexcept ( true );
                
                public: constexpr
                    explicit operator bool
                    ( void /* v_ */ ) const
                noexcept ( true );
                
            }
        ;
        
        
        
    }
    
#endif /* ERROR_HPP_NULLIX_TA9_GC */
