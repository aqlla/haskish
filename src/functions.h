//
// Created by Aquilla Sherrock on 8/12/15.
//

#ifndef HASKISH_ITFUNCTIONAL_H
#define HASKISH_ITFUNCTIONAL_H

#include <type_traits>
#include <iostream>

namespace fn_basic {
    template<class... Args>
    void print(Args... args) {
        (void) (int[]) {((std::cout << args), 0)...};
        std::cout << std::endl;
    }

    template <>
    void print<std::vector<int>>(std::vector<int> arg) {
        for (const auto& i : arg) {
            std::cout << i << std::endl;
        }
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
    auto reduce(const C<T, Args...>& container, const TRes& accumulator, const Fn& f) -> TRes {
        TRes result = accumulator;
        for (const auto& item : container) {
            result = f(result, item);
        }

        return result;
    };

    // filter
    template <class T, class... Args, template <class...> class C, class Fn>
    auto filter(const C<T, Args...>& container, const Fn& f) -> C<T, Args...> {
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

#endif //HASKISH_ITFUNCTIONAL_H
