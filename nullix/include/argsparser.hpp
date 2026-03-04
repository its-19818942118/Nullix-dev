/* #pragma once */

#ifndef ARGSPARSER_HPP_
    #define ARGSPARSER_HPP_
    
    #include<span>
    #include<cstddef>
    #include<functional>
    #include<string_view>
    #include<unordered_map>
    
    #include "globals.hpp"
    
    namespace [[
        /* nullAttr_ */
    ]] nullix
    {
        
        class [[
            /* nullAttr_ */
        ]] ArgsParser
            {
                
                public: struct Command;
                
                private: std::span<char* , std::dynamic_extent>
                    sp_PM_args
                ;
                // Internal state to track progress
                private: mutable std::size_t PMm_zu_cursor { +1ZU };
                
                /// Typedefs:
                private: using
                    _strV_t = std::string_view
                ;
                
                private: using
                    container_t_ = std::span<char* , std::dynamic_extent>
                ;
                
                private: using
                    iterator_t_ = container_t_::iterator
                ;
                
                private: using
                    l_itr_diff_t_ = std::iter_difference_t<long>
                ;
                
                private: using
                    k_Iterator_t_ = container_t_::const_iterator
                ;
                
                
                public: using Args_t_ = std::unordered_map<_strV_t , Command>;
                
                
                public: struct [[
                    /* nullAttr_ */
                ]] Command
                    {
                        using Action = std::function<void ( _strV_t )>;
                        
                        using Validation = std::function<bool ( _strV_t )>;
                        
                        using Description = std::string_view;
                        
                    
                        Action execute; // The "Worker"
                        Validation is_valid; // The "Gatekeeper"
                        Description description; // The "Helper"
                        
                    }
                ;
                
                public: auto size
                    ( void /* v_ */ ) const
                -> std::size_t;
                
                public: _strV_t operator [ ]
                    ( std::size_t /* zu_idx_ */ ) const
                ;
                
                public: auto at
                    ( std::size_t /* zu_idx_ */ ) const
                -> _strV_t;
                
                public: auto begin
                    ( void /* v_ */ ) const
                -> iterator_t_ const;
                
                public: auto end
                    ( void /* v_ */ ) const
                -> iterator_t_ const;
                
                public: auto dispatch
                    ( Args_t_ const& /* kr_map_HANDLERS_ */ ) const
                -> void;
                
                public: CLASS_CTOR
                    ArgsParser
                    (
                        std::size_t /* zu_argc_ */ ,
                        char** /* c_ptr_argv_ */
                    )
                ;
                
            }
        ;
        
    } /* namespace hyprscriptpp */
    
#endif /* ARGSPARSER_HPP_ */
