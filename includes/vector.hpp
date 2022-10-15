#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"
# include "utils.hpp"

namespace ft {

template <class T, class Allocator = std::allocator<T> >
class vector
{
public:
    typedef T																value_type;
    typedef Allocator														allocator_type;
    typedef typename allocator_type::reference								reference;
    typedef typename allocator_type::const_reference						const_reference;
    typedef typename allocator_type::size_type								size_type;
    typedef typename allocator_type::difference_type						difference_type;
    typedef typename allocator_type::pointer								pointer;
    typedef typename allocator_type::const_pointer							const_pointer;
private:
	pointer			_arr;
	size_type		_size;
	size_type		_cap;
	allocator_type	_alloc;
	const float		_rf;

	template <bool Const = false>
	class Iterator:
	public ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type > {
	public:
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type>::value_type	value_type;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::iterator_category									iterator_category;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::difference_type										difference_type;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::pointer												pointer;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::reference											reference;

		pointer _M_current;

		Iterator(): _M_current(NULL) {}
		Iterator(pointer m): _M_current(m) {}
		Iterator(const Iterator<Const>& x): _M_current(x._M_current) {}
		
		template <bool B>
		Iterator(const Iterator<B>& x, typename ft::enable_if<!B>::type* = 0): _M_current(x._M_current) {}

		Iterator& operator=(const Iterator& x) { _M_current = x._M_current; }

		// Forward iterator requirements
		template <bool B> bool operator== (const Iterator<B>& x) const { return _M_current == x._M_current; }
		template <bool B> bool operator!= (const Iterator<B>& x) const { return _M_current != x._M_current; }

		value_type& operator* () { return *_M_current; }
		value_type* operator-> () { return _M_current; }

		Iterator& operator++() { ++_M_current; return *this; }
		Iterator operator++(int) { return Iterator(_M_current++); }

		// Bidirectional iterator requirements
		Iterator& operator--() { --_M_current; return *this; }
		Iterator operator--(int) { return Iterator(_M_current--); }

		// Random access iterator requirements
		reference operator[](difference_type __n) const { return _M_current[__n]; }

		Iterator& operator+=(difference_type __n) { _M_current += __n; return *this; }
		Iterator operator+(difference_type __n) const { return Iterator(_M_current + __n); }
		Iterator& operator-=(difference_type __n) { _M_current -= __n; return *this; }
		Iterator operator-(difference_type __n) const { return Iterator(_M_current - __n); }
	};
public:

	typedef Iterator<false>							iterator;
	typedef Iterator<true>							const_iterator;
	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

    vector(): _arr(NULL), _size(0), _cap(0), _alloc(allocator_type()), _rf(1.5) {}
    explicit vector(const allocator_type& alloc): _arr(NULL), _size(0), _cap(0), _alloc(alloc), _rf(1.5) {}
    explicit vector(size_type n, const value_type& value = T(), const allocator_type& alloc = allocator_type()):
		_arr(NULL), _size(n), _cap(n), _alloc(alloc), _rf(1.5)
	{
		_arr = _alloc.allocate(_cap);
		for (int i = 0; i < _cap; i++)
			_alloc.construct(&_arr[i], value);
	}

    template <class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()):
		_arr(NULL), _size(0), _cap(0), _alloc(alloc), _rf(1.5) {
		assign(first, last);
	}

    vector(const vector& x): _arr(NULL), _size(0), _cap(x._cap), _alloc(x._alloc) {
		_arr = _alloc.allocate(_cap);
		assign(x.begin(), x.end());
	}

    ~vector() {
		clear();
		_alloc.deallocate(_arr, _cap);
	}

    vector& operator=(const vector& x) { assign(x.begin(), x.end()); }

    template <class InputIterator>
	typename enable_if<!is_integral<InputIterator>::value, void>::type
    assign(InputIterator first, InputIterator last) {
		clear();
		while (first != last)
			push_back(*first++);
	}
    void assign(size_type n, const value_type& u) {
		clear();
		for (int i = 0; i < n; i++)
			push_back(u);
	}

    allocator_type get_allocator() const { return this->_alloc; }

    iterator               begin() { return iterator(_arr); }
    const_iterator         begin() const { return iterator(_arr); }
    iterator               end() { return iterator(&_arr[_size]); }
    const_iterator         end() const { return iterator(&_arr[_size]); }

    reverse_iterator       rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator       rend() { return reverse_iterator(begin()); };
    const_reverse_iterator rend() const { return reverse_iterator(begin()); }

    size_type size() const { return _size; }
    size_type max_size() const { return _alloc.max_size(); }
    size_type capacity() const { return _cap; }
    bool empty() const { return _size == 0; }
    void reserve(size_type n) {
		if (_cap >= n)
			return ;
		pointer arr = _alloc.allocate(n);
		for (int i = 0; i < _size; i++) _alloc.construct(&arr[i], _arr[i]);
		_alloc.deallocate(_arr, _cap);
		_cap = n;
		_arr = arr;
	}

    reference       operator[](size_type n) { return _arr[n]; }
    const_reference operator[](size_type n) const { return _arr[n]; }
    reference       at(size_type n) { if (n >= _size) throw std::out_of_range("vector"); return _arr[n]; }
    const_reference at(size_type n) const { if (n >= _size) throw std::out_of_range("vector"); return _arr[n]; };

    reference       front() { return *_arr; }
    const_reference front() const { return *_arr; }
    reference       back() { return _arr[_size - 1]; }
    const_reference back() const { return _arr[_size - 1]; }

    pointer			data() { return _arr; }
    const_pointer	data() const { return _arr; }

    void push_back(const value_type& x) {
		if (_size == _cap)
			reserve(_cap > 2 ? _cap * _rf : _cap + 1);
		_arr[_size++] = x;
	}

    void pop_back() {
		_size--;
		_alloc.destroy(_arr[_size]);
	}

    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, size_type n, const value_type& x);
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

    void clear() { for (int i = 0; i < _size; i++) _alloc.destroy(&_arr[i]); _size = 0; }

    void resize(size_type sz, const value_type& c = value_type()) {
		size_type i = sz;
		if (_size >= i)
			while (i != _size) _alloc.destroy(&_arr[i++]);
		else
			while (i != _size) push_back(c);
		_size = sz;
	}

    void swap(vector& x);

};

template< class T, class Alloc >
bool operator==( const ft::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator!=( const ft ::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator>=( const ft ::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator>( const ft ::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator<=( const ft ::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator<( const ft ::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs );
// specialized algorithms:
template< class T, class Alloc >
void swap(ft ::vector<T, Alloc>& x, ft ::vector<T, Alloc>& y);

}

#endif