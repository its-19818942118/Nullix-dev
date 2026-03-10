/* #pragma once */

#ifndef TRACER_CPP_UNIQX
    #define TRACER_CPP_UNIQX
    
    #include <cstdlib>
#include <print>
#include <stacktrace>
#include <atomic>

// Standard types are safer here than rvalue refs
static bool g_track_active = true; 
static bool g_stack_trace_enabled = false;
static std::atomic<size_t> g_alloc_count{0};
static std::atomic<size_t> g_alloc_size{0};

// The "Secret Sauce": only log if this is true
static thread_local bool g_scoped_watch = false;

void* operator new(size_t size) {
    void* p = std::malloc(size);
    
    // We only print if tracking is active AND we are in a "watched" scope
    if (g_track_active && g_scoped_watch) {
        g_track_active = false; // Prevent recursion from std::println
        
        size_t current_count = ++g_alloc_count;
        g_alloc_size += size;

        std::println("\n[ SCOPED ALLOC #{:03} ] +{} bytes", current_count, size);
        
        if (g_stack_trace_enabled) {
            std::println("[ Origin ]\n{}", std::stacktrace::current());
        }
        
        g_track_active = true;
    }
    return p;
}

void operator delete(void* p) noexcept { std::free(p); }

// --- The RAII Guard ---
struct MemWatch {
    MemWatch(bool enable_stack = false) {
        g_stack_trace_enabled = enable_stack;
        g_scoped_watch = true; 
    }
    ~MemWatch() {
        g_scoped_watch = false;
        g_stack_trace_enabled = false;
    }
};
    
#endif /* TRACER_CPP_UNIQX */
