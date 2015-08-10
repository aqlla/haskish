//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_TESTS_H
#define HASKISH_TESTS_H

#include <iostream>
#include "../src/meta.h"
#include "../src/data_types.h"
#include "../src/static_list.h"

using itp::cons;
using itp::Int;
using itp::sum;

void test_sum() {
    using c1 = Int<1>;
    using c2 = Int<2>;
    using c3 = Int<3>;

    using list = cons<c1, cons<c2, cons<c3>>>;

    std::cout << sum<list>::result << std::endl;
}


void test_static_list() {
    using l1   = itp::Node<1>;
    using l2   = itp::PushBack<2, l1>::list;
    using l3   = itp::PushBack<3, l2>::list;
    using list = itp::PushBack<4, l3>::list;

    itp::static_for_each<list>() ([&](const int value) {
        std::cout << value << std::endl;
    });
}

#endif //HASKISH_TESTS_H
