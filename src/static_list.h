//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_STATIC_LIST_H
#define HASKISH_STATIC_LIST_H

#include "data_types.h"

namespace itp
{
    template <typename T>
    struct Size;

    template <int, class, int=0>
    struct Get;

    template <int, int>
    struct static_for;

    template <class List>
    struct static_for_each;

    template <class List, int... xs>
    struct make_static_list;

    template <int... xs>
    struct static_list;

    template<template<int, int> class, class, int>
    struct reduce;


    // Static linked-list node.
    template <int data, class Next=nil>
    struct Node {
        static constexpr int value = data;
        using next = Next;
    };


    /* ------- Constructor functions -------- */

    template <int data, class List>
    struct PushFront {
        using list = Node<data, List>;
    };


    template <class List, int x>
    struct make_static_list<List, x> {
        using value = typename PushFront<x, List>::list;
    };

    template <class List, int x, int... xs>
    struct make_static_list<List, x, xs...> {
        using value = typename make_static_list<typename PushFront<x, List>::list, xs...>::value;
    };

    template <int x, int... xs>
    struct static_list<x, xs...> {
        using value = typename make_static_list<Node<x, nil>, xs...>::value;
    };



    /* ------- Mutator functions -------- */

    template <template<int, int> class Fn, int accumulator, class xs, int x>
    struct reduce<Fn, Node<x, xs>, accumulator> {
        static constexpr int value = reduce<Fn, xs, Fn<accumulator, x>::value>::value;
    };

    template <template<int, int> class Fn, int accumulator>
    struct reduce<Fn, nil, accumulator> {
        static constexpr int value = accumulator;
    };

    template<int x, int y>
    struct add {
        static constexpr int value = x + y;
    };



    /* ------- Accessor functions -------- */

    /** Get the length of the list. */
    template <>
    struct Size<nil> {
        static constexpr int value = 0;
    };

    template <int data, class List>
    struct Size<Node<data, List>> {
        static constexpr int value = 1 + Size<List>::value;
    };


    /** Get the value of a given list element. */
    template <int i, int data, class List, int Begin>
    struct Get<i, Node<data, List>, Begin> {
        // the value at the specified index
        static constexpr int value = (i == Begin)
                                     ? data
                                     : Get<i, List, Begin+1>::value;

        // boolean to check if the index was found.
        static constexpr bool found = (i == Begin)
                                      ? true
                                      : Get<i, List, Begin+1>::found;
    };

    template <int i, int data, int Begin>
    struct Get<i, Node<data, nil>, Begin> {
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
    struct static_for_each<Node<data, List>> {
        template <class Fn>
        void operator ()(Fn const& fn) {
            fn(data);
            static_for_each<List>()(fn);
        }
    };

    template <int data>
    struct static_for_each<Node<data, nil>> {
        template <class Fn>
        void operator ()(Fn const& fn) {
            fn(data);
        }
    };
};

#endif //HASKISH_STATIC_LIST_H
