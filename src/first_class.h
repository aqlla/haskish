//
// Created by Aquilla Sherrock on 8/11/15.
//

#ifndef HASKISH_TFN_H
#define HASKISH_TFN_H

#include <iostream>
#include <vector>
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

    // map
    template <class T, class... Args, template <class...> class C, class Fn>
    auto map(const C<T, Args...>& container, const Fn& f)
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
    auto reduce(const C<T, Args...>& container, const TRes& accumulator, const Fn& f)
            -> TRes {
        TRes result = accumulator;
        for (const auto& item : container) {
            result = f(result, item);
        }

        return result;
    };

    // filter
    template <class T, class... Args, template <class...> class C, class Fn>
    auto filter(const C<T, Args...>& container, const Fn& f)
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
    T sum(T arg, Args... args) {
        T result = arg;
        [&result](...){}((result += args, 0)...);
        return result;
    };

};

/* create first class print function */
make_first_class(count, fn_basic::count);


// override | operator for functional piping.
template <class T, class Fn>
auto operator |(T&& args, const Fn& f) -> decltype(f(args)) {
    return f(std::forward<T>(args));
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
    auto curry(T&& arg) const
            -> decltype(fn_universal<Fn, decltype(std::tuple_cat(before, std::make_tuple(arg))), After>(f, std::tuple_cat(before, std::make_tuple(arg)), after)) {
        return fn_universal<Fn, decltype(std::tuple_cat(before, std::make_tuple(arg))), After>(f, std::tuple_cat(before, std::make_tuple(std::forward<T>(arg))), after);
    }

    template <typename T>
    auto curry_right(T&& arg) const
            -> decltype(fn_universal<Fn, Before, decltype(std::tuple_cat(after, std::make_tuple(arg)))>(f, before, std::tuple_cat(after, std::make_tuple(arg)))) {
        return fn_universal<Fn, Before, decltype(std::tuple_cat(after, std::make_tuple(arg)))>(f, before, std::tuple_cat(after, std::make_tuple(std::forward<T>(arg))));
    }
};

template <class Fn>
auto _make_universal(Fn&& f) -> fn_universal<Fn> {
    return fn_universal<Fn>(std::forward<Fn>(f));
}

#define make_universal(NAME, F) make_first_class(fc_##NAME, F); \
    const auto NAME = _make_universal(fc_##NAME());

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


make_universal(fmap, fn_basic::map);
make_universal(reduce, fn_basic::reduce);
make_universal(filter, fn_basic::filter);
make_universal(sum, fn_basic::sum);
make_universal(uprint, fn_basic::print);



#endif //HASKISH_TFN_H
