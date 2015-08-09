#include <iostream>
#include "meta.h"

struct nil;

template <class DataType=int, class Next, DataType data>
struct Node {
    static constexpr DataType value = data;
    using next = Next;
};






int main() {
    test_sum();
    return 0;
}