//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_TESTS_H
#define HASKISH_TESTS_H

#include <iostream>
#include "../src/meta.h"
#include "../src/data_types.h"
#include "../src/static_list.h"

void test_sum() {
    using itp::cons;
    using itp::Int;

    using c1 = Int<1>;
    using c2 = Int<2>;
    using c3 = Int<3>;

    using list = cons<c1, cons<c2, cons<c3>>>;
}


template <int... xs>
void test_static_list() {
    using list = typename itp::static_list<xs...>::value;

    itp::static_for_each<list>() ([&](const int value) {
        std::cout << value << std::endl;
    });

    std::cout << "Size: " << itp::Size<list>::value   << std::endl;
    std::cout << "Sum:  " << itp::reduce<itp::add, list, 0>::value << std::endl;
}

#endif //HASKISH_TESTS_H
