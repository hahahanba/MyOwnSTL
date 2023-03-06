//
// Created by gy gao on 10/9/22.
//

#ifndef MYOWNSTL_UNINITIALIZED_H
#define MYOWNSTL_UNINITIALIZED_H

// 这个头文件用于对未初始化空间构造元素

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace myownstl
{
    /*****************************************************************************************/
    // uninitialized_copy
    // 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
    /*****************************************************************************************/
    template<class InputIter, class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first, InputIter last,
                                      ForwardIter result, std::true_type)
    {
        return myownstl::copy(first, last, result);
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
                myownstl::construct(&*cur, *first);
            }
        }
        catch (...)
        {
            for (; result != cur; --cur)
                myownstl::destroy(&*cur);
        }
        return cur;
    }

    template <class InputIter, class ForwardIter>
    ForwardIter uninitialized_copy(InputIter first, InputIter last,
                                   ForwardIter result)
    {
        return myownstl::unchecked_uninit_copy(first, last, result,
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
        return myownstl::copy_n(first, n, result).second;
    }

    template<class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::false_type)
    {
        auto cur = result;
        try
        {
            for (; n > 0; --n, ++cur, ++first)
            {
                myownstl::construct(&*cur, *first);
            }
        }
        catch (...)
        {
            for (; result != cur; --cur)
                myownstl::destroy(&*cur);
        }
        return cur;
    }

    template<class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result)
    {
        return myownstl::unchecked_uninit_copy_n(first, n, result,
                                              std::is_trivially_copy_assignable<
                                              typename iterator_traits<InputIter>::
                                              value_type>{});
    }

    /*****************************************************************************************/
    // uninitialized_fill
    // 在 [first, last) 区间内填充元素值
    /*****************************************************************************************/
    template<class ForwardIter, class T>
    void unchecked_uninit_fill(ForwardIter first, ForwardIter last,
                               const T& value, std::true_type)
    {
        myownstl::fill(first, last, value);
    }

    template<class ForwardIter, class T>
    void unchecked_uninit_fill(ForwardIter first, ForwardIter last,
                               const T& value, std::false_type)
    {
        auto cur = first;
        try
        {
            for (; cur != last; ++cur)
            {
                myownstl::construct(&*cur, value);
            }
        }
        catch (...)
        {
            for (; first != cur; ++first)
                myownstl::destroy(&*first);
        }
    }

    template<class ForwardIter, class T>
    void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value)
    {
        myownstl::unchecked_uninit_fill(first, last, value,
                                     std::is_trivially_copy_assignable<
                                     typename iterator_traits<ForwardIter>::
                                     value_type>{});
    }

    /*****************************************************************************************/
    // uninitialized_fill_n
    // 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
    /*****************************************************************************************/
    template<class ForwardIter, class Size, class T>
    ForwardIter
    unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type)
    {
        return myownstl::fill_n(first, n, value);
    }

    template<class ForwardIter, class Size, class T>
    ForwardIter
    unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type)
    {
        auto cur = first;
        try 
        {
            for (; n > 0; --n, ++cur)
            {
                myownstl::construct(&*cur, value);
            }
        }
        catch (...) 
        {
            for (; first != cur; ++first)
                myownstl::destroy(&*first);
        }
        return cur;
    }

    template<class ForwardIter, class Size, class T>
    ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value)
    {
        return myownstl::unchecked_uninit_fill_n(first, n, value,
                                              std::is_trivially_copy_assignable<
                                              typename iterator_traits<ForwardIter>::
                                              value_type>{});
    }

    /*****************************************************************************************/
    // uninitialized_move
    // 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
    /*****************************************************************************************/
    template<class InputIter, class ForwardIter>
    ForwardIter
    unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
    {
        return myownstl::move(first, last, result);
    }

    template<class InputIter, class ForwardIter>
    ForwardIter
    unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
    {
        ForwardIter cur = result;
        try
        {
            for (; first != last; ++first, ++cur)
            {
                myownstl::construct(&*cur, myownstl::move(*first));
            }
        }
        catch (...)
        {
            myownstl::destroy(result, cur);
        }
        return cur;
    }

    template<class InputIter, class ForwardIter>
    ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
    {
        return myownstl::unchecked_uninit_move(first, last, result,
                                            std::is_trivially_move_assignable<
                                            typename iterator_traits<InputIter>::
                                            value_type>{});
    }

    /*****************************************************************************************/
    // uninitialized_move_n
    // 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
    /*****************************************************************************************/
    template<class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::true_type)
    {
        return myownstl::move(first, first + n, result);
    }

    template<class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::false_type)
    {
        auto cur = result;
        try
        {
            for (; n > 0; --n, ++first, ++cur)
            {
                myownstl::construct(&*cur, myownstl::move(*first));
            }
        }
        catch (...)
        {
            for (; result != cur; ++result)
                myownstl::destroy(&*result);
            throw;
        }
        return cur;
    }

    template<class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result)
    {
        return myownstl::unchecked_uninit_move_n(first, n, result,
                                              std::is_trivially_move_assignable<
                                              typename iterator_traits<InputIter>::
                                              value_type>{});
    }
}

#endif //MYOWNSTL_UNINITIALIZED_H
