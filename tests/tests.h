//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_TESTS_H
#define HASKISH_TESTS_H

#include <iostream>
#include "../src/data_types.h"
#include "../src/static_list.h"

void test_static_list() {
    // Create a compile-time linked list.
    using xs = typename itp::static_list<3, 7, 2, 12, 6>::value;

    // print the size of the list.
    std::cout << "length: "
              // access the value with the `value` member.
              << itp::length<xs>::value
              << std::endl << std::endl;


    // for_each loop accesses each element in order.
    itp::for_each<xs> foreach_xs;

    // Use a lambda function to handle data in the for_each loop;
    auto print_list = [](int value) {
        std::cout << value << std::endl;
    };

    // The loop oblect for your list will take the lambda as its argument.
    foreach_xs(print_list);
    std::cout << std::endl;


    // Or just do it all in one if ur not a pewsea.
    itp::for_each<xs>() ([](const int value) {
        std::cout << value * value << std::endl;
    });


    // reduce works in much the same way, allowing you do define a lambda to
    // accumulate the element values into one number.
    auto adder = [](int x, int a)
            -> int { return x + a; };

    int sum_xs = itp::reduce<xs, 0>()(adder);

    std::cout << "\nSum: " << sum_xs << std::endl << std::endl;
}

#endif //HASKISH_TESTS_H
