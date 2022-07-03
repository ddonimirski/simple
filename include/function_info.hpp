//
//  Simple header only template function provides 
//  information about funcions
//

#ifndef SIMPLE_FUNCTION_INFO_HPP
#define SIMPLE_FUNCTION_INFO_HPP

#include <tuple>

namespace simple::type_traits
{
    template<class F>
    struct function_info: function_info<decltype(&F::operator())>
    {
    };

    // parital specialization for function type
    template<class R, class ...Args>
    struct function_info<R(Args...)>
    {
        enum { number_of_arguments = sizeof...(Args) };
        using return_type = R;
        using arguments_tuple_type = std::tuple<Args...>;
    };

    // parital specialization for function pointer
    template<class R, class ...Args>
    struct function_info<R(*)(Args...)>
    {
        enum { number_of_arguments = sizeof...(Args) };
        using return_type = R;
        using arguments_tuple_type = std::tuple<Args...>;
    };

    template<class R, class F, class ...Args>
    struct function_info<R(F::*)(Args...)>
    {
        enum { number_of_arguments = sizeof...(Args) };
        using return_type = R;
        using arguments_tuple_type = std::tuple<Args...>;
    };

    template<class R, class F, class ...Args>
    struct function_info<R(F::*)(Args...) const>
    {
        enum { number_of_arguments = sizeof...(Args) };
        using return_type = R;
        using arguments_tuple_type = std::tuple<Args...>;
    };

    template<class F>
    static inline constexpr size_t number_of_arguments = function_info<F>::number_of_arguments;

    template<class F>
    using return_function_type = typename function_info<F>::return_type;

    template<size_t I, class F>
    using argument_function_type = typename std::tuple_element<I, typename function_info<F>::arguments_tuple_type>::type;

    template<class F>
    using first_argument_type = argument_function_type<0, F>;

    template<class F>
    using second_argument_type = argument_function_type<1, F>;

} // namespace simple::type_traits
#endif // SIMPLE_FUNCTION_INFO_HPP

