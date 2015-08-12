//
// Created by Aquilla Sherrock on 8/11/15.
//

#ifndef HASKISH_TFN_H
#define HASKISH_TFN_H

#include <iostream>
#include <vector>


/* Create a generic functor to act as a first-class function. */
#define make_first_class(NAME, X)                        \
struct NAME {                                            \
    template <class... Args>                             \
    auto operator()(Args&&... ts) const                  \
            -> decltype(X(std::forward<Args>(ts)...)) {  \
        return X(std::forward<Args>(ts)...);             \
    }                                                    \
};


namespace fn_basic {
    template<class... Args>
    void print(Args... args) {
        (void) (int[]) {((std::cout << args), 0)...};
        std::cout << std::endl;
    }

    template<class T>
    auto count(const T &container) -> int {
        return container.size();
    }
};

using fn_basic::count;
using fn_basic::print;

/* create first class print function */
make_first_class(count, fn_basic::count);


/* Apply a tuple to a function as an argument list. */
template <int ...>
struct int_sequence {};

template <int n, int ...ns>
struct gen_int_sequence : gen_int_sequence<n-1, n-1, ns...> {};

template <int ...ns>
struct gen_int_sequence<0, ns...> {
    using type = int_sequence<ns...>;
};

template <class Fn, class... Ts, int... ns>
inline auto _tuple_apply(int_sequence<ns...>, const Fn &fn, const std::tuple<Ts...> &params)
-> decltype(fn((std::get<ns>(params))...)) {
    return fn((std::get<ns>(params))...);
};

template <class Fn, class... Args>
inline auto tuple_apply(const Fn& f, const std::tuple<Args...>& params)
-> decltype(f(std::declval<Args>()...)) {
    return _tuple_apply(typename gen_int_sequence<sizeof...(Args)>::type(), f, params);
};


// override << operator to concatenate tuples
template <class... Tup, class New>
auto operator <<(const std::tuple<Tup...>& tup, const New& arg) -> std::tuple<Tup..., New> {
    return std::tuple_cat(tup, std::make_tuple(arg));
};


// override | operator for functional piping.
template <class T, class Fn>
auto operator |(T&& param, const Fn& f) -> decltype(f(param)) {
    return f(std::forward<T>(param));
};


// univeral function wrapper
template <class Fn, class Before = std::tuple<>, class After = std::tuple<>>
struct fn_universal
{
private:
    Fn f;           // function
    Before before;  // curried args before
    After after;    // curried args after

public:
    fn_universal(Fn&& f)
            : f(std::forward<Fn>(f)),
              before{std::tuple<>()},
              after{std::tuple<>()} {}

    fn_universal(const Fn& f, const Before& before, const After& after)
            : f(f),
              before{before},
              after{after} {}

    template <class... Args>
    auto operator ()(Args... args) const
            -> decltype(tuple_apply(f, std::tuple_cat(before, std::make_tuple(args...), after))) {
        return tuple_apply(f, std::tuple_cat(before, std::make_tuple(std::forward<Args>(args)...), after));
    }

    template <class T>
    auto curry(T&& param) const
            -> decltype(fn_universal<Fn, decltype(std::tuple_cat(before, std::make_tuple(param))), After>(f, std::tuple_cat(before, std::make_tuple(param)), after)) {
        return fn_universal<Fn, decltype(std::tuple_cat(before, std::make_tuple(param))), After>(f, std::tuple_cat(before, std::make_tuple(std::forward<T>(param))), after);
    }

    template <typename T>
    auto curry_right(T&& param) const
            -> decltype(fn_universal<Fn, Before, decltype(std::tuple_cat(after, std::make_tuple(param)))>(f, before, std::tuple_cat(after, std::make_tuple(param)))) {
        return fn_universal<Fn, Before, decltype(std::tuple_cat(after, std::make_tuple(param)))>(f, before, std::tuple_cat(after, std::make_tuple(std::forward<T>(param))));
    }
};

// override << operator or left curry
template <class UF, class Arg>
auto operator <<(const UF& f, Arg&& arg)
        -> decltype(f.template curry<Arg>(std::forward<Arg>(arg))) {
    return f.template curry<Arg>(std::forward<Arg>(arg));
};

// override >> operator or right curry
template <class UF, class Arg>
auto operator >>(const UF& f, Arg&& arg)
        -> decltype(f.template curry_right<Arg>(std::forward<Arg>(arg))) {
    return f.template curry_right<Arg>(std::forward<Arg>(arg));
};

template <class Fn>
auto _make_universal(Fn&& f) -> fn_universal<Fn> {
    return fn_universal<Fn>(std::forward<Fn>(f));
}

#define make_universal(NAME, F) make_first_class(fc_##NAME, F); \
    const auto NAME = _make_universal(fc_##NAME());


/* Practical example functions. */

// map
template <class T, class... Args, template <class...> class C, class Fn>
auto fn_map(const C<T, Args...>& container, const Fn& f)
        -> C<decltype(f(std::declval<T>()))> {
    using resType = decltype(f(std::declval<T>()));
    C<resType> result;
    for (const auto& item : container) {
        result.push_back(f(item));
    }

    return result;
};

// reduce
template <class TRes, class T, class... Args, template <class...> class C, class Fn>
auto fn_reduce(const C<T, Args...>& container, const TRes& accumulator, const Fn& f)
        -> TRes {
    TRes result = accumulator;
    for (const auto& item : container) {
        result = f(result, item);
    }

    return result;
};

// filter
template <class T, class... Args, template <class...> class C, class Fn>
auto fn_filter(const C<T, Args...>& container, const Fn& f)
        -> C<T, Args...> {
    C<T, Args...> result;
    for (const auto& item : container) {
        if (f(item)) {
            result.push_back(item);
        }
    }

    return result;
};


template <class T, class... Args>
T sum_impl(T arg, Args... args) {
    T result = arg;
    [&result](...){}((result += args, 0)...);
    return result;
};


make_universal(fmap, fn_map);
make_universal(reduce, fn_reduce);
make_universal(filter, fn_filter);
make_universal(sum, sum_impl);
make_universal(uprint, print);



#endif //HASKISH_TFN_H
