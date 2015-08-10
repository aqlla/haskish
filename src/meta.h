//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_META_H_H
#define HASKISH_META_H_H

#include "data_types.h"

namespace itp {
    template<template<class, class> class, class, class>
    struct fold;

    template<class, class>
    struct add;

    template<class>
    struct sum;

    template<template<class, class> class F, class x>
    struct fold<F, x, nil> {
        using result = x;
    };

    template<template<class, class> class F, class x, class y, class ys>
    struct fold<F, x, cons<y, ys>> {
        using result = typename fold<F, typename F<x, y>::result, ys>::result;
    };

    template<class x, class y>
    struct add {
        using result = Int<x::value + y::value>;
    };

    template<class xs>
    struct sum {
        static constexpr auto result = fold <add, Int<0>, xs>::result::value;
    };
};

#endif //HASKISH_META_H_H
