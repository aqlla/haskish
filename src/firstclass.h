//
// Created by Aquilla Sherrock on 8/11/15.
//

#ifndef HASKISH_TFN_H
#define HASKISH_TFN_H

#include <iostream>
#include <vector>


/* Create a generic functor to act as a first-class function. */
#define FIRST_CLASS(x) struct fc_##x {          \
    template <class... Ts>                      \
    auto operator()(Ts&&... ts) const -> decltype(x(std::forward<Ts>(ts)...)) { \
        return x(std::forward<Ts>(ts)...);      \
    }                                           \
};


/* function to test first-class creator. */
template <class T>
void print(T t) {
    std::cout << t << std::endl;
}

template <class T>
auto count(const T& container) -> int {
    return container.size();
}

/* create first class print function */
FIRST_CLASS(print);
FIRST_CLASS(count);


/* Apply a tuple to a function as an argument list. */
namespace fn_detail {
    template <int ...>
    struct int_sequence {};

    template <int n, int ...ns>
    struct gen_int_sequence : gen_int_sequence<n-1, n-1, ns...> {};

    template <int ...ns>
    struct gen_int_sequence<0, ns...> {
        using type = int_sequence<ns...>;
    };

    template <class Fn, class... Ts, int... ns>
    inline auto tuple_apply(int_sequence<ns...>, const Fn &fn, const std::tuple<Ts...> &params)
            -> decltype(fn((std::get<ns>(params))...)) {
        return fn((std::get<ns>(params))...);
    };
}

template <class Fn, class... Args>
inline auto tuple_apply(const Fn& f, const std::tuple<Args...>& params)
        -> decltype(f(std::declval<Args>()...)) {
    return fn_detail::tuple_apply(typename fn_detail::gen_int_sequence<sizeof...(Args)>::type(), f, params);
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
struct fn_universal {
private:
    Fn f;           // function
    Before before;  // curried args before
    After after;    // curried args after

public:
    fn_universal(Fn&& f)
            : f{std::forward<Fn>(f)},
              before{std::tuple<>()},
              after{std::tuple<>()} {}

    fn_universal(const Fn& f, const Before& before, const After& after)
            : f{f},
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
auto make_universal(Fn&& f) -> fn_universal<Fn> {
    return fn_universal<Fn>(std::forward<Fn>(f));
}

#define MAKE_UNIVERSAL(NAME, F) FIRST_CLASS(F); \
    const auto NAME = make_universal(fc_##F());


void test_fc() {
    fc_print print;
    print(5);
    print("Hi");

    // Function to have tuple applied to.
    auto f = [](int x, int y, int z) {
        return x + y - z;
    };

    // Param to apply to tuple as arguments.
    auto args = std::make_tuple(1, 2, 5);
    auto res = tuple_apply(f, args);
    print(res);

    auto list = std::make_tuple(1, 4);
    auto res2 = tuple_apply(f, list << 4);
    print(res2);

    fc_count count;   // Create first class count function instance.
    std::vector<std::string> slist = {"one", "two", "three"};

    // Pipe list to count and then pipe the result to the print.
    slist | count | print;


    // currying
    auto uf = make_universal(f);
    auto uf1 = uf << 1;
    auto uf2 = uf1 << 2 << 5;
    uf2() | print;
};


#endif //HASKISH_TFN_H
