# Type Traits

C++标准库中几乎每样东西都以template为根基。为了更多地支持template编程，标准库提供了template通用工具用来协助应用程序开发人员和程序库作者。

Type trait，由TR1引入而在C++11中大幅扩展的一项机制，定义出因type而异的行为。它们可被用来针对type优化代码，以便提供特别能力。

### Type Traits的目的
所谓type trait，提供一种用来处理type属性的办法。它是个template， 可在编译期根据一或多个template实参(通常也是type)产出一个type 或 value。比如，对于一个foo()函数需要考虑支持两种type的重载版本，一种是提供针对寻常type的，另一种针对pointer type的。其重载的版本可以这样写：
```c
template <typename T>
void foo (const T& val); // general implementation

template <typename T>
void foo<T*> (const T& val); // parrial specialization for pointers
```

而基于type trait的版本可以这么写：
```c
// foo() implementation for pointer types:
template <typename T>
void foo_impl (const T& val, std::true_type) 
{
    std::cout << "foo() called for pointer to " << *val << std::endl;
}

// foo() implementation for non-pointer types:
template <typename T>
void foo_impl (const T& val, std::false_type)
{
    std::cout << "foo() called for value to " << val << std::end;
}

template <typename T> 
void foo (const T& val)
{
    foo_imp1 (val, std::is_pointer<T>());
}
```

## 细究Type Traits
#### (单参)类型判断式((Unary)Type Predicate)

如果某个特定性质适用，则type predicate 产生出std::true_type, 否则产出std::false_type。这些类型都是辅助用的std::integral_constant 的特化，所以它们相应的value成员将会产出true或false :
```c
namespace std{
    template <typename T, T val> 
    struct integral_constant {
        static constexpr T value = val; 
        typedef T value_type;
        typedef integral_constant<T,v> type; 
        constexpr operator value_type () {
            return value; 
        }
    };
    typedef integral_constant<bool, trues true_type;
    typedef integral_constant<bool, false> false_type; 
}
```
```c
is_pointer<T> // Point类型(包括function point但不包括pointer to nonstatic member)
is_default_constructible<T> // Class能够完成default construction
is_copy_constructible<T>    // Class能够完成copy construction
is_move_constructible<T>    // Class能够完成move construction
is_copy_assignable<T>    // Class能够完成copy assignment
is_move_assignable<T>    // Class能够完成move assignment
is_destructible<T>       // Class带有可被调用的析构函数(不可以是deleted、protected或private)
```
#### 用以检验类型关系 (Type Relation)的Trait
此类type trait可以检查类型之间的关系，包括检查class type提供了哪一种构造函数和哪一种赋值操作
```c
is_convertible<T, T2>       // 类型T可转换至类型T2
is_constructible<T, Args>   // 可运用类型Args初始化T
is_assignable<T, T2>        // 可将类型T2赋值给类型T
```
#### 类型修饰符 (Type Modifier)
此类Traits允许你改动类型。所有这一类Trait为类型添加一个属性，前提是该属性尚末存在，或移除一个属性，前提是该属性已经存在。
```c
remove_cv<T>          // 对应之不带const和volatile的类型
remove_reference<T>   // 对应之nonreference类型
```
#### 其他
它们用来查询特殊属性、检查类型关系或提供更复杂的类型变换。
```c
enable_if<B, T=void>  // 只有当bool B为true时会产出type T
conditional<B, T, F>   // 当bool B为true时会产出type T, 否则产出type F
common_type<T1, ...>   // 所有被传入的类型的共通类型(common type)
```