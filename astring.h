//
// Created by gy gao on 8/11/22.
//

#ifndef MYOWNSTL_ASTRING_H
#define MYOWNSTL_ASTRING_H

// 定义 string, wstring, u16string, u32string 类型

#include "basic_string.h"

namespace myownstl
{
    using string    = myownstl::basic_string<char>;
    using wstring   = myownstl::basic_string<wchar_t>;
    using u16string = myownstl::basic_string<char16_t>;
    using u32string = myownstl::basic_string<char32_t>;
}

#endif //MYOWNSTL_ASTRING_H
