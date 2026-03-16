cmake_minimum_required ( VERSION 4.2.3 )

set ( CMAKE_COLOR_DIAGNOSTICS ON )

project( nullib
    VERSION 0.1.5
    LANGUAGES CXX
    DESCRIPTION "A set of internal and external libraries for Nullix"
)

add_library ( nullib STATIC )

find_package ( tomlplusplus REQUIRED )

add_library ( nullix::nullib ALIAS nullib )


target_compile_features ( nullib PUBLIC
    cxx_std_26
)

target_compile_options( nullib PRIVATE
    $<$<CXX_COMPILER_ID:GCC,GNU,Clang>:
        -ggdb
        
        -Wall
        -Werror
        -Wextra
        -Wpedantic
        
        -fno-exceptions
        -fdiagnostics-color
        -ferror-limit=99999999
    >
    
    $<$<CXX_COMPILER_ID:Clang>:
        -ggdb
        
        -Werror
        -Weverything
        
        -Wno-c++98-compat
        -Wno-switch-default
        -Wno-c++98-compat-pedantic
        -Wno-covered-switch-default
        
        -fdiagnostics-color
        -ferror-limit=99999999
    >
    --verbose
)

target_compile_options ( nullib PUBLIC
    $<$<CXX_COMPILER_ID:GCC,GNU,Clang>:
        -fno-exceptions
    >
)

target_sources( nullib PRIVATE
    nullix/source/host/host.cpp
    errika/source/error/error.cpp
    uniqx/source/filesystem/Path.cpp
    uniqx/source/filesystem/FileTree.cpp
    uniqx/source/system/process/ProcPd.cpp
    uniqx/source/system/process/ProcPipe.cpp
    uniqx/source/system/process/SubProcess.cpp
    uniqx/source/system/process/ProcStream.cpp
    uniqx/source/system/process/ProcResult.cpp
)

target_include_directories ( nullib PUBLIC
    uniqx/include/system/process
    uniqx/include/filesystem
    uniqx/include/system
    errika/include/error
    nullix/include
    errika/include
    uniqx/include
    nullix
    errika
    uniqx
    .
    uniqx
    errika
    nullix
    uniqx/include
    errika/include
    nullix/include
    errika/include/error
    uniqx/include/system
    uniqx/include/filesystem
    uniqx/include/system/process
)

target_link_options ( nullib PRIVATE stdc++exp )

target_link_libraries ( nullib PRIVATE stdc++exp )

target_link_libraries ( nullib PRIVATE tomlplusplus::tomlplusplus )

target_compile_definitions ( nullib PRIVATE TOML_EXCEPTIONS=0 )
