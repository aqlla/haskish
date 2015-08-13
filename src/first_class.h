//
// Created by Aquilla Sherrock on 8/11/15.
//

#ifndef HASKISH_TFN_H
#define HASKISH_TFN_H

#include <type_traits>
#include <iostream>
#include <vector>

#include "functions.h"
#include "tuple_apply.h"


/* Create a generic functor to act as a first-class function. */
#define make_first_class(NAME, X)                        \
struct NAME {                                            \
    template <class... Args>                             \
    auto operator()(Args&&... ts) const                  \
            -> decltype(X(std::forward<Args>(ts)...)) {  \
        return X(std::forward<Args>(ts)...);             \
    }                                                    \
};


// override | operator for functional piping.
template <class T, class Fn>
auto operator |(T&& args, const Fn& f) -> decltype(f(args)) {
    return f(std::forward<T>(args));
};

/** function wrapper.
 *
 * Wrapper type enabling functions and lambdas to make use of currying and for
 * tuples to be applied as parameter lists.
 */
template <class Fn, class Before = std::tuple<>, class After = std::tuple<>>
struct function
{
private:
    Fn f;           // function
    Before before;  // curried args before
    After after;    // curried args after

public:
    function(Fn&& f)
            : f(std::forward<Fn>(f)),
              before{std::tuple<>()},
              after{std::tuple<>()} {}

    function(const Fn& f, const Before& before, const After& after)
            : f(f),
              before{before},
              after{after} {}

    template <class... Args>
    auto operator ()(Args... args) const
            -> decltype(tuple_apply(f, std::tuple_cat(before, std::make_tuple(args...), after))) {
        return tuple_apply(f, std::tuple_cat(before, std::make_tuple(std::forward<Args>(args)...), after));
    }

    template <class T>
    auto curry(T&& arg) const
            -> decltype(function<Fn, decltype(std::tuple_cat(before, std::make_tuple(arg))), After>(f, std::tuple_cat(before, std::make_tuple(arg)), after)) {
        return function<Fn, decltype(std::tuple_cat(before, std::make_tuple(arg))), After>(f, std::tuple_cat(before, std::make_tuple(std::forward<T>(arg))), after);
    }

    template <typename T>
    auto curry_right(T&& arg) const
            -> decltype(function<Fn, Before, decltype(std::tuple_cat(after, std::make_tuple(arg)))>(f, before, std::tuple_cat(after, std::make_tuple(arg)))) {
        return function<Fn, Before, decltype(std::tuple_cat(after, std::make_tuple(arg)))>(f, before, std::tuple_cat(after, std::make_tuple(std::forward<T>(arg))));
    }
};

// override << operator or left curry
template <class UF, class Arg>
auto operator <<(const UF& f, Arg&& arg)
    -> decltype(f.template curry<Arg>(std::forward<Arg>(arg))) {
    return f.template curry<Arg>(std::forward<Arg>(arg));
}

// override >> operator or right curry
template <class UF, class Arg>
auto operator >>(const UF& f, Arg&& arg)
    -> decltype(f.template curry_right<Arg>(std::forward<Arg>(arg))) {
    return f.template curry_right<Arg>(std::forward<Arg>(arg));
}



template <class Fn>
auto _make_universal(Fn&& f) -> function<Fn> {
    return function<Fn>(std::forward<Fn>(f));
}

#define make_func(NAME, F) make_first_class(fc_##NAME, F); \
    const auto NAME = _make_universal(fc_##NAME());

/* create first class print function */
make_func(count, fn_basic::count);
make_func(uprint, fn_basic::print);
make_func(reduce, fn_basic::reduce);
make_func(filter, fn_basic::filter);
make_func(fmap, fn_basic::map);
make_func(sum, fn_basic::sum);



#endif //HASKISH_TFN_H