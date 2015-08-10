//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_STATIC_LIST_H
#define HASKISH_STATIC_LIST_H

#include "data_types.h"

namespace itp
{
    template <class List>
    struct length;

    template <int, class, int=0>
    struct get;

    template <int, int>
    struct static_for;

    template <class List>
    struct for_each;

    template <class List, int... xs>
    struct _static_list;

    template <int... xs>
    struct static_list;

    template <class List, int accumulator>
    struct reduce;

    template<template<int, int> class, class, int=0>
    struct _reduce;


    // Static linked-list node.
    template <int data, class Next=nil>
    struct cons {
        static constexpr int value = data;
        using next = Next;
    };


    /* ------- Constructor functions -------- */

    template <int data, class List>
    struct push_front {
        using list = cons<data, List>;
    };


    template <class List, int x>
    struct _static_list<List, x> {
        using value = typename push_front<x, List>::list;
    };

    template <class List, int x, int... xs>
    struct _static_list<List, x, xs...> {
        using value = typename _static_list<typename push_front<x, List>::list, xs...>::value;
    };

    template <int x, int... xs>
    struct static_list<x, xs...> {
        using value = typename _static_list<cons<x, nil>, xs...>::value;
    };



    /* ------- Mutator functions -------- */

    template <template<int, int> class Fn, int accumulator, class xs, int x>
    struct _reduce<Fn, cons<x, xs>, accumulator> {
        static constexpr int value = _reduce<Fn, xs, Fn<accumulator, x>::value>::value;
    };

    template <template<int, int> class Fn, int accumulator>
    struct _reduce<Fn, nil, accumulator> {
        static constexpr int value = accumulator;
    };


    template <int data, class List, int accumulator>
    struct reduce<cons<data, List>, accumulator> {
        template <class Fn>
        constexpr int operator ()(Fn const& fn) {
            return fn(data, reduce<List, accumulator>()(fn));
        }
    };

    template <int data, int accumulator>
    struct reduce<cons<data, nil>, accumulator> {
        template <class Fn>
        constexpr int operator ()(Fn const& fn) {
            return fn(data, accumulator);
        }
    };




    template<int x, int y>
    struct add {
        static constexpr int value = x + y;
    };



    /* ------- Accessor functions -------- */

    /** Get the length of the list. */
    template <>
    struct length<nil> {
        static constexpr int value = 0;
    };

    template <int data, class List>
    struct length<cons<data, List>> {
        static constexpr int value = 1 + length<List>::value;
    };


    /** Get the value of a given list element. */
    template <int i, int data, class List, int Begin>
    struct get<i, cons<data, List>, Begin> {
        // the value at the specified index
        static constexpr int value = (i == Begin)
                                     ? data : get<i, List, Begin+1>::value;

        // boolean to check if the index was found.
        static constexpr bool found = (i == Begin)
                                      ? true : get<i, List, Begin+1>::found;
    };

    template <int i, int data, int Begin>
    struct get<i, cons<data, nil>, Begin> {
        static constexpr int value = (i == Begin) ? data : -1;
        static constexpr bool found = i == Begin;
    };



    /* --------- Static Iterators --------- */

    template <int Begin, int End>
    struct static_for {
        template <class Fn>
        void operator ()(Fn const& fn) const {
            if (Begin < End) {
                fn(Begin);
                static_for<Begin+1, End>()(fn);
            }
        }
    };

    template <int n>
    struct static_for<n, n> {
        template <class Fn>
        void operator ()(Fn const& fn) const {}
    };



    template <int data, class List>
    struct for_each<cons<data, List>> {
        template <class Fn>
        void operator ()(Fn const& fn) const {
            fn(data);
            for_each<List>()(fn);
        }
    };

    template <int data>
    struct for_each<cons<data, nil>> {
        template <class Fn>
        void operator ()(Fn const& fn) const {
            fn(data);
        }
    };
};

#endif //HASKISH_STATIC_LIST_H
