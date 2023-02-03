//
// Created by gy gao on 8/31/22.
//

#ifndef MYTINYSTL_CONSTRUCT_H
#define MYTINYSTL_CONSTRUCT_H

// 头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy : 负责对象的析构

#include <new>

#include "type_traits.h"
#include "iterator.h"

namespace mystl
{
    // construct
    template<class Ty>
    void construct(Ty* ptr)
    {
        ::new ((void *)ptr) Ty();
    }

    // placement new
    template<class Ty1, class Ty2>
    void construct(Ty1* ptr, const Ty2& value)
    {
        ::new ((void *)ptr) Ty1(value);
    }

    template<class Ty, class... Args>
    void construct(Ty* ptr, Args&&... args)
    {
        ::new ((void *)ptr) Ty(std::forward<Args>(args)...);
    }

    // destroy 将对象析构
    template<class Ty>
    void destroy_one(Ty*, std::true_type) {}

    template<class Ty>
    void destroy_one(Ty* pointer, std::false_type)
    {
        if (pointer != nullptr)
        {
            pointer->~Ty();
        }
    }

    template<class ForwardIter>
    void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

    template<class ForwardIter>
    void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
    {
        for (; first != last; ++first)
            destory(&(*first));
    }

    template<class Ty>
    void destroy(Ty* pointer)
    {
        destory_one(pointer, std::is_trivially_destructible<Ty>{});
    }

    template<class ForwardIter>
    void destroy(ForwardIter first, ForwardIter last)
    {
        destory_cat(first, last, std::is_trivially_destructible<
                typename iterator_traits<ForwardIter>::value_type>{});
    }

}


#endif //MYTINYSTL_CONSTRUCT_H
