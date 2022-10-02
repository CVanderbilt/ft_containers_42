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

template <bool, typename T = void>
struct enable_if
{};

template <typename T>
struct enable_if<true, T> {
  typedef T type;
};

}

#endif