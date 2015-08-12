//
// Created by Aquilla Sherrock on 8/9/15.
//

#ifndef HASKISH_TESTS_H
#define HASKISH_TESTS_H

#include <iostream>
#include "../src/data_types.h"
#include "../src/static_list.h"
#include "../src/firstclass.h"

using std::cout;
using std::endl;
using namespace itp;


void test_fc() {
    fc_print printer;

    // Function to have tuple applied to.
    auto f = [](int x, int y, int z) {
        return x + y - z;
    };

    fc_count count;   // Create first class count function instance.
    std::vector<std::string> slist = {"one", "two", "three"};

    // Pipe list to count and then pipe the result to the printer.
    slist | count | printer;


    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Test Reduce
    nums | reduce >> 0 >> sum | uprint << "\nSum: ";

    // Test fmap
    auto squares = nums | fmap >> [](int n) {
        return n*n;
    };

    "\nSquares:" | uprint;
    for (const auto& item : squares)
        item | uprint;


    // Test filter
    auto evens = nums | filter >> [](int n) {
        return (n % 2) == 0;
    };

    "\nEvens:" | uprint;
    for (const auto& item : evens)
        item | uprint;
};



void test_static_list() {
    // Create a compile-time linked list.
    using xs = typename pre::static_list<3, 7, 2, 12, 6>::value;

    // print the size of the list.
    cout << "length: "
         // access the value with the `value` member.
         << pre::length<xs>::value
         << endl << endl;


    // for_each loop accesses each element in order.
    pre::for_each<xs> foreach_xs;

    // Use a lambda function to handle data in the for_each loop;
    auto print_list = [](int value) { cout << value << endl; };

    // The loop oblect for your list will take the lambda as its argument.
    foreach_xs(print_list);
    cout << endl;


    // Or just do it all in one if ur not a pewsea.
    pre::for_each<xs>() ([](const int value) {
        cout << value * value << endl;
    });


    // reduce works in much the same way, allowing you do define a lambda to
    // accumulate the element values into one number.
    auto adder = [](int x, int a) -> int { return x + a; };
    int sum_xs = pre::reduce<xs, 0>()(adder);

    cout << "\nSum: " 
         << sum_xs
         << endl
         << endl;
}

#endif //HASKISH_TESTS_H
