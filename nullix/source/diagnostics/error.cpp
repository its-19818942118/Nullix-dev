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
    
    CLASS_CTOR [[
        deprecated
        ( "Please use `nullix::err::v2::Error` instead" )
    ]] Error::
            Error
            (
                std::string const& kr_s_errWhat_ ,
                std::source_location const& kr_sl_errWhere_ =
                { std::source_location::current ( ) }
            )
        : _errWhat ( kr_s_errWhat_ )
        , _errWhere ( kr_sl_errWhere_ )
    { }
    
    constexpr auto
        Error::mt_str_value_or_log
        ( std::string const& fallback_ ) const
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
    
    CLASS_CTOR [[
        deprecated
        ( "Please use `nullix::err::v2::ErrorCode` instead" )
    ]] ErrorInt::
            ErrorInt
            (
                long const k_l_errCode_ ,
                std::string const& kr_s_errWhat_ ,
                std::source_location const& kr_sl_errWhere_ =
                { std::source_location::current ( ) }
            )
        : Error ( kr_s_errWhat_ , kr_sl_errWhere_ )
        , _errCode ( k_l_errCode_ )
    { }
    
    constexpr auto
        ErrorInt::mt_str_value_or_log
        ( std::string const& fallback_ ) const
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
        _result.reserve ( errWhatRaw_.size ( ) + +64 );
        
        for
            (
                std::size_t _col { 0xF };
                auto const& c : errWhatRaw_
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
                e_ErrType const errType_ ,
                std::string const k_s_errWhat_ ,
                std::source_location const k_sl_errWhereCaller_ ,
                std::source_location const k_sl_errWhereOrigin_
            )
        : Pmk_errType ( std::move ( errType_ ) )
        , Pmk_errWhat ( std::move ( k_s_errWhat_ ) )
        , Pmkr_str_errWhat_ ( detail::sanErr ( Pmk_errWhat ) )
        , Pmk_errWhereCaller ( std::move ( k_sl_errWhereCaller_ ) )
        , Pmk_errWhereOrigin ( std::move ( k_sl_errWhereOrigin_ ) )
    { }
    
    [[
        nodiscard
        ( "Important: returns a formatted log string" )
    ]] auto
        Error::mt_str_errLog
        ( void /* v_ */ ) const
    -> std::string
    {
        
        std::string_view _errLevel { };
        
        switch
            ( Pmk_errType )
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
                    , Pmk_errWhat
                    , _errLevel
                    , Pmk_errWhereOrigin.line ( )
                    , Pmk_errWhereOrigin.column ( )
                    , Pmk_errWhereOrigin.file_name ( )
                    , Pmk_errWhereOrigin.function_name ( )
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
        ( std::string const& fallback_ ) const
    -> std::string
    {
        
        return { fallback_ };
        
    }
    
    [[
        nodiscard
        ( "Important: prints error & returns a fallback_ value" ) ,
    ]] auto
        Error::mt_str_errLog_or
        ( std::string const& fallback_ ) const
    -> std::string
    {
        
        std::print ( stderr , "{}" , mt_str_errLog ( ) );
        
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
        , pmk_errCode_ ( std::move ( errCode_ ) )
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
            ( Pmk_errType )
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
                    , pmk_errCode_
                    , Pmk_errWhat
                    , Pmk_errWhereCaller.line ( )
                    , Pmk_errWhereCaller.column ( )
                    , Pmk_errWhereCaller.file_name ( )
                    , Pmk_errWhereCaller.function_name ( )
                    , Pmk_errWhereOrigin.line ( )
                    , Pmk_errWhereOrigin.column ( )
                    , Pmk_errWhereOrigin.file_name ( )
                    , Pmk_errWhereOrigin.function_name ( )
                )
                
            }
        ;
        
        return { _errLog };
        
    }
    
    constexpr ErrorCode::operator int
        ( void /* v_ */ ) const noexcept
    {
        return static_cast <int> ( pmk_errCode_ );
    }
    
    constexpr ErrorCode::operator long
        ( void /* v_ */ ) const noexcept
    {
        return pmk_errCode_;
    }
    
    constexpr ErrorCode::operator bool
        ( void /* v_ */ ) const noexcept
    {
        return pmk_errCode_;
    }
    
}
