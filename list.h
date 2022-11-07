//
// Created by gy gao on 11/7/22.
//

#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H

// 这个头文件包含了一个模板类 list
// list : 双向链表

// notes:
//
// 异常保证：
// mystl::list<T> 满足基本异常保证，部分函数无异常保证，并对以下等函数做强异常安全保证：
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{
    template<class T> struct list_node_base;
    template<class T> struct list_node;

    template<class T>
    struct node_traits
    {
        typedef list_node_base<T>* base_ptr;
        typedef list_node<T>*      node_ptr;
    };

    // list的节点结构
    template<class T>
    struct list_node_base
    {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        base_ptr prev;   // 前驱节点
        base_ptr next;   // 后继节点

        list_node_base() = default;

        base_ptr self()
        {
            return static_cast<base_ptr>(&*this);
        }

        node_ptr as_node()
        {
            return static_cast<node_ptr>(self());
        }

        void unlink()
        {
            prev = next = self();
        }
    };

    template<class T>
    struct list_node : public list_node_base<T>
    {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value;    // 数据域

        list_node() = default;
        list_node(const T& v) : value(v) {}
        list_node(T&& v) : value(mystl::move(v)) {}

        base_ptr as_node()
        {
            return static_cast<base_ptr>(&*this);
        }
        node_ptr self()
        {
            return static_cast<node_ptr>(&*this);
        }
    };

    // list迭代器设计
    template<class T>
    struct list_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag, T>
    {
        typedef T                                 value_type;
        typedef T*                                pointer;
        typedef T&                                reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator<T>                  self;

        base_ptr node_;    // 指向当前节点

        // 构造函数
        list_iterator() = default;
        list_iterator(base_ptr x) : node_(x) {}
        list_iterator(node_ptr x) : node_(x->as_node()) {}
        list_iterator(const list_iterator& rhs) : node_(rhs.node_) {}

        // 重载操作符
        reference operator*()  const {return node_->as_node()->value;}
        pointer   operator->() const {return &(operator*());}

        self& operator++()
        {
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--()
        {
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->prev;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }

        // 重载比较操作符
        bool operator==(const self& rhs) const {return node_ == rhs.node_;}
        bool operator!=(const self& rhs) const {return node_ != rhs.node_;}
    };


}

#endif //MYTINYSTL_LIST_H
