**前言**

本文主要给大家介绍了关于C++17转发一个函数调用的相关内容，分享出来供大家参考学习，下面话不多说了，来一起看看详细的介绍吧。

**方法如下**

首先你灵光一闪：

```
#define WARP_CALL(fun, ...) fun(__VA_ARGS__)
```

不我们并不喜欢宏，扩展性太差了

```
template``<``class` `R, ``class` `T1, ``class` `T2, ``class` `T3>``R warp_call(R(*fun)(T1, T2, T3), T1 a, T2 b, T3 c)``{`` ``return` `fun(a, b, c);``}
```

如果你写出来上面这段代码，你肯定是从C转过来的，C++还没用熟。考虑callable object和C++11 variadic template特性用上：

```
template``<``class` `Fun, ``class``... Args>``auto wrap_call(Fun f, Args... args) -> decltype(f(args...))``{`` ``return` `f(args...);``}
```

加上移动语义，返回值推导：

```
template``<``class` `Fun, ``class``... Args>``auto wrap_call(Fun&& f, Args&&... args)``{`` ``return` `std::forward<Fun>(f)(std::forward<Args>(args)...);``}
```

auto返回值实际上会有参数被decay的问题，用decltype + 尾置返回值

```
template``<``class` `Fun, ``class``... Args>``auto wrap_call(Fun&& f, Args&&... args)`` ``-> decltype(std::forward<Fun>(f)(std::forward<Args>(args)...))``{`` ``return` `std::forward<Fun>(f)(std::forward<Args>(args)...);``}
```

有了C++14，可以直接使用decltype(auto)

```
template``<``class` `Fun, ``class``... Args>``decltype(auto) wrap_call(Fun&& f, Args&&... args)``{`` ``return` `std::forward<Fun>(f)(std::forward<Args>(args)...);``}
```

别忘了noexcept

```
template``<``class` `Fun, ``class``... Args>``decltype(auto) wrap_call(Fun&& f, Args&&... args)`` ``noexcept(noexcept(std::forward<Fun>(f)(std::forward<Args>(args)...)))``{`` ``return` `std::forward<Fun>(f)(std::forward<Args>(args)...);``}
```

但是上面的函数不是SFINAE-friendly的，因为decltype(auto)返回值的函数并不能直接从函数签名获得返回值，而对这个函数进行返回值推导，是可能产生hard error打断SFINAE的。所以最好手动写返回值

```
template``<``class` `Fun, ``class``... Args>``auto wrap_call(Fun&& f, Args&&... args)`` ``noexcept(noexcept(std::forward<Fun>(f)(std::forward<Args>(args)...)))`` ``-> decltype(std::forward<Fun>(f)(std::forward<Args>(args)...))``{`` ``return` `std::forward<Fun>(f)(std::forward<Args>(args)...);``}
```

我们还遗漏了啥？constexpr

```
template``<``class` `Fun, ``class``... Args>``constexpr auto wrap_call(Fun&& f, Args&&... args)`` ``noexcept(noexcept(std::forward<Fun>(f)(std::forward<Args>(args)...)))`` ``-> decltype(std::forward<Fun>(f)(std::forward<Args>(args)...))``{`` ``return` `std::forward<Fun>(f)(std::forward<Args>(args)...);``}
```

上面是完美的

再来看看std::invoke:

```C++
#define _IMPLEMENT_INVOKE(NAME_PREFIX, CONSTEXPR) \
	/* FUNCTION TEMPLATE invoke */ \
struct _CONCAT(NAME_PREFIX, _Invoker_pmf_object) \
	{	/* INVOKE a pointer to member function on an object */ \
	template<class _Decayed, \
		class _Ty1, \
		class... _Types2> \
		static CONSTEXPR auto _Call(_Decayed _Pmf, _Ty1&& _Arg1, _Types2&&... _Args2) \
		_NOEXCEPT_COND(_NOEXCEPT_OPER((_STD forward<_Ty1>(_Arg1).*_Pmf)(_STD forward<_Types2>(_Args2)...))) \
		-> decltype((_STD forward<_Ty1>(_Arg1).*_Pmf)(_STD forward<_Types2>(_Args2)...)) \
		{	/* INVOKE a pointer to member function on an object */ \
		return ((_STD forward<_Ty1>(_Arg1).*_Pmf)(_STD forward<_Types2>(_Args2)...)); \
		} \
	}; \
 \
```

PS：转发自知乎一篇已经被删掉的文章，地址不可考