//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_META_H_H
#define HASKISH_META_H_H

#include <iostream>

struct nil;
template <class, class> struct cons;
template <template <class, class> class, class, class> struct foldl;
template <class, class> struct add;
template <class> struct sum;

template <class T, T x>
struct Type {
    static constexpr T value = x;
};

template <int  x> struct Int  : Type<int,  x> {};
template <bool x> struct Bool : Type<bool, x> {};


template <template <class, class> class f, class x>
struct foldl<f, x, nil> {
    using result = x;
};

template <template <class, class> class f, class x, class y, class ys>
struct foldl<f, x, cons<y, ys>> {
    using result = typename foldl<f, typename f<x, y>::result, ys>::result;
};

template <class x, class y>
struct add {
    using result = Int<x::value + y::value>;
};

template <class xs>
struct sum {
    static constexpr auto result = foldl<add, Int<0>, xs>::result::value;
};

void test_sum() {
    using a = Int<3>;
    using b = Int<5>;
    using c = Int<2>;

    using list = cons<a, cons<b, cons<c, nil>>>;

    std::cout << sum<list>::result << std::endl;
}

#endif //HASKISH_META_H_H
