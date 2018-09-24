#pragma once
#include <type_traits>

//--------------------------------------------------------

class Shape {};
class Circle : public Shape {};

template <typename T>
using IsShape = typename std::enable_if<std::is_base_of<Shape, T>::value>::type;

template <typename T, typename = IsShape<T>>
void munge_shape(T const &a);

//--------------------------------------------------------

template <typename S, typename T>
using similar = std::is_same<std::decay_t<S>, std::decay_t<T>>; // decay will strip everything to a value

template <typename S, typename T>
using NotSimilar = std::enable_if<!similar<S, T>::value>;

//--------------------------------------------------------
template <typename T>
class sometype_traits
{
public:
	static const bool is_specialized = false;
	///...
};

template <>
class sometype_traits<Shape>
{
public:
	// Now we have a specialization for Shape.
	// It does exists.
	static const bool is_specialized = true;
	static int min() throw() {
		return 0;
	}
	static int max() throw() {
		return 2147483647;
	}
	static const int digits = 31;
};

static bool is_int_spec = sometype_traits<int>::is_specialized; // false
static bool is_Shape_spec = sometype_traits<Shape>::is_specialized; // true

//--------------------------------------------------------
// Problem:
template<typename T>
class MyClass
{
public:
	void f(T const& x);
	void f(T&& x); // Leads to error when MyClass<int&> x; int i = 0; x.f(i);
};

//--------------------------------------------------------
// Fix 1: tag dispatching
// https://crazycpp.wordpress.com/2014/12/15/tutorial-on-tag-dispatching/
namespace detail 
{ 
	struct tag_3 {};
	struct tag_2 : tag_3 {};
	struct tag_1 : tag_2 {};

	template<bool>
	struct tag_1_with_sth : tag_2 {};

	static tag_1 selector;

	// Even when this is not necessary though it can be beneficial to do provide a metafunction or traits template to retrieve the tag.
	template < typename T >
	struct mytype_traits
	{
		using category = tag_2;
		// much more stuff...
	};

	template < typename T >
	struct mytype_traits<T*>
	{
		using category = tag_1_with_sth<false>;
		// ... other useful stuff...
	};

	template<typename T>
	void f(T x, tag_1) {}

	//template<typename T> // can be commented out and we should fallback to tag_3 overload
	//void f(T x, tag_2) {}

	template<typename T>
	void f(T x, tag_3) {}

	template<typename T>
	void f(T x, tag_1_with_sth<true>) {}

	template<typename T>
	void f(T x, tag_1_with_sth<false>) {}
}

template<typename T>
void f(T x) {
	// Tag can be taken fron any place, like:
	using tag = typename detail::mytype_traits<T>::category; 
	detail::f(x, tag); // ...traits template
	detail::f(x, typename T::get_tag()); // ...metafunction

	//detail::f(x, detail::selector);
}

struct SomeType {
	uint64_t id;
	static constexpr auto get_tag() { return detail::tag_2{}; }
};

//--------------------------------------------------------
// Fix 2: SFINAE option 1
// point of SFINAE is to deactivate a piece of template code for some types
// https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html
template<typename T>
class MyFixed2Class
{
public:
	template<typename T>
	void f(T x, typename T::type);

	template<typename T>
	void f(T x, typename T::other_type);
};

//--------------------------------------------------------
// Fix 2: SFINAE option 2 with enable_if & expression SFINAE
// One of the main uses of SFINAE can be found in enable_if expressions.
template<typename T>
class MyFixed3Class
{
public:
	void f(T const& x);
	void f(T&& x, typename std::enable_if<std::is_reference<T>::value /*, ...*/>::type);
	auto f(T&& x) -> decltype(x + x); // expression of x+x needs to be checked
};

// NOTE: SFINAE will be obsolete once "if constexpr" gets shipped with C++17
