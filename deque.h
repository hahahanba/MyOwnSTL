//
// Created by gy gao on 10/14/22.
//

#ifndef MYTINYSTL_DEQUE_H
#define MYTINYSTL_DEQUE_H


// 这个头文件包含了一个模板类 deque
// deque: 双端队列

// notes:
//
// 异常保证：
// mystl::deque<T> 满足基本异常保证，部分函数无异常保证，并对以下等函数做强异常安全保证：
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco max")
#undef min
#endif // min

// deque map 初始化的大小
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

    template<class T>
    struct deque_buf_size
    {
        // Constant expressions
        static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    };

    // deque的迭代器设计
    template<class T, class Ref, class Ptr>
    struct deque_iterator : public iterator<random_access_iterator_tag, T>
    {
        typedef deque_iterator<T, T&, T*>             iterator;
        typedef deque_iterator<T, const T&, const T*> const_iterator;
        typedef deque_iterator                        self;

        typedef T            value_type;
        typedef Ptr          pointer;
        typedef Ref          reference;
        typedef size_t       size_type;
        typedef ptrdiff_t    difference_type;
        typedef T*           value_pointer;
        typedef T**          map_pointer;         // 指向buffer的整型指针，整型指针存放到一vector内？
        static const size_type buffer_size = deque_buf_size<T>::value;

        // 迭代器所含成员函数
        value_pointer cur;    // 指向所在缓冲区的当前元素
        value_pointer first;  // 指向所在缓冲区的头部
        value_pointer last;   // 指向所在缓冲区的尾部
        map_pointer node;     // 缓冲区所在节点

        // 构造、复制、移动函数
        deque_iterator() noex
    };


}



#endif //MYTINYSTL_DEQUE_H
