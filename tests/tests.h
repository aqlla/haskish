//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_TESTS_H
#define HASKISH_TESTS_H

#include <iostream>
#include "../src/data_types.h"
#include "../src/static_list.h"

using itp::static_foreach;
using itp::length;
using itp::reduce;
using itp::add;

template <int... xs>
void test_static_list() {
    using list = typename itp::static_list<xs...>::value;

    static_foreach<list>() ([](int value) {
        std::cout << value << std::endl;
    });

    std::cout << "length: " << length<list>::value << std::endl;
    std::cout << "Sum:  "   << reduce<add, list>::value << std::endl;
}

#endif //HASKISH_TESTS_H
