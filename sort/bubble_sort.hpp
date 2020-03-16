#pragma once

#include <iostream>
#include <iterator>
#include <type_traits>

using namespace std;

template<int ...vals> struct Vals {};

template<int i, typename T> struct ValueAt;
template<int i, int val0, int ...vals> struct ValueAt<i, Vals<val0, vals...>>
{
    constexpr static int val = ValueAt<i - 1, Vals<vals...>>::val;
};
template<int val0, int ...vals> struct ValueAt<0, Vals<val0, vals...>>
{
    constexpr static int val = val0;
};

template<typename T1, typename T2> struct Combine;
template<int ...vals1, int ...vals2> struct Combine<Vals<vals1...>, Vals<vals2...>>
{
    using type = Vals<vals1..., vals2...>;
};

template<int i, typename T> struct SwapValue;
template<int i, int val, int ...vals> struct SwapValue<i, Vals<val, vals...>>
{
    using type = typename Combine<Vals<val>, typename SwapValue<i - 1, Vals<vals...>>::type>::type;
};
template<int val1, int val2, int ...vals> struct SwapValue<0, Vals<val1, val2, vals...>>
{
    using type = typename Combine<Vals<val2>, typename Combine<Vals<val1>, Vals<vals...>>::type>::type;
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
template<int val> struct BubbleSort<1, 1, Vals<val>>
{
    using type = Vals<val>;
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

/* Test:
print(Sort<Vals<1>>::type());
print(Sort<Vals<2,1>>::type());
print(Sort<Vals<3,5,2,8,7,1,6,9,4>>::type());
*/
