/* #pragma once */

#ifndef ERROR_HPP_ERRIKA_
    #define ERROR_HPP_ERRIKA_
    
    #include <string>
    #include <source_location>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
    #
    #   include "errika/errika.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
    
    namespace
        errika::error
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
                protected: e_ErrType Pm_errType { };
                /* The error reason variable to store the error reason */
                protected: std::string Pm_errWhat { };
                /* The formatted error reason variable sanitized */
                protected: std::string Pm_str_errWhat_ { };
                /* The variable to store callsite error */
                protected: std::source_location Pm_errWhereCaller { };
                /* The variable to store function that made the error */
                protected: std::source_location Pm_errWhereOrigin { };
                
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
                -> Error& = delete ( "disable mv/cp operations" );
                public: auto
                    operator =
                    ( Error const& )
                -> Error& = delete ( "disable mv/cp operations" );
                
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
                    errReport
                    ( void /* v_ */ ) const
                -> std::string;
                
                public: [[ nodiscard
                    ( "Important: returns a fallback_ value" ) ,
                ]] auto
                    fallback
                    ( std::string const& fallback_ ) const
                -> std::string;
                
                public: [[ nodiscard
                    ( "Important: prints error & returns a fallback_ value" ) ,
                ]] auto
                    logReport_and
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
                long pm_errCode_ { };
                
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
                    errReport
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
    
#endif /* ERROR_HPP_ERRIKA_ */
