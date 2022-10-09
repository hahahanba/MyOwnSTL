//
// Created by gy gao on 10/9/22.
//

#ifndef MYTINYSTL_UNINITIALIZED_H
#define MYTINYSTL_UNINITIALIZED_H

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace stl
{
    /*****************************************************************************************/
    // uninitialized_copy
    // 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
    /*****************************************************************************************/
    template<class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                      ForwardIter result, std::true_type)
    {
        return mystl::copy(first, last, result);
    }

    template<class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                      ForwardIter result, std::false_type)
    {
        auto cur = result;
        try
        {
            for (; first != last; ++first, ++cur)
            {
                mystl::construct(&*cur, *first);
            }
        }
        catch (...)
        {
            for (; result != cur; --cur)
                mystl::destroy(&*cur);
        }
        return cur;
    }

    template <class InputIter, class ForwardIter>
    ForwardIter uninitialized_copy(InputIter first, InputIter last,
                                   ForwardIter result)
    {
        return mystl::unchecked_uninit_copy(first, last, result,
                                            std::is_trivially_copy_assignable<
                                            typename iterator_traits<ForwardIter>::value_type>{});
    }

    /*****************************************************************************************/
    // uninitialized_copy_n
    // 把 [first, first + n) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
    /*****************************************************************************************/
    template<class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::true_type)
    {
        return mystl::copy_n(first, n, result).second;
    }







}

#endif //MYTINYSTL_UNINITIALIZED_H
