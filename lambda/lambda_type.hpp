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

template<typename T>
struct allshit {
    template<typename U>
    static auto fuck(int) -> decltype(&U::template operator()<>);
    template<typename U>
    static auto fuck(...) -> decltype(&U::operator());

    using type = decltype(fuck<T>(0));
};

template<typename T, typename ...Args>
struct lambda_traits : lambda_traits_base<decltype(&T::template operator()<Args...>)> {};

template<typename T>
struct lambda_traits<T> : lambda_traits_base<typename allshit<T>::type> {};

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
