#include <functional>
#include <type_traits>

template<typename T> struct lambda_traits_base;
template<typename ClassType, typename R, typename ...Args>
struct lambda_traits_base<R(ClassType::*)(Args...) const> {
    using result_type = R;
    using args_type = std::tuple<Args...>;
    template<size_t index>
    using arg_type_at = std::tuple_element_t<index, args_type>;
    static constexpr size_t arity = sizeof...(Args);
};

template<typename T, typename = std::void_t<>>
struct call_which {
    using type = decltype(&T::operator());
};
template<typename T>
struct call_which<T, std::void_t<decltype(&T::template operator()<>)>> {
    using type = decltype(&T::template operator()<>);
};
template<typename T>
using call_which_t = typename call_which<T>::type;

template<typename T, typename ...Args>
struct lambda_traits : lambda_traits_base<decltype(&T::template operator()<Args...>)> {};
template<typename T>
struct lambda_traits<T> : lambda_traits_base<call_which_t<T>> {};

/* Test:
template<typename Func>
void test(Func &&f) {
    std::cout << typeid(typename lambda_traits<Func>::result_type).name() << std::endl;
}

int main()
{
    test([]() -> double { return 1.0; });
}
*/
