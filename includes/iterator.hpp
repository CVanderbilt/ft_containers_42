#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include "utils.hpp"

namespace ft
{
	
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag {};
struct bidirectional_iterator_tag {};
struct random_access_iterator_tag {};

//The template iterator_traits<Iterator> is defined as
template<class Iterator> struct iterator_traits
{
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::iterator_category iterator_category;
};
//It is specialized for pointers as
template<class T> struct iterator_traits<T*>
{
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef random_access_iterator_tag iterator_category;
};
//and for pointers to const as
template<class T> struct iterator_traits<const T*>
{
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef const T* pointer;
	typedef const T& reference;
	typedef random_access_iterator_tag iterator_category;
};
/*
	[Note: If there is an additional pointer type _ _far such that the difference of two _ _far is of type
	long, an implementation may define
	template<class T> struct iterator_traits<T _ _far*>
	{
		typedef long difference_type;
		typedef T value_type;
		typedef T _ _far* pointer;
		typedef T _ _far& reference;
		typedef random_access_iterator_tag iterator_category;
	};
*/
	//base iterator class
template<
	class Category,
	class T,
	class Distance = ptrdiff_t,
	class Pointer = T*,
	class Reference = T&>
struct iterator
{
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
	typedef Category iterator_category;
};

template <class Iterator>
class reverse_iterator
: public iterator<typename iterator_traits<Iterator>::iterator_category,
					typename iterator_traits<Iterator>::value_type,
					typename iterator_traits<Iterator>::difference_type,
					typename iterator_traits<Iterator>::pointer,
					typename iterator_traits<Iterator>::reference>
{
protected:
    Iterator current;
public:
    typedef Iterator                                            iterator_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::reference       reference;
    typedef typename iterator_traits<Iterator>::pointer         pointer;

    reverse_iterator(): current() {}
    explicit reverse_iterator(Iterator x): current(x) {}
    template <class U> reverse_iterator(const reverse_iterator<U>& u): current(u.base()) {}

    template <class U> reverse_iterator& operator=(const reverse_iterator<U>& u) { current = u.base(); return *this; }

    Iterator base() const { return current; }

    reference operator*() const { Iterator tmp = current; return *--tmp; }
    pointer   operator->() const { return &(operator*()); }
    reverse_iterator& operator++() { --current; return *this; }
    reverse_iterator  operator++(int) { reverse_iterator tmp(*this); --current; return tmp; }
    reverse_iterator& operator--() { ++current; return *this; }
    reverse_iterator  operator--(int) { reverse_iterator tmp(*this); ++current; return tmp; }
    reverse_iterator  operator+ (difference_type n) const { return reverse_iterator(current - n); }
    reverse_iterator& operator+=(difference_type n) { current -= n; return *this; }
    reverse_iterator  operator- (difference_type n) const { return reverse_iterator(current + n); }
    reverse_iterator& operator-=(difference_type n) { current -= n; return *this; }
    reference         operator[](difference_type n) const { return *(*this + n); }
};


template <class I1, class I2>
bool operator==(const reverse_iterator<I1>& x, const reverse_iterator<I2>& y) { return x.base() == y.base(); }
template <class I1, class I2>
bool operator<(const reverse_iterator<I1>& x, const reverse_iterator<I2>& y) { return x.base() > y.base(); }
template <class I1, class I2>
bool operator!=(const reverse_iterator<I1>& x, const reverse_iterator<I2>& y) { return x.base() != y.base(); }
template <class I1, class I2>
bool operator>(const reverse_iterator<I1>& x, const reverse_iterator<I2>& y) { return x.base() < y.base(); }
template <class I1, class I2>
bool operator>=(const reverse_iterator<I1>& x, const reverse_iterator<I2>& y) { return x.base() <= y.base(); }
template <class I1, class I2>
bool operator<=(const reverse_iterator<I1>& x, const reverse_iterator<I2>& y) { return x.base() >= y.base(); }

template< class It >
reverse_iterator<It>
operator+( typename reverse_iterator<It>::difference_type n,
const reverse_iterator<It>& it ) {
	return reverse_iterator<It>(it.base() - n);
}

template< class It >
typename reverse_iterator<It>::difference_type
operator-( const reverse_iterator<It>& lhs,
const reverse_iterator<It>& rhs ) {
	return rhs.base() - lhs.base();
}


template<typename _Iterator, typename _Container>
    class __normal_iterator
    {
    protected:
      _Iterator _M_current;

      typedef iterator_traits<_Iterator>		__traits_type;

    public:
      typedef _Iterator									iterator_type;
      typedef typename __traits_type::iterator_category	iterator_category;
      typedef typename __traits_type::value_type		value_type;
      typedef typename __traits_type::difference_type	difference_type;
      typedef typename __traits_type::reference			reference;
      typedef typename __traits_type::pointer			pointer;

    __normal_iterator()
      : _M_current(_Iterator()) { }

      explicit
      __normal_iterator(const _Iterator& __i)
      : _M_current(__i) { }

      // Allow iterator to const_iterator conversion
      template<typename _Iter>
        __normal_iterator(const __normal_iterator<_Iter,
			  typename enable_if<
      	       (__are_same<_Iter, typename _Container::pointer>::__value),
		      _Container>::__type>& __i)
        : _M_current(__i.base()) { }

      // Forward iterator requirements
      reference
      operator*() const
      { return *_M_current; }

      pointer
      operator->() const
      { return _M_current; }

      __normal_iterator&
      operator++()
      {
	++_M_current;
	return *this;
      }

      __normal_iterator
      operator++(int)
      { return __normal_iterator(_M_current++); }

      // Bidirectional iterator requirements
      __normal_iterator&
      operator--()
      {
	--_M_current;
	return *this;
      }

      __normal_iterator
      operator--(int)
      { return __normal_iterator(_M_current--); }

      // Random access iterator requirements
      reference
      operator[](difference_type __n) const 
      { return _M_current[__n]; }

      __normal_iterator&
      operator+=(difference_type __n) 
      { _M_current += __n; return *this; }

      __normal_iterator
      operator+(difference_type __n) const 
      { return __normal_iterator(_M_current + __n); }

      __normal_iterator&
      operator-=(difference_type __n) 
      { _M_current -= __n; return *this; }

      __normal_iterator
      operator-(difference_type __n) const 
      { return __normal_iterator(_M_current - __n); }

      const _Iterator&
      base() const 
      { return _M_current; }
    };

	// Forward iterator requirements
	template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
	       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() == __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator==(const __normal_iterator<_Iterator, _Container>& __lhs,
	       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() == __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
	       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() != __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
	       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() != __rhs.base(); }

  // Random access iterator requirements
  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
	      const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() < __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
	      const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() < __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
	      const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() > __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator>(const __normal_iterator<_Iterator, _Container>& __lhs,
	      const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() > __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
	       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() <= __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator<=(const __normal_iterator<_Iterator, _Container>& __lhs,
	       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() <= __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
	       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() >= __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator>=(const __normal_iterator<_Iterator, _Container>& __lhs,
	       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() >= __rhs.base(); }

}

#endif