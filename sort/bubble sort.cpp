#include <iostream>
#include <iterator>
#include <type_traits>

using namespace std;

template<int val, int ...vals> struct Vals
{
    constexpr static int fval = val;
    constexpr static size_t size = sizeof...(vals) + 1;
    using TailsType = Vals<vals...>;
};

template<int val> struct Vals <val>
{
    constexpr static int fval = val;
    constexpr static size_t size = 1;
    using TailsType = void;
};

template<int i, typename T> struct ValueAt
{
    using type = typename ValueAt<i - 1, typename T::TailsType>::type;
    constexpr static int val = type::fval;
};

template<int  ...vals> struct ValueAt<0, Vals<vals...>>
{
    using type = Vals<vals...>;
    constexpr static int val = type::fval;
};

template<typename T1, typename T2> struct Combine;
template<int ...vals1, int ...vals2> struct Combine<Vals<vals1...>, Vals<vals2...>>
{
    using type = Vals<vals1..., vals2...>;
};

template<int i, typename T> struct SwapValue
{
    using type = typename Combine<
        Vals<T::fval>,
        typename SwapValue<i - 1, typename T::TailsType>::type>::type;
};

template<int ...vals> struct SwapValue<0, Vals<vals...>>
{
    using V = Vals<vals...>;
    using type = typename Combine<
        Vals<ValueAt<1, V>::val>,
        typename Combine<Vals<
            ValueAt<0, V>::val>,
            typename Vals<vals...>::TailsType::TailsType>::type>::type;
};

template<int val1, int val2> struct SwapValue<0, Vals<val1, val2>>
{
    using type = Vals<val2, val1>;
};

template<int i, int j, typename T> struct BubbleSort
{
    using type = typename conditional_t<(ValueAt<j - 1, T>::val > ValueAt<j, T>::val),
        BubbleSort<i, j + 1, typename SwapValue<j - 1, T>::type>,
        BubbleSort<i, j + 1, T>>::type;
};

template<int j, int ...vals> struct BubbleSort<sizeof...(vals), j, Vals<vals...>>
{
    using type = Vals<vals...>;
};

template<int i, int ...vals> struct BubbleSort<i, sizeof...(vals), Vals<vals...>>
{
    using type = typename BubbleSort<i + 1, 1, Vals<vals...>>::type;
};

template<typename T> struct Sort
{
    using type = typename BubbleSort<0, 1, T>::type;
};

template<int ...vals> void print(Vals<vals...>)
{
    static constexpr auto vs = { vals... };
    copy(begin(vs), end(vs), ostream_iterator<int>(cout, " "));
    endl(cout);
}

int main()
{
    print(Sort<Vals<3,5,2,8,7,1,6,9,4>>::type());
    return 0;
}
