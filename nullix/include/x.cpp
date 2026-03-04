
#include "argsparser.hpp"
#include "globals.hpp"
#include <cstddef>
#include <print>

auto inline
    adjustVolume
    ( const int stepDelta_ )
{
    using namespace nullix;
    
    constexpr int _maxVolume { 150 } , _minVolume { };
    
    const int
        _curVolume
        { std::stoi ( utils::system ( "pamixer --get-volume" ) ) }
    ;
    
    // 3. Apply the math (+10 or -10)
    int _newVolume { _curVolume + stepDelta_ };
    
    // 4. Constrain (keep it between 0 and 150)
    if ( _newVolume > _maxVolume ) _newVolume = _maxVolume;
    if ( _newVolume < _minVolume ) _newVolume = _minVolume;
    
    const
      std::string
        _command
        {
            std::format
            (
                "pamixer --allow-boost --set-limit {} --set-volume {}" ,
                _maxVolume , _newVolume
            )
        }
    ;
    
    system ( _command.c_str ( ) );
    
    return
        (
            std::format
            (
                "Volume adjusted: {} -> {}" ,
                _curVolume , _newVolume
            )
        )
    ;
}

// auto
//     main
//     ( int argc = { } , char** argv = { } )
// -> int
// {
    
//     // using namespace std::literals;
    
//     // std::print ( "{}"sv , nullix::utils::system ( R"sh(echo $USER)sh" ) );
    
//     // int value { 10 };
//     // std::println (stderr, "{}", ::adjustVolume ( -value ) );
//     // std::println (stderr, "{}", ::adjustVolume ( +value ) );
    
    
//     nullix::ArgsParser argsparser { static_cast<size_t>(argc), argv };
    
//     nullix::ArgsParser::Args_t_ volumeCtl;

//     // Helper to convert string_view to int safely
//     auto to_int = [](std::string_view sv) -> int {
//         int val = 0;
//         if (sv.empty()) return 0;
//         std::from_chars(sv.data(), sv.data() + sv.size(), val);
//         return val;
//     };

//     volumeCtl["--increase"] = {
//         .execute = [to_int](auto value) {
//             // If user did '--increase' without value, default to 1
//             int delta = value.empty() ? 1 : to_int(value);
//             adjustVolume(delta);
//         },
//         .is_valid = [](auto value) {
//             // Optional: check if digits or empty (for default)
//             return value.empty() || value.find_first_not_of("0123456789") == std::string_view::npos;
//         },
//         .description = "Increase volume by X (default 1)"
//     };

//     volumeCtl["--decrease"] = {
//         .execute = [to_int](auto value) {
//             int delta = value.empty() ? 1 : to_int(value);
//             adjustVolume(-delta); // Pass negative for decrease
//         },
//         .is_valid = nullptr, // Skipping validation for brevity
//         .description = "Decrease volume by X"
//     };

//     argsparser.dispatch(volumeCtl);
// }

#include <print>
#include <string_view>

int main(int argc, char** argv) {
    // 1. Initialize Parser
    nullix::ArgsParser argsparser { static_cast<size_t>(argc), argv };
    
    // 2. Define Command Map
    nullix::ArgsParser::Args_t_ volumeCtl;

    // --increase implementation
    volumeCtl["--increase"] = {
        .execute = [](std::string_view value) {
            // Default to 1 if no value provided, otherwise parse
            int delta = value.empty() ? 1 : nullix::ValueParser::to_num<int>(value);
            ::adjustVolume(+delta);
            std::println("Action: Increasing by {}", +delta);
        },
        .is_valid = [](std::string_view value) {
            // Valid if empty (flag only) or contains only digits
            return value.empty() || value.find_first_not_of("0123456789") == std::string_view::npos;
        },
        .description = "Increase volume. Usage: --increase=5 or --increase 5"
    };

    // --decrease implementation
    volumeCtl["--decrease"] = {
        .execute = [](std::string_view value) {
            int delta = value.empty() ? 1 : nullix::ValueParser::to_num<int>(value);
            ::adjustVolume(-delta);
            std::println("Action: Decreasing by {}", -delta);
        },
        .is_valid = [](std::string_view value) {
            return value.empty() || value.find_first_not_of("0123456789") == std::string_view::npos;
        },
        .description = "Decrease volume. Usage: --decrease=5 or --decrease 5"
    };

    // 3. Dispatch
    argsparser.dispatch(volumeCtl);

    return 0;
}
