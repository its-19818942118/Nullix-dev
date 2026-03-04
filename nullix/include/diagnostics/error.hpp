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
                            const std::string& /* errWhat_ */ ,
                            const std::source_location& /* errWhere_ */
                        )
                    ;
                    
                    constexpr auto
                        mt_str_value_or_log
                        ( const std::string& /* fallback_ */ ) const -> std::string
                    ;
                    
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
                            const long& /* errCode_ */ ,
                            const std::string& /* errWhat_ */ ,
                            const std::source_location& /* errWhere_ */
                        )
                    ;
                    
                    public: constexpr auto
                        mt_str_value_or_log
                        ( const std::string& fallback_ ) const -> std::string
                    ;
                    
                }
            ;
        }
    }
    
    namespace
        nullix::err::v2::detail
    {
        
        [[
            nodiscard ( "Important: returns sanitized errWhat_" )
        ]] auto
            sanErr
            ( std::string_view errWhatRaw_ ) -> std::string
        ;
        
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
                protected: const e_ErrType kPm_errType { };
                /* The error reason variable to store the error reason */
                protected: const std::string kPm_errWhat { };
                /* The formatted error reason variable sanitized */
                protected: const std::string kPm_errWhat_ { };
                /* The variable to store callsite error */
                protected: const std::source_location kPm_errWhereCaller { };
                /* The variable to store function that made the error */
                protected: const std::source_location kPm_errWhereOrigin { };
                
                // not marking all these `public:` (i forgot to) costed me 2hrs
                // of debugging. always set accessor explicitly
                public: CLASS_CTOR
                    Error ( Error&& ) = default
                ;
                public: CLASS_CTOR
                    Error ( const Error& ) = default
                ;
                
                public: auto
                    operator =
                    ( Error&& )
                -> Error& = delete ( "disable mv/cp" );
                public: auto
                    operator =
                    ( const Error& )
                -> Error& = delete ( "disable mv/cp" );
                
                /* The constructor to initialize the values */
                public: explicit
                    CLASS_CTOR Error
                    (
                        const e_ErrType /* errType_ */ ,
                        const std::string /* errWhat_ */ ,
                        const std::source_location /* errWhereCaller_ */ ,
                        const std::source_location /* errWhereOrigin_ */ =
                        { std::source_location::current ( ) }
                    )
                ;
                
                public: [[ nodiscard
                    ( "Important: returns a formatted log string" )
                ]] virtual auto
                    mt_str_errLog
                    ( void /* v_ */ ) const -> std::string
                ;
                
                public: [[ nodiscard
                    ( "Important: returns a fallback_ value" ) ,
                ]] auto
                    mt_str_fallback
                    ( const std::string& fallback_ ) const -> std::string
                ;
                
                public: [[ nodiscard
                    ( "Important: prints error & returns a fallback_ value" ) ,
                ]] auto
                    mt_str_errLog_or
                    ( const std::string& fallback_ ) const -> std::string
                ;
                
                // will implment later
                virtual CLASS_DTOR ~Error ( );
                
            }
        ;
        
        class
            ErrorCode :
            public Error
            {
                
                // use long to satisfy compiler's (-Wpadded) warnings
                const long kpm_errCode_ { };
                
                public: explicit
                    CLASS_CTOR ErrorCode
                    (
                        const long /* errCode_ */ ,
                        const e_ErrType /* errType_ */ ,
                        const std::string /* errWhat_ */ ,
                        const std::source_location /* errWhereCaller_ */ ,
                        const std::source_location /* errWhereOrigin_ */ =
                        { std::source_location::current ( ) }
                    )
                ;
                
                public: [[ nodiscard
                    ( "Important: returns a formatted log string" )
                ]] auto
                    mt_str_errLog
                    ( void /* v_ */ ) const
                -> std::string override;
                
                public: explicit
                    constexpr operator int
                    ( void /* v_ */ ) const noexcept
                ;
                
                public: explicit
                    constexpr operator long
                    ( void /* v_ */ ) const noexcept
                ;
                
                public: explicit
                    constexpr operator bool
                    ( void /* v_ */ ) const noexcept
                ;
                
            }
        ;
        
        
        
    }
    
#endif /* ERROR_HPP_NULLIX_TA9_GC */
