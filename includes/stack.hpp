#ifndef STACK_HPP
# define STACK_HPP

namespace ft {

template<typename _Tp, typename _Sequence = vector<_Tp> >
class stack {
	template<typename _Tp1, typename _Seq1>
	friend bool
	operator==(const stack<_Tp1, _Seq1>&, const stack<_Tp1, _Seq1>&);

	template<typename _Tp1, typename _Seq1>
	friend bool
	operator<(const stack<_Tp1, _Seq1>&, const stack<_Tp1, _Seq1>&);
public:
	typedef typename _Sequence::value_type		value_type;
	typedef typename _Sequence::reference		reference;
	typedef typename _Sequence::const_reference	const_reference;
	typedef typename _Sequence::size_type		size_type;
	typedef _Sequence							container_type;

protected:
	container_type c;

public:

	/*template<typename _Seq = _Sequence, typename _Requires = typename
		enable_if<std::is_default_constructible<_Seq>::value>::type>*/
	stack(): c() {}
	explicit stack(const container_type& cont = container_type()): c(container_type) {}
	stack(const stack& other): c(other.c) {}

	stack& operator=(const stack& other) { c = other.c; return *this; }

	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }

	void push(const value_type& v) { c.push_back(v); }
	void pop() { c.pop_back(); }
};

template<typename _Tp, typename _Seq>
inline bool
operator==(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return __x.c == __y.c; }

template<typename _Tp, typename _Seq>
inline bool
operator<(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return __x.c < __y.c; }

template<typename _Tp, typename _Seq>
inline bool
operator<(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return __x.c < __y.c; }

/// Based on operator==
template<typename _Tp, typename _Seq>
inline bool
operator!=(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return !(__x == __y); }

/// Based on operator<
template<typename _Tp, typename _Seq>
inline bool
operator>(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return __y < __x; }

/// Based on operator<
template<typename _Tp, typename _Seq>
inline bool
operator<=(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return !(__y < __x); }

/// Based on operator<
template<typename _Tp, typename _Seq>
inline bool
operator>=(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y)
{ return !(__x < __y); }

}

#endif