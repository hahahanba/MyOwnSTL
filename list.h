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
    struct list_iterator : public iterator<bidirectional_iterator_tag, T>
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

    template<class T>
    struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
    {
        typedef T                                 value_type;
        typedef const T*                          pointer;
        typedef const T&                          reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_const_iterator<T>            self;

        base_ptr node_;

        list_const_iterator() = default;
        list_const_iterator(base_ptr x) : node_(x) {}
        list_const_iterator(node_ptr x) : node_(x->as_base()) {}
        list_const_iterator(const list_iterator<T>& rhs) : node_(rhs.node_) {}
        list_const_iterator(const list_const_iterator& rhs) : node_(rhs.node_) {}

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

    // 模板类：list
    // 模板参数 T 代表数据类型
    template<class T>
    class list
    {
    public:
        // list的嵌套型别定义
        typedef mystl::allocator<T>                      allocator_type;
        typedef mystl::allocator<T>                      data_allocator;
        typedef mystl::allocator<list_node_base<T>>      base_allocator;
        typedef mystl::allocator<list_node<T>>           node_allocator;

        typedef typename allocator_type::value_type      value_type;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef list_iterator<T>                         iterator;
        typedef list_const_iterator<T>                   const_iterator;
        typedef mystl::reverse_iterator<iterator>        reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

        typedef typename node_traits<T>::base_ptr        base_ptr;
        typedef typename node_traits<T>::node_ptr        node_ptr;

        allocator_type get_allocator() {return node_allocator();}

    private:
        base_ptr  node_;    // 指向末尾节点
        size_type size_;    // 大小

    public:
        // 构造、复制、移动、析构函数
        list() {fill_init(0, value_type());}
        explicit list(size_type n) {fill_init(n, value_type());}
        list(size_type n, const T& value) { fill_init(n, value); }

        template<class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        list(Iter first, Iter last) {copy_init(first, last);}

        list(std::initializer_list<T> ilist) {copy_init(ilist.begin(), ilist.end());}
        list(const list& rhs) {copy_init(rhs.begin(), rhs.end());}
        list(list&& rhs) noexcept : node_(rhs.node_), size_(rhs.size_)
        {
            rhs.node_ = nullptr;
            rhs.size = 0;
        }

        list& operator=(const list& rhs)
        {
            if (this != &rhs)
            {
                assign(rhs.begin(), rhs.end());
            }
            return *this;
        }
        list& operator=(list&& rhs) noexcept
        {
            clear();
            splice(end(), rhs);
            return *this;
        }
        list& operator=(std::initializer_list<T> ilist)
        {
            list tmp(ilist.begin(), ilist.end());
            swap(tmp);
            return *this;
        }

        ~list()
        {
            if (node_)
            {
                clear();
                base_allocator::deallocate(node_);
                node_ = nullptr;
                size_ = 0;
            }
        }

    public:
        // 迭代器相关操作(双向链表)
        iterator               begin()             noexcept
        { return node_->next; }
        const_iterator         begin()       const noexcept
        { return node_->next; }
        iterator               end()               noexcept
        { return node_; }
        const_iterator         end()         const noexcept
        { return node_; }

        reverse_iterator       rbegin()            noexcept
        { return reverse_iterator(end()); }
        const_reverse_iterator rbegin()      const noexcept
        { return reverse_iterator(end()); }
        reverse_iterator       rend()              noexcept
        { return reverse_iterator(begin()); }
        const_reverse_iterator rend()        const noexcept
        { return reverse_iterator(begin()); }

        const_iterator         cbegin()      const noexcept
        { return begin(); }
        const_iterator         cend()        const noexcept
        { return end(); }
        const_reverse_iterator crbegin()     const noexcept
        { return rbegin(); }
        const_reverse_iterator crend()       const noexcept
        { return rend(); }

        // 1. 容器相关操作
        bool      empty()    const noexcept
        { return node_->next == node_; }

        size_type size()     const noexcept
        { return size_; }

        size_type max_size() const noexcept
        { return static_cast<size_type>(-1); }

        // 2. 访问元素相关操作
        reference       front()
        {
            MYSTL_DEBUG(!empty());
            return *begin();
        }

        const_reference front() const
        {
            MYSTL_DEBUG(!empty());
            return *begin();
        }

        reference       back()
        {
            MYSTL_DEBUG(!empty());
            return *(--end());
        }

        const_reference back()  const
        {
            MYSTL_DEBUG(!empty());
            return *(--end());
        }

        // 3. 调整容器相关操作
        // a) assign
        void     assign(size_type n, const value_type& value)
        { fill_assign(n, value); }

        template<class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        void     assign(Iter first, Iter last)
        { copy_assign(first, last); }

        void     assign(std::initializer_list<T> ilist)
        { copy_assign(ilist.begin(), ilist.end()); }

        // b) emplace_front / emplace_back / emplace
        template<class ...Args>
        void     emplace_front(Args&& ...args)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(mystl::forward<Args>(args)...);
            link_nodes_at_front(link_node->as_base(), link_node->as_base());
            ++size_;
        }

        template<class ...Args>
        void     emplace_back(Args&& ...args)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(mystl::forward<Args>(args)...);
            link_nodes_at_back(link_node->as_base(), link_node->as_base());
            ++size_;
        }

        template<class ...Args>
        iterator emplace(const_iterator pos, Args&& ...args)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(mystl::forward<Args>(args)...);
            link_nodes(pos.node_, link_node->as_base(), link_node->as_base());
            ++size_;
            return iterator(link_node);
        }

        // c) insert
        iterator insert(const_iterator pos, const value_type& value)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(value);
            ++size_;
            return link_iter_node(pos, link_node->as_base());
        }

        iterator insert(const_iterator pos, value_type&& value)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(mystl::move(value));
            ++size_;
            return link_iter_node(pos, link->as_base());
        }

        iterator insert(const_iterator pos, size_type n, const value_type& value)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
            return fill_insert(pos, n, value);
        }

        template<class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        iterator insert(const_iterator pos, Iter first, Iter last)
        {
            size_type n = mystl::distance(first, last);
            THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
            return copy_insert(pos, n, first);
        }

        // d) push_front / push_back
        void push_front(const value_type& value)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(value);
            link_nodes_at_front(link_node->as_base(), link_node->as_base());
            ++size_;
        }

        void push_front(value_type&& value)
        {
            emplace_front(mystl::move(value));
        }

        void push_back(const value_type& value)
        {
            THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
            auto link_node = create_node(value);
            link_nodes_at_back(link_node->as_base(), link_node->as_base());
            ++size_;
        }

        void push_back(value_type&& value)
        {
            emplace_back(mystl::move(value));
        }

        // e) pop_front / pop_back
        void pop_front()
        {
            MYSTL_DEBUG(!empty());
            auto n = node_->next;
            unlink_nodes(n, n);
            destroy_node(n->as_node());
            --size_;
        }

        void pop_back()
        {
            MYSTL_DEBUG(!empty());
            auto n = node_->prev;
            unlink_nodes(n, n);
            destroy_node(n->as_node());
            --size_;
        }

        // f) erase / clear
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        void clear();

        // g) resize
        void resize(size_type new_size) { resize(new_size, value_type()); }
        void resize(size_type new_size, const value_type& value);

        void swap(list& rhs) noexcept
        {
            mystl::swap(node_, rhs.node_);
            mystl::swap(node_, rhs.size_);
        }

        // list 相关操作
        // 1. splice
        void splice(const_iterator pos, list& other);
        void splice(const_iterator pos, list& other, const_iterator it);
        void splice(const_iterator pos, list& other, const_iterator first, const_iterator last)

        // 2. remove
        void remove(const value_type& value)
        {remove_if([&](const value_type& v){return v == value});}

        template<class UnaryPredicate>
        void remove_if(UnaryPredicate pred);

        // 3. unique
        void unique()
        {unique(mystl::equal_to<T>());}

        template<class BinaryPredicate>
        void unique(BinaryPredicate pred);

        // 4. merge
        void merge(list& x)
        { merge(x, mystl::less<T>());}

        template<class Compare>
        void merge(list& x, Compare comp);

        // 5. sort
        void sort()
        { list_sort(begin(), end(), size(), mystl::less<T>()); }

        template<class Compared>
        void sort(Compared comp)
        { list_sort(begin(), end(), size(), comp); }

        // 6. reverse
        void reverse();

    private:
        // helper function
        // 1. create/destroy node
        template<class ...Args>
        node_ptr create_node(Args&& ...args);
        void     destroy_node(node_ptr p);

        // 2. initialize
        void      fill_init(size_type n, const value_type& value);

        template<class Iter>
        void      copy_init(Iter first, Iter last);

        // 3. link/unlink
        iterator  link_iter_node(const_iterator pos, base_ptr node);
        void      link_nodes(base_ptr p, base_ptr first, base_ptr last);
        void      link_nodes_at_front(base_ptr first, base_ptr last);
        void      link_nodes_at_back(base_ptr first, base_ptr last);
        void      unlink_nodes(base_ptr f, base_ptr l);

        // 4. assign
        void      fill_assign(size_type n, const value_type& value);

        template<class Iter>
        void      copy_assign(Iter first, Iter last);

        // 5. insert
        iterator  fill_insert(const_iterator pos, size_type n, const value_type& value);

        template<class Iter>
        iterator  copy_insert(const_iterator pos, size_type n, Iter first);

        // 6. sort
        template<class Compared>
        iterator list_sort(iterator first, iterator last, size_type n, Compared comp);
    };

}

#endif //MYTINYSTL_LIST_H
