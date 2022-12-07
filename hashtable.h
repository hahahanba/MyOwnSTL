//
// Created by gy gao on 12/2/22.
//

#ifndef MYTINYSTL_HASHTABLE_H
#define MYTINYSTL_HASHTABLE_H

// 这个头文件包含了一个模板类 hashtable
// hashtable : 哈希表，使用开链法处理冲突

#include <initializer_list>

#include "algo.h"
#include "functional.h"
#include "memory.h"
#include "vector.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{
    // hashtable的节点定义
    template<class T>
    struct hashtable_node
    {
        hashtable_node* next;
        T               value;

        hashtable_node() = default;
        hashtable_node(const T& n)
          : next(nullptr),
          value(n)
        {}

        hashtable_node(const hashtable_node& node)
          : next(node.next),
          value(node.value)
        {}

        hashtable_node(hashtable_node&& node)
          : next(node.next),
          value(mystl::move(node.value))
        {
            node.next = nullptr;
        }
    };

    // value traits
    // 执行类
    template<class T, bool>
    struct ht_value_traits_imp
    {
        typedef T key_type;
        typedef T mapped_type;
        typedef T value_type;

        template<class Ty>
        static const key_type& get_key(const Ty& value)
        {
            return value;
        }

        template<class Ty>
        static const value_type& get_value(const Ty& value)
        {
            return value;
        }
    };

    template<class T>
    struct ht_value_traits_imp<T, true>
    {
        typedef typename std::remove_cv<typename T::first_type>::type key_type;
        typedef typename T::second_type                               mapped_type;
        typedef T                                                     value_type;

        template<class Ty>
        static const key_type& get_key(const Ty& value)
        {
            return value.first;
        }

        template<class Ty>
        static const value_type& get_value(const Ty& value)
        {
            return value;
        }
    };

    template<class T>
    struct ht_value_traits
    {
        static constexpr bool is_map = mystl::is_pair<T>::value;

        typedef ht_value_traits_imp<T, is_map> value_traits_type;

        typedef typename value_traits_type::key_type    key_type;
        typedef typename value_traits_type::mapped_type mapped_type;
        typedef typename value_traits_type::value_type  value_type;

        template<class Ty>
        static const key_type& get_key(const Ty& value)
        {
            return value_traits_type::get_key(value);
        }

        template<class Ty>
        static const value_type& get_value(const Ty& value)
        {
            return value_traits_type::get_value(value);
        }
    };

    // forward declaration
    template<class T, class HashFun, class KeyEqual>
    class hashtable;

    template<class T, class HashFun, class KeyEqual>
    struct ht_iterator;

    template<class T, class HashFun, class KeyEqual>
    struct ht_const_iterator;

    template<class T>
    struct ht_local_iterator;

    template<class T>
    struct ht_const_local_iterator;

    // ht_iterator
    template<class T, class Hash, class KeyEqual>
    struct ht_iterator_base : public mystl::iterator<mystl::forward_iterator_tag, T>
    {
        typedef mystl::hashtable<T, Hash, KeyEqual>          hashtable;
        typedef ht_iterator_base<T, Hash, KeyEqual>          base;
        typedef mystl::ht_iterator<T, Hash, KeyEqual>        iterator;
        typedef mystl::ht_const_iterator<T, Hash, KeyEqual>  const_iterator;
        typedef hashtable_node<T>*                           node_ptr;
        typedef hashtable*                                   contain_ptr;
        typedef const node_ptr                               const_node_ptr;
        typedef const contain_ptr                            const_contain_ptr;

        typedef size_t                                       size_type;
        typedef ptrdiff_t                                    difference_type;

        node_ptr    node;  // 迭代器当前节点
        contain_ptr ht;    // 保持与容器的连接

        ht_iterator_base() = default;

        bool operator==(const base& rhs) const {return node == rhs.node;}
        bool operator!=(const base& rhs) const {return node != rhs.node;}
    };



}

#endif //MYTINYSTL_HASHTABLE_H
