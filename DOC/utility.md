# utility

---

## move
std::move 用于指示对象 t 可以“被移动”，即允许从 t 到另一对象的有效率的资源传递。
特别是， std::move 生成标识其参数 t 的亡值表达式。它准确地等价于到右值引用类型的 static_cast 。
用以避免非必要copy和临时对象 (temporary)
```c
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept
```

## forward
```c
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    // 禁止右值转发为左值，否则编译时报错"bad forward"
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}
```
1) 转发左值为左值或右值，依赖于T
   当 t 是转发引用（作为到无 cv 限定函数模板形参的右值引用的函数实参），此重载将参数以在传递给调用方函数时的值类别转发给另一个函数。
例如，若用于如下的包装器，则模板表现为下方所描述：

```c
template<class T>
void wrapper(T&& arg)
{
    // arg 始终是左值
    foo(std::forward<T>(arg)); // 转发为左值或右值，依赖于 T
}
```

- 若对 wrapper() 的调用传递右值 std::string ，则推导 T 为 std::string，且 std::forward 确保将右值引用传递给 foo。
- 若对 wrapper() 的调用传递 const 左值 std::string，则推导 T 为 const std::string&，且 std::forward 确保将 const 左值引用传递给 foo。
- 若对 wrapper() 的调用传递非 const 左值 std::string，则推导 T 为 std::string&，且 std::forward 确保将非 const 左值引用传递给 foo。
2) 转发右值为右值并禁止右值的转发为左值
   此重载令转发表达式（如函数调用）的结果可行，结果可以是右值或左值，同转发引用参数的原始值类别。

## 辅助函数swap
swap()的最大优势在于，通过template specialization (模板特化)
或function overloading (函数重载)，我们可以为更复杂的类型提供特殊实现版本，
这些特殊实现有可能交换内部成员，而非对对象赋值。
```c
// 在其内部，数值被moved或被move assigned。
// 在 C++11之前，数值是被assigned或copied.
template <class Tp>
void swap(Tp& lhs, Tp& rhs)


template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
  for (; first1 != last1; ++first1, (void) ++first2)
    myownstl::swap(*first1, *first2);
  return first2;
}

// 针对array的重载版本
template <class Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N])
{
  myownstl::swap_range(a, a + N, b);
}
```

## 结构体模板Pair

---

> Class pair 可将两个value 视为一个单元。C++ 标准库内多处用到了这个class。 尤其容器
> map、multimap、unordered_map和unordered_multinap就是使用pair来管理其以
> key/value pair 形式存在的元素。任何的数如果需返回两个value, 也需用到 pair。
> 原则上你可以对pair<>执行create、copy/assign/swap 及compare 操作。
> 此外它还提供first_type 和second_type类型定义式，用来表示第一value和第二value的类型。

```c++
template <class Ty1, class Ty2>
struct pair
{
  typedef Ty1    first_type;
  typedef Ty2    second_type;

  first_type first;    // 保存第一个数据
  second_type second;  // 保存第二个数据
  ...
};
```

| 成员函数      | 描述 
| :---        |    :----:   | 
| pair<T1, T2> p     | 默认构造函数，建立一个pair,其元素类型分别为T1和T2，各自以其默认构造函数初始化 
| pair<T1, T2> p(val1, val2)  | 建立一个pair，元素类型分别为T1和T2、以val1 和 val2 为初值  
| pair<T1, T2> p(rv1, rv2)  | 建立一个pair，元素类型分别为T1和T2、以rv1 和 rv2 进行搬移式初始化(move initialized)
| pair<T1, T2> p(p2)    | copy构造函数，建立p成为p2的拷贝
| pair<T1, T2> p(rv)     | move构造函数，将rv的内容移至p（允许隐式类型转换）
| p = p2     | 将 p2 赋值给p
| p = rv     | 将rv 的值move assign给p(始自C++11，允许隐式类型转换)
| p1.swap(p2)     | 互换p1和p2的数据（成员函数）

| 非成员函数      | 描述
| :---        |    :----:   |
| p1 == p2     | 返回"是否 p1 等于 p2"
| p1 != p2     | 返回"是否 p1 不等于 p2"
| p1 < p2     | 返回"是否 p1 小于 p2"（比较first，如果相等则比较second）
| p1 > p2     | 返回"是否 p1 大于 p2"
| p1 <= p2     | 返回"是否 p1 小于等于 p2"
| p1 >= p2     | 返回"是否 p1 大于等于 p2"
| swap(p1, p2)     | 全局函数
| make_pair(val1, val2)     | 返回一个pair，带有val1和val2的类型和数值











