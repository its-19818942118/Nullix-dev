#include <print>
#include <format>
#include <string>
#include <cstddef>
#include <source_location>

#define ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_
#
#   include "errika/errika.hpp"
#
#undef ONLY_FORWARD_SYMBOL_DECLS_ERRIKA_

#include "error/error.hpp"

namespace
    errika::error::_detail
{
    
    [[
        nodiscard ( "Important: returns sanitized kr_str_errWhat_" )
    ]] auto sanErr
        ( std::string_view errWhatRaw_ )
    -> std::string;
    
} /* namespace errika::error::v2::_detail */

namespace
    errika::error::_detail
{
    
    [[
        nodiscard
        ( "Important: returns sanitized kr_s_errWhat_" )
    ]] auto sanErr
        ( std::string_view errWhatRaw_ )
    -> std::string
    {
        
        if
            ( errWhatRaw_.empty ( ) ) [[ unlikely ]]
        {
            return { "No reason provided" };
        }
        
        using namespace std::literals;
        
        std::string _result { };
        _result.reserve ( errWhatRaw_.size ( ) + ( +64 ) );
        
        for
            (
                std::size_t _col { +15 };
                auto const& c : errWhatRaw_
            ) [[likely]]
        {
            if
                (
                    ( c == '\n' ) ||
                    ( _col >= +70 && c == ' ' ) || ( _col >= +80 )
                ) [[likely]]
            {
                _col = +15;
                _result.append ( "\n        #>> " );
                if ( c == '\n' ) continue;
            }
            _result.push_back ( c );
            ++_col;
        }
        
        return { _result };
        
    }
    
    
} /* namespace errika::error::v2::_detail */

namespace
    errika::error
{
    
    CLASS_CTOR
        Error::Error
            (
                e_ErrType const errType_ ,
                std::string const k_s_errWhat_ ,
                std::source_location const k_sl_errWhereCaller_ ,
                std::source_location const k_sl_errWhereOrigin_
            )
        : Pm_errType ( std::move ( errType_ ) )
        , Pm_errWhat ( std::move ( k_s_errWhat_ ) )
        , Pm_str_errWhat_ ( _detail::sanErr ( Pm_errWhat ) )
        , Pm_errWhereCaller ( std::move ( k_sl_errWhereCaller_ ) )
        , Pm_errWhereOrigin ( std::move ( k_sl_errWhereOrigin_ ) )
    { }
    
    [[
        nodiscard
        ( "Important: returns a formatted log string" )
    ]] auto
        Error::errReport
        ( void /* v_ */ ) const
    -> std::string
    {
        
        std::string_view _errLevel { };
        
        switch
            ( Pm_errType )
        {
            case
                e_ErrType::Fatal:
            {
                _errLevel = "Err.Fatal";
                break;
            }
            case
                e_ErrType::Warning:
            {
                _errLevel = "Err.Warnng";
                break;
            }
            case
                e_ErrType::Unknown:
            {
                _errLevel = "Err.unknown";
                break;
            }
        }
        
        auto const&
            _errLog
            {
                
                std::format
                (
                    "{0}\n"
                    "{0:=>80}\n"
                    "<[\x1b[1m{1}\x1b[0m]:\n"
                    "{0:2}{0:->78}\n"
                    "{0:4}<<[func.name]::\n"
                    "{0:8}|_~-> \x1b[1;4m\"{6}\"\x1b[0m\n"
                    "{0:4}::[func.name]>>\n"
                    "{0:2}{0:->78}\n"
                    "{0:4}<<[src.location]::\n"
                    "{0:6}{0:->74}\n"
                    "{0:8}<<[file.name]::\n"
                    "{0:12}|_~-> \x1b[1;3;4m\"{5}\"\x1b[0m\n"
                    "{0:8}::[file.name]>>\n"
                    "{0:10}{0:->70}\n"
                    "{0:8}<<[line.column]::\n"
                    "{0:12}|_~-> \x1b[1m#{3}:{4}\x1b[0m\n"
                    "{0:8}::[line.column]>>\n"
                    "{0:6}{0:->74}\n"
                    "{0:4}::[src.location]>>\n"
                    "{0:2}{0:->78}\n"
                    "{0:4}<<[what.reason]::\n"
                    // "{0:8}\x1b[1;3m#>> {2}\x1b[0m\n"
                    "{0:8}#>> {2}\n"
                    "{0:4}::[what.reason]>>\n"
                    "{0:2}{0:->78}\n"
                    ":[\x1b[1m{1}\x1b[0m]>\n"
                    "{0:=>80}\n"
                    , ""
                    , Pm_errWhat
                    , _errLevel
                    , Pm_errWhereOrigin.line ( )
                    , Pm_errWhereOrigin.column ( )
                    , Pm_errWhereOrigin.file_name ( )
                    , Pm_errWhereOrigin.function_name ( )
                )
                
            }
        ;
        
        return { _errLog };
        
    }
    
    [[
        nodiscard
        ( "Important: returns a fallback_ value" ) ,
    ]] auto
        Error::fallback
        ( std::string const& fallback_ ) const
    -> std::string
    {
        
        return { fallback_ };
        
    }
    
    [[
        nodiscard
        ( "Important: prints error & returns a fallback_ value" ) ,
    ]] auto
        Error::logReport_and
        ( std::string const& fallback_ ) const
    -> std::string
    {
        
        std::print ( stderr , "{}" , errReport ( ) );
        
        return { fallback_ };
        
    }
    
    Error::~Error ( ) = default;
    
    CLASS_CTOR ErrorCode::
        ErrorCode
            (
                long const errCode_ ,
                e_ErrType const errType_ ,
                std::string const kr_s_errWhat_ ,
                std::source_location const k_sl_errWhereCaller_ ,
                std::source_location const k_sl_errWhereOrigin_
            )
        : Error ( errType_ , kr_s_errWhat_ , k_sl_errWhereCaller_ , k_sl_errWhereOrigin_ )
        , pm_errCode_ ( std::move ( errCode_ ) )
    { }
    
    [[
        nodiscard
        ( "Important: returns a formatted log string" )
    ]] auto
        ErrorCode::errReport
        ( void /* v_ */ ) const
    -> std::string
    {
        
        using namespace std::literals;
        std::string_view _errLevel { };
        
        switch
            ( Pm_errType )
        {
            case
                e_ErrType::Fatal:
            {
                _errLevel = "Err::Fatal"sv;
                break;
            }
            case
                e_ErrType::Warning:
            {
                _errLevel = "Err::Warnng"sv;
                break;
            }
            case
                e_ErrType::Unknown:
            {
                _errLevel = "Err::unknown"sv;
                break;
            }
        }
        
        auto const&
            _errLog
            {
                
                std::format
                (
                    "<[{1}] --> {{\n"
                    "{0:>6} -+=> err_code ~> {{{2}}}\n"
                    "{0:>6} -+=> what_reason ~> '{3}'\n"
                    "{0:>4}<-[Err.Caller]: {{\n"
                    "{0:>6} .:ln:col ~> #{4}:{5}\n"
                    "{0:>6} .:file_name ~> \"{6}\"\n"
                    "{0:>6} .:fn_name ~> {7}\n"
                    "{0:>6} -+> src_loc ~> \"{6}#{4}:{5}\"\n"
                    "{0:>4}:[Err.Caller]-> }}\n"
                    "{0:>8}%{0:->64}%\n"
                    "{0:>4}<-[Err.Origin]: {{\n"
                    "{0:>6} .:ln:col ~> #{8}:{9}\n"
                    "{0:>6} .:file_name ~> \"{10}\"\n"
                    "{0:>6} .:fn_name ~> {11}\n"
                    "{0:>6} -+> src_loc ~> \"{10}#{8}:{9}\"\n"
                    "{0:>4}:[Err.Origin]-> }}\n"
                    "<-- [{1}]> }}"
                    , ""
                    , _errLevel
                    , pm_errCode_
                    , Pm_errWhat
                    , Pm_errWhereCaller.line ( )
                    , Pm_errWhereCaller.column ( )
                    , Pm_errWhereCaller.file_name ( )
                    , Pm_errWhereCaller.function_name ( )
                    , Pm_errWhereOrigin.line ( )
                    , Pm_errWhereOrigin.column ( )
                    , Pm_errWhereOrigin.file_name ( )
                    , Pm_errWhereOrigin.function_name ( )
                )
                
            }
        ;
        
        return { _errLog };
        
    }
    
    constexpr ErrorCode::operator int
        ( void /* v_ */ ) const noexcept
    {
        return static_cast <int> ( pm_errCode_ );
    }
    
    constexpr ErrorCode::operator long
        ( void /* v_ */ ) const noexcept
    {
        return pm_errCode_;
    }
    
    constexpr ErrorCode::operator bool
        ( void /* v_ */ ) const noexcept
    {
        return pm_errCode_;
    }
    
}
