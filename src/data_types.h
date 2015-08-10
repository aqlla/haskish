//
// Created by Aquilla Sherrock on 8/8/15.
//

#ifndef HASKISH_DATA_TYPES_H_H
#define HASKISH_DATA_TYPES_H_H


namespace itp
{
    struct nil;

    template<class T, T x>
    struct BaseType {
        static constexpr T value = x;
    };

    template<int  x> struct Int    : BaseType<int,  x> {};
    template<bool x> struct Bool   : BaseType<bool, x> {};
};

#endif //HASKISH_DATA_TYPES_H_H
