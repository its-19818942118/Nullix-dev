/* #pragma once */

#ifndef ARGSPARSER_HPP_HYPRSCRIPTSPP
    #define ARGSPARSER_HPP_HYPRSCRIPTSPP
    
    #include <span>
    #include <cstddef>
    #include <functional>
    #include <string_view>
    #include <unordered_map>
    
    namespace nullix
    {
        
        class ArgsParser
            {
                
                public: struct
                    Command
                ;
                
                private: std::span <char* , std::dynamic_extent>
                    sp_PM_args
                ;
                // Internal state to track progress
                private: mutable
                    std::size_t mut_zu_PM_cursor { 1ZU }
                ;
                
                /// Typedefs:
                private: using
                    _strV_t = std::string_view
                ;
                
                private: using
                    _container_t = std::span <char* , std::dynamic_extent >
                ;
                
                private: using
                    _iterator_t = _container_t::iterator
                ;
                
                private: using
                    _l_itr_diff_t = std::iter_difference_t <long>
                ;
                
                private: using
                    _const_iterator_t = _container_t::const_iterator
                ;
                
                
                public: using
                    Args_t_ = std::unordered_map <_strV_t , Command>
                ;
                
                
                public: struct
                    Command
                    {
                        using
                            Action = std::function < void ( _strV_t ) >
                        ;
                        
                        using
                            Validation = std::function < bool ( _strV_t ) >
                        ;
                        
                        using
                            Description = std::string_view
                        ;
                        
                    
                        Action execute;  // The "Worker"
                        Validation is_valid; // The "Gatekeeper"
                        Description description; // The "Helper"
                        
                    }
                ;
                
                public: std::size_t
                    size
                    ( void /* v_ */ ) const
                ;
                
                public: _strV_t
                    operator []
                    ( std::size_t /* zu_idx_ */ ) const
                ;
                
                public: _strV_t
                    at
                    ( std::size_t /*  zu_idx_ */ ) const
                ;
                
                public: const
                  _iterator_t
                    begin
                    ( void /* v_ */ ) const
                ;
                
                public: const
                  _iterator_t
                    end
                    ( void /* v_ */ ) const
                ;
                
                public: void
                    dispatch
                    (
                        const
                          Args_t_&
                        /* cst_map_ref_HANDLERS_ */
                    ) const
                ;
                
                public:
                    ArgsParser
                    (
                        std::size_t /* zu_argc_ */ ,
                        char** /* c_ptr_argv_ */
                    )
                ;
                
            }
        ;
        
    } /* namespace hyprscriptpp */
    
#endif /* ARGSPARSER_HPP_HYPRSCRIPTSPP */
