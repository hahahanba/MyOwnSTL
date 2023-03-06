//
// Created by gy gao on 8/11/22.
//

#ifndef MYOWNSTL_TYPE_TRAITS_H
#define MYOWNSTL_TYPE_TRAITS_H

// 用于提取类型信息, 使用std的type_traits

// use std header for type_traits
#include <type_traits>

namespace myownstl
{
    // type_traits Helper classes
    // 1. integral_constant
    // 2. true_type
    // 3. false_type
    // template <typename type> 函数模板
    // template <class type>    类模板
    template <class T, T v>
    struct m_integral_constant
    {
        // 简单说const其实是readonly，constexpr才是const
        static constexpr T value = v;
    };

    template<bool b>
    using m_bool_constant = m_integral_constant<bool, b>;

    typedef m_bool_constant<true>  m_true_type;
    typedef m_bool_constant<false> m_false_type;

    /*********************************************************/
    // type traits
    // is_pair (自定义type traits stl中没有该类型)

    // ---forward declaration begin
    template<class T1, class T2>
    struct pair;
    // ---forward declaration end

    template<class T>
    struct is_pair : myownstl::m_false_type {};

    template <class T1, class T2>
    struct is_pair<myownstl::pair<T1, T2>> : myownstl::m_true_type {};

}

#endif //MYOWNSTL_TYPE_TRAITS_H
