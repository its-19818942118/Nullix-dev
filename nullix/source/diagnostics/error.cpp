#include <print>
#include <format>
#include <string>
#include <cstddef>
#include <source_location>

#include "nullix.hpp"
#include "diagnostics/error.hpp"

namespace
    nullix::err::v1
{
    
    [[
        deprecated
        ( "Please use `nullix::err::v2::Error` instead" )
    ]] Error::Error
            (
                const std::string& errWhat_ ,
                const std::source_location& errWhere_ =
                { std::source_location::current ( ) }
            )
        : _errWhat ( errWhat_ )
        , _errWhere ( errWhere_ )
    { }
    
    constexpr auto
        Error::mt_str_value_or_log
        ( const std::string& fallback_ ) const
    -> std::string
    {
        
        std::println
            (
                stderr ,
                "<[ERR.FATAL]: "
                "On function \x1b[1;4m`{4}`\x1b[0m ,\n"
                "  <<[source.location.file]: "
                "\x1b[3;4m'{3}'\x1b[0m\n"
                "  <<[source.location.line]: '{1}:{2}'\n"
                "  <<<[Reason]: {0}"
                , _errWhat
                , _errWhere.line ( )
                , _errWhere.column ( )
                , _errWhere.file_name ( )
                , _errWhere.function_name ( )
            )
        ;
        
        return { fallback_ };
        
    }
    
    [[
        deprecated
        ( "Please use `nullix::err::v2::ErrorCode` instead" )
    ]] ErrorInt::
        ErrorInt
            (
                const long& errCode_ ,
                const std::string& errWhat_ ,
                const std::source_location& errWhere_ =
                { std::source_location::current ( ) }
            )
        : Error ( errWhat_ , errWhere_ )
        , _errCode ( errCode_ )
    { }
    
    constexpr auto
        ErrorInt::mt_str_value_or_log
        ( const std::string& fallback_ ) const
    -> std::string
    {
        
        std::println
            (
                stderr ,
                "<[ERR]: on ({4} {2}:{3})\n"
                "  <<[Reason]: {1}\n"
                "   <<[Hint.err.code]: {0}"
                , _errCode , _errWhat
                , _errWhere.line ( )
                , _errWhere.column ( )
                , _errWhere.file_name ( )
            )
        ;
        
        return { fallback_ };
        
    }
    
} /* nullix::err::v1 */

namespace
    nullix::err::v2::detail
{
    
    [[
        nodiscard
        ( "Important: returns sanitized errWhat_" )
    ]] auto
        sanErr
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
        _result.reserve ( errWhatRaw_.size ( ) + 64 );
        
        for
            (
                std::size_t _col { 0xF };
                const auto& c : errWhatRaw_
            ) [[likely]]
        {
            if
                (
                    ( c == '\n' ) ||
                    ( _col >= 70 && c == ' ' ) ||
                    ( _col >= 80 )
                ) [[likely]]
            {
                _col = 0xF;
                _result.append ( "\n        #>> " );
                if ( c == '\n' ) continue;
            }
            _result.push_back ( c );
            ++_col;
        }
        
        return { _result };
        
    }
    
    
} /* namespace nullix::err::v2::detail */

namespace
    nullix::err::v2
{
    
    CLASS_CTOR
        Error:: Error
            (
                const e_ErrType errType_ ,
                const std::string errWhat_ ,
                const std::source_location errWhereCaller_ ,
                const std::source_location errWhereOrigin_
            )
        : kPm_errType ( std::move ( errType_ ) )
        , kPm_errWhat ( std::move ( errWhat_ ) )
        , kPm_errWhat_ ( detail::sanErr ( kPm_errWhat ) )
        , kPm_errWhereCaller ( std::move ( errWhereCaller_ ) )
        , kPm_errWhereOrigin ( std::move ( errWhereOrigin_ ) )
    { }
    
    [[
        nodiscard
        ( "Important: returns a formatted log string" )
    ]] auto
        Error::mt_str_errLog
        ( void /* v_ */ ) const
    -> std::string
    {
        
        using namespace std::literals;
        std::string_view _errLevel { };
        
        switch
            ( kPm_errType )
        {
            case
                e_ErrType::Fatal:
            {
                _errLevel = "Err.Fatal"sv;
                break;
            }
            case
                e_ErrType::Warning:
            {
                _errLevel = "Err.Warnng"sv;
                break;
            }
            case
                e_ErrType::Unknown:
            {
                _errLevel = "Err.unknown"sv;
                break;
            }
        }
        
        const auto&
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
                    , kPm_errWhat_
                    , _errLevel
                    , kPm_errWhereOrigin.line ( )
                    , kPm_errWhereOrigin.column ( )
                    , kPm_errWhereOrigin.file_name ( )
                    , kPm_errWhereOrigin.function_name ( )
                )
                
            }
        ;
        
        return { _errLog };
        
    }
    
    [[
        nodiscard
        ( "Important: returns a fallback_ value" ) ,
    ]] auto
        Error::mt_str_fallback
        ( const std::string& fallback_ ) const
    -> std::string
    {
        
        return { fallback_ };
        
    }
    
    [[
        nodiscard
        ( "Important: prints error & returns a fallback_ value" ) ,
    ]] auto
        Error::mt_str_errLog_or
        ( const std::string& fallback_ ) const
    -> std::string
    {
        
        std::print ( stderr , "{}" , mt_str_errLog ( ) );
        
        return { fallback_ };
        
    }
    
    Error::~Error ( ) = default;
    
    CLASS_CTOR ErrorCode::
        ErrorCode
            (
                const long errCode_ ,
                const e_ErrType errType_ ,
                const std::string errWhat_ ,
                const std::source_location errWhereCaller_ ,
                const std::source_location errWhereOrigin_
            )
        : Error ( errType_ , errWhat_ , errWhereCaller_ , errWhereOrigin_ )
        , kpm_errCode_ ( std::move ( errCode_ ) )
    { }
    
    [[
        nodiscard
        ( "Important: returns a formatted log string" )
    ]] auto
        ErrorCode::mt_str_errLog
        ( void /* v_ */ ) const
    -> std::string
    {
        
        using namespace std::literals;
        std::string_view _errLevel { };
        
        switch
            ( kPm_errType )
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
        
        const auto&
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
                    , kpm_errCode_
                    , kPm_errWhat_
                    , kPm_errWhereCaller.line ( )
                    , kPm_errWhereCaller.column ( )
                    , kPm_errWhereCaller.file_name ( )
                    , kPm_errWhereCaller.function_name ( )
                    , kPm_errWhereOrigin.line ( )
                    , kPm_errWhereOrigin.column ( )
                    , kPm_errWhereOrigin.file_name ( )
                    , kPm_errWhereOrigin.function_name ( )
                )
                
            }
        ;
        
        return { _errLog };
        
    }
    
    constexpr ErrorCode::operator int
        ( void /* v_ */ ) const noexcept
    {
        return static_cast <int> ( kpm_errCode_ );
    }
    
    constexpr ErrorCode::operator long
        ( void /* v_ */ ) const noexcept
    {
        return kpm_errCode_;
    }
    
    constexpr ErrorCode::operator bool
        ( void /* v_ */ ) const noexcept
    {
        return kpm_errCode_;
    }
    
}
