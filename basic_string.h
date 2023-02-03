//
// Created by gy gao on 11/25/22.
//

#ifndef MYTINYSTL_BASIC_STRING_H
#define MYTINYSTL_BASIC_STRING_H

// 包含一个模板类 basic_string 用于表示字符串类型

#include <iostream>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "exceptdef.h"

namespace mystl
{

    // char_traits

    template <class CharType>
    struct char_traits
    {
        typedef CharType char_type;

        static size_t length(const char_type* str)
        {
            size_t len = 0;
            for (; *str != char_type(0); ++str)
                ++len;
            return len;
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n)
        {
            for (; n != 0; --n, ++s1, ++s2)
            {
                if (*s1 < *s2)
                    return -1;
                if (*s2 < *s1)
                    return 1;
            }
            return 0;
        }

        static char_type* copy(char_type* dst, const char_type* src, size_t n)
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            char_type* r = dst;
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
            return r;
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n)
        {
            char_type* r = dst;
            if (dst < src)
            {
                for (; n != 0; --n, ++dst, ++src)
                    *dst = *src;
            }
            else if (src < dst)
            {
                dst += n;
                src += n;
                for (; n != 0; --n)
                    *--dst = *--src;
            }
            return r;
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count)
        {
            char_type* r = dst;
            for (; count > 0; --count, ++dst)
                *dst = ch;
            return r;
        }
    };

    // Partialized. char_traits<char>
    template <>
    struct char_traits<char>
    {
        typedef char char_type;

        static size_t length(const char_type* str) noexcept
        { return std::strlen(str); }

        static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept
        { return std::memcmp(s1, s2, n); }

        static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            return static_cast<char_type*>(std::memcpy(dst, src, n));
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
        {
            return static_cast<char_type*>(std::memmove(dst, src, n));
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
        {
            return static_cast<char_type*>(std::memset(dst, ch, count));
        }
    };

    // Partialized. char_traits<wchar_t>
    template <>
    struct char_traits<wchar_t>
    {
        typedef wchar_t char_type;

        static size_t length(const char_type* str) noexcept
        {
            return std::wcslen(str);
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept
        {
            return std::wmemcmp(s1, s2, n);
        }

        static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            return static_cast<char_type*>(std::wmemcpy(dst, src, n));
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
        {
            return static_cast<char_type*>(std::wmemmove(dst, src, n));
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
        {
            return static_cast<char_type*>(std::wmemset(dst, ch, count));
        }
    };

    // Partialized. char_traits<char16_t>
    template <>
    struct char_traits<char16_t>
    {
        typedef char16_t char_type;

        static size_t length(const char_type* str) noexcept
        {
            size_t len = 0;
            for (; *str != char_type(0); ++str)
                ++len;
            return len;
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept
        {
            for (; n != 0; --n, ++s1, ++s2)
            {
                if (*s1 < *s2)
                    return -1;
                if (*s2 < *s1)
                    return 1;
            }
            return 0;
        }

        static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            char_type* r = dst;
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
            return r;
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
        {
            char_type* r = dst;
            if (dst < src)
            {
                for (; n != 0; --n, ++dst, ++src)
                    *dst = *src;
            }
            else if (src < dst)
            {
                dst += n;
                src += n;
                for (; n != 0; --n)
                    *--dst = *--src;
            }
            return r;
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
        {
            char_type* r = dst;
            for (; count > 0; --count, ++dst)
                *dst = ch;
            return r;
        }
    };

    // Partialized. char_traits<char32_t>
    template <>
    struct char_traits<char32_t>
    {
        typedef char32_t char_type;

        static size_t length(const char_type* str) noexcept
        {
            size_t len = 0;
            for (; *str != char_type(0); ++str)
                ++len;
            return len;
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept
        {
            for (; n != 0; --n, ++s1, ++s2)
            {
                if (*s1 < *s2)
                    return -1;
                if (*s2 < *s1)
                    return 1;
            }
            return 0;
        }

        static char_type* copy(char_type* dst, const char_type* src, size_t n) noexcept
        {
            MYSTL_DEBUG(src + n <= dst || dst + n <= src);
            char_type* r = dst;
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
            return r;
        }

        static char_type* move(char_type* dst, const char_type* src, size_t n) noexcept
        {
            char_type* r = dst;
            if (dst < src)
            {
                for (; n != 0; --n, ++dst, ++src)
                    *dst = *src;
            }
            else if (src < dst)
            {
                dst += n;
                src += n;
                for (; n != 0; --n)
                    *--dst = *--src;
            }
            return r;
        }

        static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept
        {
            char_type* r = dst;
            for (; count > 0; --count, ++dst)
                *dst = ch;
            return r;
        }
    };

    // 初始化 basic_string 尝试分配的最小 buffer 大小，可能被忽略
#define STRING_INIT_SIZE 32

    // 模板类 basic_string
    // 参数一代表字符类型，参数二代表萃取字符类型的方式，缺省使用 mystl::char_traits
    template <class CharType, class CharTraits = mystl::char_traits<CharType>>
    class basic_string
    {
    public:
        typedef CharTraits                               traits_type;
        typedef CharTraits                               char_traits;

        typedef mystl::allocator<CharType>               allocator_type;
        typedef mystl::allocator<CharType>               data_allocator;

        typedef typename allocator_type::value_type      value_type;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef value_type*                              iterator;
        typedef const value_type*                        const_iterator;
        typedef mystl::reverse_iterator<iterator>        reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

        allocator_type get_allocator() { return allocator_type(); }

        static_assert(std::is_pod<CharType>::value, "Character type of basic_string must be a POD");
        static_assert(std::is_same<CharType, typename traits_type::char_type>::value,
                      "CharType must be same as traits_type::char_type");

    public:
        // 末尾位置的值，例:
        // if (str.find('a') != string::npos) { /* do something */ }
        static constexpr size_type npos = static_cast<size_type>(-1);

    private:
        iterator  buffer_;  // 储存字符串的起始位置
        size_type size_;    // 大小
        size_type cap_;     // 容量

    public:
        // 构造、复制、移动、析构函数

        basic_string() noexcept
        { try_init(); }

        basic_string(size_type n, value_type ch)
                :buffer_(nullptr), size_(0), cap_(0)
        {
            fill_init(n, ch);
        }

        basic_string(const basic_string& other, size_type pos)
                :buffer_(nullptr), size_(0), cap_(0)
        {
            init_from(other.buffer_, pos, other.size_ - pos);
        }
        basic_string(const basic_string& other, size_type pos, size_type count)
                :buffer_(nullptr), size_(0), cap_(0)
        {
            init_from(other.buffer_, pos, count);
        }

        basic_string(const_pointer str)
                :buffer_(nullptr), size_(0), cap_(0)
        {
            init_from(str, 0, char_traits::length(str));
        }
        basic_string(const_pointer str, size_type count)
                :buffer_(nullptr), size_(0), cap_(0)
        {
            init_from(str, 0, count);
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        basic_string(Iter first, Iter last)
        { copy_init(first, last, iterator_category(first)); }

        basic_string(const basic_string& rhs)
                :buffer_(nullptr), size_(0), cap_(0)
        {
            init_from(rhs.buffer_, 0, rhs.size_);
        }
        basic_string(basic_string&& rhs) noexcept
                :buffer_(rhs.buffer_), size_(rhs.size_), cap_(rhs.cap_)
        {
            rhs.buffer_ = nullptr;
            rhs.size_ = 0;
            rhs.cap_ = 0;
        }

        basic_string& operator=(const basic_string& rhs);
        basic_string& operator=(basic_string&& rhs) noexcept;

        basic_string& operator=(const_pointer str);
        basic_string& operator=(value_type ch)
        {
            if (cap_ < 1)
            {
                auto new_buffer = data_allocator::allocate(2);
                data_allocator::deallocate(buffer_);
                buffer_ = new_buffer;
                cap_ = 2;
            }
            *buffer_ = ch;
            size_ = 1;
            return *this;
        }

        ~basic_string() { destroy_buffer(); }

    public:
        // 迭代器相关操作
        iterator               begin()         noexcept
        { return buffer_; }
        const_iterator         begin()   const noexcept
        { return buffer_; }
        iterator               end()           noexcept
        { return buffer_ + size_; }
        const_iterator         end()     const noexcept
        { return buffer_ + size_; }

        reverse_iterator       rbegin()        noexcept
        { return reverse_iterator(end()); }
        const_reverse_iterator rbegin()  const noexcept
        { return const_reverse_iterator(end()); }
        reverse_iterator       rend()          noexcept
        { return reverse_iterator(begin()); }
        const_reverse_iterator rend()    const noexcept
        { return const_reverse_iterator(begin()); }

        const_iterator         cbegin()  const noexcept
        { return begin(); }
        const_iterator         cend()    const noexcept
        { return end(); }
        const_reverse_iterator crbegin() const noexcept
        { return rbegin(); }
        const_reverse_iterator crend()   const noexcept
        { return rend(); }

        // 容量相关操作
        bool      empty()    const noexcept
        { return size_ == 0; }

        size_type size()     const noexcept
        { return size_; }
        size_type length()   const noexcept
        { return size_; }
        size_type capacity() const noexcept
        { return cap_; }
        size_type max_size() const noexcept
        { return static_cast<size_type>(-1); }

        void      reserve(size_type n);
        void      shrink_to_fit();

        // 访问元素相关操作
        reference       operator[](size_type n)
        {
            MYSTL_DEBUG(n <= size_);
            if (n == size_)
                *(buffer_ + n) = value_type();
            return *(buffer_ + n);
        }
        const_reference operator[](size_type n) const
        {
            MYSTL_DEBUG(n <= size_);
            if (n == size_)
                *(buffer_ + n) = value_type();
            return *(buffer_ + n);
        }

        reference       at(size_type n)
        {
            THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at()"
                                              "subscript out of range");
            return (*this)[n];
        }
        const_reference at(size_type n) const
        {
            THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at()"
                                              "subscript out of range");
            return (*this)[n];
        }

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
            return *(end() - 1);
        }
        const_reference back()  const
        {
            MYSTL_DEBUG(!empty());
            return *(end() - 1);
        }

        const_pointer   data()  const noexcept
        { return to_raw_pointer(); }
        const_pointer   c_str() const noexcept
        { return to_raw_pointer(); }

        // 添加删除相关操作

        // insert
        iterator insert(const_iterator pos, value_type ch);
        iterator insert(const_iterator pos, size_type count, value_type ch);

        template <class Iter>
        iterator insert(const_iterator pos, Iter first, Iter last);


        // push_back / pop_back
        void     push_back(value_type ch)
        { append(1, ch); }
        void     pop_back()
        {
            MYSTL_DEBUG(!empty());
            --size_;
        }

        // append
        basic_string& append(size_type count, value_type ch);

        basic_string& append(const basic_string& str)
        { return append(str, 0, str.size_); }
        basic_string& append(const basic_string& str, size_type pos)
        { return append(str, pos, str.size_ - pos); }
        basic_string& append(const basic_string& str, size_type pos, size_type count);

        basic_string& append(const_pointer s)
        { return append(s, char_traits::length(s)); }
        basic_string& append(const_pointer s, size_type count);

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        basic_string& append(Iter first, Iter last)
        { return append_range(first, last); }

        // erase /clear
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        // resize
        void resize(size_type count)
        { resize(count, value_type()); }
        void resize(size_type count, value_type ch);

        void     clear() noexcept
        { size_ = 0; }

        // basic_string 相关操作

        // compare
        int compare(const basic_string& other) const;
        int compare(size_type pos1, size_type count1, const basic_string& other) const;
        int compare(size_type pos1, size_type count1, const basic_string& other,
                    size_type pos2, size_type count2 = npos) const;
        int compare(const_pointer s) const;
        int compare(size_type pos1, size_type count1, const_pointer s) const;
        int compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const;

        // substr
        basic_string substr(size_type index, size_type count = npos)
        {
            count = mystl::min(count, size_ - index);
            return basic_string(buffer_ + index, buffer_ + index + count);
        }

        // replace
        basic_string& replace(size_type pos, size_type count, const basic_string& str)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos, count, str.buffer_, str.size_);
        }
        basic_string& replace(const_iterator first, const_iterator last, const basic_string& str)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_cstr(first, static_cast<size_type>(last - first), str.buffer_, str.size_);
        }

        basic_string& replace(size_type pos, size_type count, const_pointer str)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos, count, str, char_traits::length(str));
        }
        basic_string& replace(const_iterator first, const_iterator last, const_pointer str)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_cstr(first, static_cast<size_type>(last - first), str, char_traits::length(str));
        }

        basic_string& replace(size_type pos, size_type count, const_pointer str, size_type count2)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos, count, str, count2);
        }
        basic_string& replace(const_iterator first, const_iterator last, const_pointer str, size_type count)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_cstr(first, static_cast<size_type>(last - first), str, count);

        }

        basic_string& replace(size_type pos, size_type count, size_type count2, value_type ch)
        {
            THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace's pos out of range");
            return replace_fill(buffer_ + pos, count, count2, ch);
        }
        basic_string& replace(const_iterator first, const_iterator last, size_type count, value_type ch)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_fill(first, static_cast<size_type>(last - first), count, ch);
        }

        basic_string& replace(size_type pos1, size_type count1, const basic_string& str,
                              size_type pos2, size_type count2 = npos)
        {
            THROW_OUT_OF_RANGE_IF(pos1 > size_ || pos2 > str.size_,
                                  "basic_string<Char, Traits>::replace's pos out of range");
            return replace_cstr(buffer_ + pos1, count1, str.buffer_ + pos2, count2);
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        basic_string& replace(const_iterator first, const_iterator last, Iter first2, Iter last2)
        {
            MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
            return replace_copy(first, last, first2, last2);
        }

        // reverse
        void reverse() noexcept;

        // swap
        void swap(basic_string& rhs) noexcept;

        // 查找相关操作

        // find
        size_type find(value_type ch, size_type pos = 0)                             const noexcept;
        size_type find(const_pointer str, size_type pos = 0)                         const noexcept;
        size_type find(const_pointer str, size_type pos, size_type count)            const noexcept;
        size_type find(const basic_string& str, size_type pos = 0)                   const noexcept;

        // rfind
        size_type rfind(value_type ch, size_type pos = npos)                         const noexcept;
        size_type rfind(const_pointer str, size_type pos = npos)                     const noexcept;
        size_type rfind(const_pointer str, size_type pos, size_type count)           const noexcept;
        size_type rfind(const basic_string& str, size_type pos = npos)               const noexcept;

        // find_first_of
        size_type find_first_of(value_type ch, size_type pos = 0)                    const noexcept;
        size_type find_first_of(const_pointer s, size_type pos = 0)                  const noexcept;
        size_type find_first_of(const_pointer s, size_type pos, size_type count)     const noexcept;
        size_type find_first_of(const basic_string& str, size_type pos = 0)          const noexcept;

        // find_first_not_of
        size_type find_first_not_of(value_type ch, size_type pos = 0)                const noexcept;
        size_type find_first_not_of(const_pointer s, size_type pos = 0)              const noexcept;
        size_type find_first_not_of(const_pointer s, size_type pos, size_type count) const noexcept;
        size_type find_first_not_of(const basic_string& str, size_type pos = 0)      const noexcept;

        // find_last_of
        size_type find_last_of(value_type ch, size_type pos = 0)                     const noexcept;
        size_type find_last_of(const_pointer s, size_type pos = 0)                   const noexcept;
        size_type find_last_of(const_pointer s, size_type pos, size_type count)      const noexcept;
        size_type find_last_of(const basic_string& str, size_type pos = 0)           const noexcept;

        // find_last_not_of
        size_type find_last_not_of(value_type ch, size_type pos = 0)                 const noexcept;
        size_type find_last_not_of(const_pointer s, size_type pos = 0)               const noexcept;
        size_type find_last_not_of(const_pointer s, size_type pos, size_type count)  const noexcept;
        size_type find_last_not_of(const basic_string& str, size_type pos = 0)       const noexcept;

        // count
        size_type count(value_type ch, size_type pos = 0) const noexcept;

    public:
        // 重载 operator+=
        basic_string& operator+=(const basic_string& str)
        { return append(str); }
        basic_string& operator+=(value_type ch)
        { return append(1, ch); }
        basic_string& operator+=(const_pointer str)
        { return append(str, str + char_traits::length(str)); }

        // 重载 operator >> / operatror <<

        friend std::istream& operator >> (std::istream& is, basic_string& str)
        {
            value_type* buf = new value_type[4096];
            is >> buf;
            basic_string tmp(buf);
            str = std::move(tmp);
            delete[]buf;
            return is;
        }

        friend std::ostream& operator << (std::ostream& os, const basic_string& str)
        {
            for (size_type i = 0; i < str.size_; ++i)
                os << *(str.buffer_ + i);
            return os;
        }

    private:
        // helper functions

        // init / destroy
        void          try_init() noexcept;

        void          fill_init(size_type n, value_type ch);

        template <class Iter>
        void          copy_init(Iter first, Iter last, mystl::input_iterator_tag);
        template <class Iter>
        void          copy_init(Iter first, Iter last, mystl::forward_iterator_tag);

        void          init_from(const_pointer src, size_type pos, size_type n);

        void          destroy_buffer();

        // get raw pointer
        const_pointer to_raw_pointer() const;

        // shrink_to_fit
        void          reinsert(size_type size);

        // append
        template <class Iter>
        basic_string& append_range(Iter first, Iter last);

        // compare
        int compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const;

        // replace
        basic_string& replace_cstr(const_iterator first, size_type count1, const_pointer str, size_type count2);
        basic_string& replace_fill(const_iterator first, size_type count1, size_type count2, value_type ch);
        template <class Iter>
        basic_string& replace_copy(const_iterator first, const_iterator last, Iter first2, Iter last2);

        // reallocate
        void          reallocate(size_type need);
        iterator      reallocate_and_fill(iterator pos, size_type n, value_type ch);
        iterator      reallocate_and_copy(iterator pos, const_iterator first, const_iterator last);
    };

    /*****************************************************************************************/

    // 复制赋值操作符
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>&
    basic_string<CharType, CharTraits>::
    operator=(const basic_string& rhs)
    {
        if (this != &rhs)
        {
            basic_string tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值操作符
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>&
    basic_string<CharType, CharTraits>::
    operator=(basic_string&& rhs) noexcept
    {
        destroy_buffer();
        buffer_ = rhs.buffer_;
        size_ = rhs.size_;
        cap_ = rhs.cap_;
        rhs.buffer_ = nullptr;
        rhs.size_ = 0;
        rhs.cap_ = 0;
        return *this;
    }

    // 用一个字符串赋值
    template <class CharType, class CharTraits>
    basic_string<CharType, CharTraits>&
    basic_string<CharType, CharTraits>::
    operator=(const_pointer str)
    {
        const size_type len = char_traits::length(str);
        if (cap_ < len)
        {
            auto new_buffer = data_allocator::allocate(len + 1);
            data_allocator::deallocate(buffer_);
            buffer_ = new_buffer;
            cap_ = len + 1;
        }
        char_traits::copy(buffer_, str, len);
        size_ = len;
        return *this;
    }

    // 预留储存空间
    template<class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::reserve(basic_string::size_type n)
    {
        if (cap_ < n)
        {
            THROW_LENGTH_ERROR_IF(n > max_size(), "n can not larger than max_size()"
                                                  "in basic_string<Char,Traits>::reserve(n)");
            auto new_buffer = data_allocator::allocate(n);
            char_traits::move(new_buffer, buffer_, size_);
            buffer_ = new_buffer;
            cap_ = n;
        }
    }

    // 减少不用的空间
    template<class CharType, class CharTraits>
    void basic_string<CharType, CharTraits>::shrink_to_fit()
    {
        if (size_ != cap_)
        {
            reinsert(size_);
        }
    }

    // 在 pos 处插入一个元素
    template<class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator
    basic_string<CharType, CharTraits>::insert(basic_string::const_iterator pos, value_type ch)
    {
        iterator r = const_cast<iterator>(pos);
        if (size_ == cap_)
        {
            return reallocate_and_fill(r, 1, ch);
        }
        char_traits::move(r + 1, r, end() - r);
        ++size_;
        *r = ch;
        return r;
    }

    // 在 pos 处插入 n 个元素
    template<class CharType, class CharTraits>
    typename basic_string<CharType, CharTraits>::iterator
    basic_string<CharType, CharTraits>::
    insert(basic_string::const_iterator pos, basic_string::size_type count, value_type ch)
    {
        iterator r = const_cast<iterator>(pos);
        if (count == 0)
            return r;
        if (cap_ - size_ < count)
        {
            return reallocate_and_fill(r, count, ch);
        }
        if (pos == end())
        {
            char_traits::fill(end(), ch, count);
            size_ += count;
            return r;
        }
        char_traits::move(r + count, r , count);
        char_traits::fill(r, ch, count);
        size_ += count;
        return r;
    }

    // 在 pos 处插入 [first, last) 内的元素
    template<class CharType, class CharTraits>
    template<class Iter>
    typename basic_string<CharType, CharTraits>::iterator
    basic_string<CharType, CharTraits>::insert(basic_string::const_iterator pos, Iter first, Iter last)
    {
        iterator r = const_cast<iterator>(pos);
        const size_type len = mystl::distance(first, last);
        if (len == 0)
            return r;
        if (cap_ - size_ < len)
        {
            return reallocate_and_copy(r, first, last);
        }
        if (pos == end())
        {
            mystl::uninitialized_copy(first, last, end());
            size_ += len;
            return r;
        }
        char_traits::move(r + len, r, len);
        mystl::uninitialized_copy(first, last, r);
        size_ += len;
        return r;
    }

} // namespace mystl

#endif //MYTINYSTL_BASIC_STRING_H
