#ifndef UTILS_HPP
# define UTILS_HPP
//iterators_traits, reverse_iterator, enable_if, is_integral, equal/lexicographical compare, std::pair, std::make_pair, must be reimplemented.

namespace ft{

template<
    class T1,
    class T2
> struct pair {
	
	public:

	typedef T1 first_type;
	typedef T2 second_type;

	first_type first;
	second_type second;

	pair(): first(), second() {}

	pair(
		const first_type x,
		const second_type y):
		first(x),
		second(y)
	{}

	template< class U1, class U2 >
	pair(const pair<U1, U2>& p):
		first(p.first),
		second(p.second)
	{}

	pair& operator=(const pair& other) {
		this->first = other.first;
		this->second = other.second;

		return *this;
	}
};

//non-member functions
template< class T1, class T2 >
ft::pair<T1, T2> make_pair(T1 t, T2 u) {
	return ft::pair<T1, T2>(t, u);
}

template< class T1, class T2 >
bool operator==( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ) { return (lhs.first == rhs.first && lhs.second == rhs.second); }

template< class T1, class T2 >
bool operator!=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ) { return !(lhs == rhs); }

template< class T1, class T2 >
bool operator<( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ) {
	if (lhs.first < rhs.first)
		return true;
	if (rhs.first < lhs.first)
		return false;
	if (lhs.second < rhs.second)
		return true;
	return false;
}

template< class T1, class T2 >
bool operator<=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ) { return !(rhs < lhs); }

template< class T1, class T2 >
bool operator>( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ) { return rhs < lhs; }

template< class T1, class T2 >
bool operator>=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ) { return !(lhs < rhs); }

template <bool, typename T = void> struct enable_if {};
template <typename T> struct enable_if<true, T> { typedef T type; };

template <typename T> struct is_integral { const static bool value = false; };
template <> struct is_integral<bool> { const static bool value = true; };
template <> struct is_integral<char16_t> { const static bool value = true; };
template <> struct is_integral<char32_t> { const static bool value = true; };
template <> struct is_integral<wchar_t> { const static bool value = true; };
template <> struct is_integral<short> { const static bool value = true; };
template <> struct is_integral<int> { const static bool value = true; };
template <> struct is_integral<long> { const static bool value = true; };
template <> struct is_integral<long long> { const static bool value = true; };

//bool, char, char8_t (since C++20), char16_t, char32_t, wchar_t, short, int, long, long long

//is_integral<_Tp>::value
// Compare for equality of types.
struct __false_type {};
struct __true_type {};

  template<typename, typename>
    struct __are_same
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<typename _Tp>
    struct __are_same<_Tp, _Tp>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };


template <class T>
void swap (T & a, T & b)
{
	T tmp(a);
	a = b;
	b = tmp;
}

}

#endif