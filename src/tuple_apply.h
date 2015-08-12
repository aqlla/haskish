//
// Created by Aquilla Sherrock on 8/12/15.
//

#ifndef HASKISH_TUPLE_APPLY_HPP
#define HASKISH_TUPLE_APPLY_HPP

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
inline auto _tuple_apply(int_sequence<ns...>, const Fn& func, const std::tuple<Ts...>& args)
        -> decltype(func((std::get<ns>(args))...)) {
    return func((std::get<ns>(args))...);
};

template <class Fn, class... Args>
inline auto tuple_apply(const Fn& func, const std::tuple<Args...>& args)
        -> decltype(func(std::declval<Args>()...)) {
    return _tuple_apply(typename gen_int_sequence<sizeof...(Args)>::type(), func, args);
};


// override << operator to concatenate tuples
template <class... Tup, class New>
auto operator <<(const std::tuple<Tup...>& tup, const New& arg)
        -> std::tuple<Tup..., New> {
    return std::tuple_cat(tup, std::make_tuple(arg));
};


#endif //HASKISH_TUPLE_APPLY_HPP
