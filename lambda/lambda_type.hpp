#include <functional>
#include <type_traits>

template<typename T>
struct lambda_traits : lambda_traits<decltype(&T::operator())> {};

template<typename ClassType, typename R, typename ...Args>
struct lambda_traits<R(ClassType::*)(Args...) const> {
    using result_type = R;
    using args_type = std::tuple<Args...>;
    template<size_t index>
    using arg_type_at = std::tuple_element_t<index, args_type>;
    static constexpr size_t arity = sizeof...(Args);
};

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
