#pragma once

#include <array>
#include <cstdlib>
#include <filesystem>
#include <diagnostics.h>
#include <string>
#include <unistd.h>

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
        ( void )
    -> const std::fs::path
    {
        
        if
            ( const auto& home = std::getenv ( "HOME" ) )
        {
            return std::fs::path { home };
        }
        else if
            ( const auto& xdgHome = std::getenv ( "$XDG_HOME" ) )
        {
            return std::fs::path { xdgHome };
        }
        
        return { "/tmp/home/nullix_user" };
        
    }
    
    auto inline
        src
        ( void )
    -> const std::fs::path
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
        ( void )
    -> const std::fs::path
    {
        
        const auto
            path
            {
                homeDir ( ) /
                std::fs::path { ".cache/nullix" }
            }
        ;
        
        return { path };
        
    }
    
    auto inline
        thmbCache
        ( void )
    -> const std::fs::path
    {
        
        const auto path { cache ( ) / "thumbnails" };
        
        return { path };
        
    }
    
    auto inline
        blurCache
        ( void )
    -> const std::fs::path
    {
        
        const auto path { cache ( ) / "blur" };
        
        return { path };
        
    }
    
    auto inline
        clrsCache
        ( void )
    -> const std::fs::path
    {
        
        const auto path { cache ( ) / "clrs" };
        
        return { path };
        
    }
    
}


namespace
    nullix::utils
{
    
    auto
      inline
        getEnv
        ( std::string envVar_ )
    -> const std::string
    {
        
        if
            ( envVar_.starts_with('$') )
        {
            envVar_.erase ( 0 , 1 );
        }
        
        if
            ( const auto& _envVar = std::getenv ( envVar_.c_str ( ) ) )
        {
            return { _envVar };
        }
        
        return { };
        
    }
    
    auto
      inline
        sysProc
        ( const std::string& )
    {
        
        std::array<int, 2> _pipeFd { };
        
        if
            ( pipe ( _pipeFd.data ( ) ) == -1 )
        {
            
        }
        
    }
    
    auto
      inline
        mkdir
        ( const std::fs::path& path )
    -> const std::fs::path
    {
        
        if
            ( path.is_relative() )
        {
            
            const auto& relPath { std::fs::current_path ( ) / path };
            
            std::fs::create_directories ( relPath );
            return { relPath.lexically_normal ( ) };
        }
        
        else if
            ( path.is_absolute ( ) )
        {
            
            std::fs::create_directories ( path );
            
            return { path.lexically_normal ( ) };
            
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
            if (sv.empty()) return default_val;

            T val{};
            
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
            // The standard library requires start and end pointers.
            const char* first = sv.data();
            const char* last = sv.data() + sv.size();
            const auto [ptr, ec] = std::from_chars(first, last, val);
#pragma clang diagnostic pop

            if (ec == std::errc{}) {
                return val;
            }
            return default_val;
        }
    };
}

