//
// Created by gy gao on 8/14/22.
//

#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H

// 用于迭代器的设计，包含一些模板结构体与全局函数
#include <cstddef>

#include "type_traits.h"

namespace mystl
{
    /*****************************************************************************/
    // 5种迭代器类型
    /*****************************************************************************/
    // Category tags (class 属性)
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    /*****************************************************************************/
    // Iterator base class
    /*****************************************************************************/
    // iterator 类模板(class template)
    template<class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category                 iterator_category;
        typedef T                        value_type;
        typedef Pointer                  pointer;
        typedef Reference                reference;
        typedef Distance                 difference_type;
    };
    // 模版对类型做操作，不是对变量做操作

    /*****************************************************************************/
    // iterator traits cat(category)
    /*****************************************************************************/
    // 当T定义了嵌套类型iterator_category时，第二个测试的实例化工作成功，空指针常量(0)被成功传递。
    // (表达式的结果类型是char。)如果它不起作用，惟一可用的函数是第一个测试，并且表达式的结果类型
    // 是two。使用省略号不仅是因为它会接受任何参数，还因为它的转换级别最低，所以如果可能的话，
    // 首选调用第二个函数。
    // WIKI : Substitution failure is not an error
    template<class T>
    struct has_iterator_cat
    {
    private:
        // Types "char" and "two" are guaranteed to have different sizes,
        // specifically sizeof(two) == 2 and sizeof(char) == 1.
        struct two {char a; char b;};
        template<class U> static two test(...);
        template<class U> static char test(typename U::iterator_category* = 0);
    public:
        // If the "sizeof" of the result of calling test<T>(0) is equal to
        // sizeof(char), the second overload worked and T has a nested type named
        // iterator_category.
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    // impl泛化
    template<class Iterator, bool>
    struct iterator_traits_impl {};

    // impl特化
    template<class Iterator>
    struct iterator_traits_impl<Iterator, true>
    {
        typedef typename Iterator::iterator_category  iterator_category;
        typedef typename Iterator::value_type         value_type;
        typedef typename Iterator::pointer            pointer;
        typedef typename Iterator::reference          reference;
        typedef typename Iterator::difference_type    difference_type;
    };

    // helper泛化
    template<class Iterator, bool>
    struct iterator_traits_helper {};

    // helper特化 (是否继承于input/output_iterator_tag)
    template<class Iterator>
    struct iterator_traits_helper<Iterator, true>
            : public iterator_traits_impl<Iterator,
            std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
            std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
    {};

    // iterator_traits 萃取迭代器的特性
    // 1.泛化版本
    template<class Iterator>
    struct iterator_traits
            : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

    // 2.针对原生指针的偏特化版本
    template<class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag       iterator_category;
        typedef T                                value_type;
        typedef T*                               pointer;
        typedef T&                               reference;
        typedef ptrdiff_t                        difference_type;
    };

    template<class T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag       iterator_category;
        typedef T                                value_type;
        typedef const T*                         pointer;
        typedef const T&                         reference;
        typedef ptrdiff_t                        difference_type;
    };

    /*****************************************************************************/
    // type_traits实现
    /*****************************************************************************/
    // template <class From, class To> struct is_convertible;
    // Trait class that identifies whether From is implicitly convertible to To.
    // 是否能将T的迭代器类型转换成U的迭代器类型
    template<class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of
            : public mystl::m_bool_constant<std::is_convertible<
                    typename iterator_traits<T>::iterator_category, U>::value>
    {};

    // 萃取某种迭代器
    template<class T, class U>
    struct has_iterator_cat_of<T, U, false> : public m_false_type {};

    template<class Iter>
    struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

    template<class Iter>
    struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

    template<class Iter>
    struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

    template<class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

    template<class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

    // 用两种迭代器类型判断是否为迭代器
    template<class Iterator>
    struct is_iterator
            : public m_bool_constant<is_input_iterator<Iterator>::value ||
                    is_output_iterator<Iterator>::value>
    {};

    // 任何时候当你想要在 template 中指涉一个嵌套从属类型名称，
    // 就必须在紧临它的前一个位置放上关键字 typename。
    // 1. 萃取某个迭代器的category
    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    } // 返回值为typename iterator_traits<Iterator>::iterator_category

    // 2. 萃取某个迭代器的distance_type
    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&)
    {
        // static_cast返回
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 3. 萃取某个迭代器的value_type
    template<class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // 迭代器操作：迭代器距离与迭代器前进n的距离
    // 连续空间的距离用整数，非连续空间用指针
    // 以下函数用于计算迭代器间的距离
    // 1. diatance 的 input_iterator_tag 的版本
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    // 2. diatance 的 random_access_iterator_tag 的版本
    template<class RandomIter>
    typename iterator_traits<RandomIter>::difference_type
    distance_dispatch(RandomIter first, RandomIter last,
                      random_access_iterator_tag)
    {
        return last - first;
    }

    // 整合以上两个版本
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last)
    {
        return distance_dispatch(first, last, iterator_category(first));
    }

    // 以下迭代器用于让迭代器前进n个距离
    // 1. advance的input_iterator_tag的版本
    template<class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
    {
        while (n--)
            ++i;
    }

    // 2. advance的bidirectional_iterator_tag的版本
    template<class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
    {
        if (n >= 0)
            while (n--) ++i;
        else
            while (n++) --i;
    }

    // 3. advance的random_access_iterator_tag的版本
    template<class RandomIter, class Distance>
    void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
    {
        i += n;
    }

    // 整合上述三个版本
    template<class InputIterator, class Distance>
    void advance(InputIterator& i, Distance n)
    {
        advance_dispatch(i, n, iterator_category(i));
    }

    /*************************************************************************************/
    // 模版类 ：reverse_iterator
    // 反向迭代器
    template<class Iterator>
    class reverse_iterator
    {
    private:
        Iterator current;

    public:
        // 反向迭代器的五种相应型别
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type        value_type;
        typedef typename iterator_traits<Iterator>::difference_type   difference_type;
        typedef typename iterator_traits<Iterator>::pointer           pointer;
        typedef typename iterator_traits<Iterator>::reference         reference;

        typedef Iterator                                              iterator_type;
        typedef reverse_iterator<Iterator>                            self;

    public:
        // 构造函数
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type i)
            : current(i)
        {}
        reverse_iterator(const self& rhs)
            : current(rhs.current)
        {}

    public:
        // 取出对应的正向迭代器
        iterator_type base() const
        {return current;}

        // 重载操作符
        reference operator*() const
        {// 对应正向迭代器的前一个位置
            auto tmp = current;
            return *--tmp;
        }
        pointer operator->() const
        {
            return &(operator*());
        }

        // 前进(++)变为后退(--)
        self& operator++()
        {
            --current;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            --current;
            return tmp;
        }

        // 后退(--)变为前进(++)
        self& operator--()
        {
            ++current;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            ++current;
            return tmp;
        }

        self& operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }
        self operator+(difference_type n) const
        {//显式调用构造函数
            return self(current - n);
        }
        self& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }
        self operator-(difference_type n)
        {// 减号
            return self(current + n);
        }
        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }
    };

    // overloading operator-
    template<class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& lhs,
              const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() - lhs.base();
    }

    // 重载比较运算符
    template<class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template<class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() > rhs.base();
    }

    template<class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() != rhs.base();
    }

    template<class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() < rhs.base();
    }

    template<class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() >= rhs.base();
    }

    template<class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() <= rhs.base();
    }
}


#endif //MYTINYSTL_ITERATOR_H
