//
// Created by gy gao on 10/11/22.
//

#ifndef MYOWNSTL_FUNCTIONAL_H
#define MYOWNSTL_FUNCTIONAL_H

// 这个头文件包含了myownstl的函数对象与哈希函数
#include <cstddef>

namespace myownstl
{
    // 定义一元函数的参数型别和返回值型别
    template<class Arg, class Result>
    struct unarg_function
    {
        typedef Arg        argument_type;
        typedef Result     result_type;
    };

    // 定义二元函数的参数型别的返回值型别
    template<class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1        first_argument_type;
        typedef Arg2        second_argument_type;
        typedef Result      result_type;
    };

    /*
     * 给隐含的this指针加const，表示这个this指向的东西是const的，也就是说这个函数中无法改动数据成员了。
     * const是一种保证，告诉你这个成员不会改变对象的状态。声明一个成员函数的时候用const关键字是用来说明
     * 这个函数是 “只读(read-only)”函数，也就是说明这个函数不会修改任何数据成员(object)。 为了声明一个const成员函数，
     * 把const关键字放在函数括号的后面。声明和定义的时候都应该放const关键字。任何不会修改数据成员的函数都应该
     * 声明为const类型。如果在编写const成员函数时，不慎修改了数据成员，或者调用了其它非const成员函数，编译器将指出错误，
     * 这无疑会提高程序的健壮性。
     * */
    // 函数对象：加法
    template<class T>
    struct plus : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const {return x + y;}
    };

    // 函数对象：减法
    template<class T>
    struct minus : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const {return x - y;}
    };

    // 函数对象：乘法
    template<class T>
    struct multiplies : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const {return x * y;}
    };

    // 函数对象：除法
    template<class T>
    struct divides : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const {return x / y;}
    };

    // 函数对象：模取
    template<class T>
    struct modulus : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const {return x % y;}
    };

    // 函数对象：否定
    template<class T>
    struct negate : public unarg_function<T, T>
    {
        T operator()(const T& x) const {return -x;}
    };

    // 加法的证同元素
    template<class T>
    T identity_element(plus<T>) {return T(0);}

    // 乘法的证同元素
    template<class T>
    T identity_element(multiplies<T>) {return T(1);}

    // 函数对象：等于
    template<class T>
    struct equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x == y;}
    };

    // 函数对象：不等于
    template<class T>
    struct not_equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x != y;}
    };

    // 函数对象：大于
    template<class T>
    struct greater : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x > y;}
    };

    // 函数对象：小于
    template<class T>
    struct less : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x < y;}
    };

    // 函数对象：大于等于
    template<class T>
    struct greater_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x >= y;}
    };

    // 函数对象：小于等于
    template<class T>
    struct less_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x <= y;}
    };

    // 函数对象：逻辑与
    template<class T>
    struct logical_and : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x && y;}
    };

    // 函数对象：逻辑或
    template<class T>
    struct logical_or : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const {return x || y;}
    };

    // 函数对象：逻辑非
    template<class T>
    struct logical_not : public binary_function<T, T, bool>
    {
        bool operator()(const T& x) const {return !x;}
    };

    // 证同函数：不会改变元素，返回本身
    template<class T>
    struct identity : public unarg_function<T, bool>
    {
        const T& operator()(const T& x) const {return x;}
    };

    // 选择函数：接受一个pair，返回第一个元素
    template<class Pair>
    struct selectfirst : public unarg_function<Pair, typename Pair::first_type>
    {
        const typename Pair::first_type& operator()(const Pair& x) const
        {
            return x.first;
        }
    };

    // 选择函数：接受一个pair，返回第二个元素
    template<class Pair>
    struct selectsecond : public unarg_function<Pair, typename Pair::second_type>
    {
        const typename Pair::second_type& operator()(const Pair& x) const
        {
            return x.second;
        }
    };

    // 投射函数：返回第一参数
    template<class Arg1, class Arg2>
    struct projectfirst : public binary_function<Arg1, Arg2, Arg1>
    {
        Arg1 operator()(const Arg1& x, const Arg2&) const {return x;}
    };

    // 投射函数：返回第二参数
    template<class Arg1, class Arg2>
    struct projectsecond : public binary_function<Arg1, Arg2, Arg1>
    {
        Arg2 operator()(const Arg1&, const Arg2 y) const {return y;}
    };

    /************************************************************************************************/
    // 哈希函数对象

    // 对于大部分类型，hash function 什么都不做
    template<class Key>
    struct hash {};

    // 针对指针的偏特化版本
    template<class T>
    struct hash<T*>
    {
        size_t operator()(T* p) const noexcept
        {return reinterpret_cast<size_t>(p);}
    };

// 对于整数类型，只是返回原值
#define MYOWNSTL_TRIVIAL_HASH_FCN(Type)            \
template <> struct hash<Type>                   \
{                                               \
    size_t operator()(Type val) const noexcept  \
    {return static_cast<size_t>(val);}          \
};

MYOWNSTL_TRIVIAL_HASH_FCN(bool)

MYOWNSTL_TRIVIAL_HASH_FCN(char)

MYOWNSTL_TRIVIAL_HASH_FCN(signed char)

MYOWNSTL_TRIVIAL_HASH_FCN(unsigned char)

MYOWNSTL_TRIVIAL_HASH_FCN(wchar_t)

MYOWNSTL_TRIVIAL_HASH_FCN(char16_t)

MYOWNSTL_TRIVIAL_HASH_FCN(char32_t)

MYOWNSTL_TRIVIAL_HASH_FCN(short)

MYOWNSTL_TRIVIAL_HASH_FCN(unsigned short)

MYOWNSTL_TRIVIAL_HASH_FCN(int)

MYOWNSTL_TRIVIAL_HASH_FCN(unsigned int)

MYOWNSTL_TRIVIAL_HASH_FCN(long)

MYOWNSTL_TRIVIAL_HASH_FCN(unsigned long)

MYOWNSTL_TRIVIAL_HASH_FCN(long long)

MYOWNSTL_TRIVIAL_HASH_FCN(unsigned long long)

#undef MYOWNSTL_TRIVIAL_HASH_FCN

    // 对于浮点数，逐位哈希
    inline size_t bitwise_hash(const unsigned char* first, size_t count)
    {
    #if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) && __SIZEOF_POINTER__ == 8)
        const size_t fnv_offset = 14695981039346656037ull;
        const size_t fnv_prime = 1099511628211ull;
    #else
        const size_t fnv_offset = 2166136261u;
        const size_t fnv_prime = 16777619u;
    #endif
        size_t result = fnv_offset;
        for (size_t i = 0; i < count; ++i)
        {
            result ^= (size_t)first[i];
            result *= fnv_prime;
        }

        return result;
    }

    template <>
    struct hash<float>
    {
        size_t operator()(const float& val)
        {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(float ));
        }
    };

    template <>
    struct hash<double>
    {
        size_t operator()(const double& val)
        {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(double));
        }
    };

    template <>
    struct hash<long double>
    {
        size_t operator()(const long double& val)
        {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(long double));
        }
    };

}

#endif //MYOWNSTL_FUNCTIONAL_H
