#pragma once

#include <array>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <diagnostics.h>

namespace
    std::fs
{
    using namespace ::std::filesystem;
} // this isn't recommended but i always do this. to make it
// std::fs instead of std::filesystem

namespace
    nullix::dirs
{
    
    auto inline
        homeDir
        ( void /* v_ */ )
    -> std::fs::path const
    {
        
        if
            ( auto const& home = std::getenv ( "HOME" ) )
        {
            return std::fs::path { home };
        }
        else if
            ( auto const& xdgHome = std::getenv ( "$XDG_HOME" ) )
        {
            return std::fs::path { xdgHome };
        }
        
        return { "/tmp/home/nullix_user" };
        
    }
    
    auto inline src
        ( void /* v_ */ )
    -> std::fs::path const
    {
        
        return
            {
                std::fs::read_symlink
                ( "/proc/self/exe" ).parent_path ( ).lexically_normal ( )
            }
        ;
        
    }
    
    auto inline
        cache
        ( void /* v_ */ )
    -> std::fs::path const
    {
        
        auto const _k_path { homeDir ( ) / std::fs::path { ".cache/nullix" } };
        
        return { _k_path };
        
    }
    
    auto inline
        thmbCache
        ( void /* v_ */ )
    -> std::fs::path const
    {
        
        auto const _k_path { cache ( ) / "thumbnails" };
        
        return { _k_path };
        
    }
    
    auto inline
        blurCache
        ( void /* v_ */ )
    -> std::fs::path const
    {
        
        auto const _k_path { cache ( ) / "blur" };
        
        return { _k_path };
        
    }
    
    auto inline
        clrsCache
        ( void /* v_ */ )
    -> std::fs::path const
    {
        
        auto const _k_path { cache ( ) / "clrs" };
        
        return { _k_path };
        
    }
    
}

namespace
    nullix::utils
{
    
    auto inline getEnv
        ( std::string envVar_ )
    -> std::string const
    {
        
        if
            ( envVar_.starts_with('$') )
        {
            envVar_.erase ( 0 , 1 );
        }
        
        if
            ( auto const& _envVar = std::getenv ( envVar_.c_str ( ) ) )
        {
            return { _envVar };
        }
        
        return { };
        
    }
    
    auto inline sysProc
        ( std::string const& )
    {
        
        std::array<int, 2> _pipeFd { };
        
        if
            ( pipe ( _pipeFd.data ( ) ) == -1 )
        {
            
        }
        
    }
    
    auto inline mkdir
        ( std::fs::path const& kr_fsp_path_ )
    -> std::fs::path const
    {
        
        if
            ( kr_fsp_path_.is_relative ( ) )
        {
            
            auto const& kr_relPath { std::fs::current_path ( ) / kr_fsp_path_ };
            
            std::fs::create_directories ( kr_relPath );
            return { kr_relPath.lexically_normal ( ) };
        }
        
        else if
            ( kr_fsp_path_.is_absolute ( ) )
        {
            
            std::fs::create_directories ( kr_fsp_path_ );
            
            return { kr_fsp_path_.lexically_normal ( ) };
            
        }
        
        return { };
        
    }
    
}

namespace nullix {

    struct ValueParser {
        /**
         * Converts string_view to a numeric type safely.
         * Locally suppresses unsafe buffer warnings for the pointer arithmetic
         * required by std::from_chars.
         */
        template <typename T>
        static auto to_num(std::string_view sv, T default_val = { }) -> T {
            if (sv.empty ( )) return default_val;

            T val{};
            
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
            // The standard library requires start and end pointers.
            char const* first = sv.data ( );
            char const* last = sv.data ( ) + sv.size ( );
            auto const [ptr, ec] = std::from_chars(first, last, val);
#pragma clang diagnostic pop

            if (ec == std::errc{}) {
                return val;
            }
            return default_val;
        }
    };
}

