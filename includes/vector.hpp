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

template <typename T, bool Const = false>
class VectorIterator:
public ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type > {
public:
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type>::value_type	value_type;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::iterator_category									iterator_category;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::difference_type										difference_type;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::pointer												pointer;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::reference											reference;

	pointer _M_current;

	VectorIterator(): _M_current(NULL) {}
	VectorIterator(pointer m): _M_current(m) {}
	VectorIterator(const VectorIterator<T, Const>& x): _M_current(x._M_current) {}
	
	// todo: check if this constructor would be enough for both cases (cit(cit) and cit(it)) and it(it) only
	template <bool B>
	VectorIterator(const VectorIterator<T, B>& x, typename ft::enable_if<!B>::type* = 0): _M_current(x._M_current) {}

	VectorIterator& operator=(const VectorIterator& x) { _M_current = x._M_current; return *this; }

	// Forward iterator requirements
	template <bool B> bool operator== (const VectorIterator<T, B>& x) const { return _M_current == x._M_current; }
	template <bool B> bool operator!= (const VectorIterator<T, B>& x) const { return _M_current != x._M_current; }

	value_type& operator* () { return *_M_current; }
	value_type* operator-> () { return _M_current; }

	VectorIterator& operator++() { ++_M_current; return *this; }
	VectorIterator operator++(int) { return VectorIterator(_M_current++); }

	// Bidirectional iterator requirements
	VectorIterator& operator--() { --_M_current; return *this; }
	VectorIterator operator--(int) { return VectorIterator(_M_current--); }

	// Random access iterator requirements
	reference operator[](difference_type __n) const { return _M_current[__n]; }

	VectorIterator& operator+=(difference_type __n) { _M_current += __n; return *this; }
	VectorIterator operator+(difference_type __n) const { return VectorIterator(_M_current + __n); }
	VectorIterator& operator-=(difference_type __n) { _M_current -= __n; return *this; }
	VectorIterator operator-(difference_type __n) const { return VectorIterator(_M_current - __n); }

	template <bool B>
	difference_type operator-(const VectorIterator<T, B>& x) const { return _M_current - x._M_current; }

	reference operator[](difference_type __n) { return _M_current[__n]; }

	template <bool B> bool operator< (const VectorIterator<T, B>& x) const { return _M_current < x._M_current; }
	template <bool B> bool operator<= (const VectorIterator<T, B>& x) const { return _M_current <= x._M_current; }
	template <bool B> bool operator> (const VectorIterator<T, B>& x) const { return _M_current > x._M_current; }
	template <bool B> bool operator>= (const VectorIterator<T, B>& x) const { return _M_current >= x._M_current; }

};

template <class T, class Allocator = std::allocator<T> >
class vector
{
public:
    typedef T											value_type;
    typedef Allocator									allocator_type;
    typedef typename allocator_type::reference			reference;
    typedef typename allocator_type::const_reference	const_reference;
    typedef typename allocator_type::size_type			size_type;
    typedef typename allocator_type::difference_type	difference_type;
    typedef typename allocator_type::pointer			pointer;
    typedef typename allocator_type::const_pointer		const_pointer;
	typedef VectorIterator<value_type, false>			iterator;
	typedef VectorIterator<value_type, true>			const_iterator;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;
private:
	pointer			_arr;
	size_type		_size;
	size_type		_cap;
	allocator_type	_alloc;
	const float		_rf;

public:


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

    vector(const vector& x): _arr(NULL), _size(0), _cap(x._cap), _alloc(x._alloc), _rf(1.5) {
		_arr = _alloc.allocate(_cap);
		assign(x.begin(), x.end());
	}

    ~vector() {
		clear();
		_alloc.deallocate(_arr, _cap);
	}

    vector& operator=(const vector& x) { assign(x.begin(), x.end()); return *this; }

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
		for (size_type i = 0; i < _size; i++) _alloc.construct(&arr[i], _arr[i]);
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
		_alloc.construct(&_arr[_size], x);
		_size++;
	}

    void pop_back() {
		_size--;
		_alloc.destroy(_arr[_size]);
	}

    iterator insert(const_iterator position, const value_type& x) { return insert(position, 1, x); }

    iterator insert(const_iterator position, size_type n, const value_type& x) {
		iterator it = move_right(position, n);
		for (size_type i = 0; i < n; i++)
			*it++ = x;
		return it;
	}

	template <class InputIterator>
	typename enable_if<!is_integral<InputIterator>::value, iterator>::type
    insert(const_iterator position, InputIterator first, InputIterator last) {
		iterator ret = const_cast<value_type*>(position._M_current);
		while (first != last) {
			//todo: instead of this calculate index once and use index internally 
			difference_type diff = position - begin();
			ret = insert(position, 1, *(first++));
			position = _arr + diff;
			position++;
		}
		return ret;
	}

    iterator erase(const_iterator position) {
		if (position >= end() || _size == 0)
			return end();

		iterator ret = const_cast<value_type*>(position._M_current);

		iterator next = ret;
		iterator it = next++;

		_alloc.destroy(it._M_current);

		while (next != end())
			*it++ = *next++;
		
		_size--;
		return ret;
	}

    iterator erase(const_iterator first, const_iterator last) {
		iterator ret = const_cast<value_type*>(first._M_current);
		iterator l = const_cast<value_type*>(last._M_current);
		iterator e = end();
		size_type i = 0;
		while (&l[i] < e._M_current) {
			_alloc.destroy(&ret[i]);
			_alloc.construct(&ret[i], l[i]);
			_alloc.destroy(&l[i]);
			i++;
		}
		_size -= last - first;
		return ret;
	}

    void clear() { for (size_type i = 0; i < _size; i++) _alloc.destroy(&_arr[i]); _size = 0; }

    void resize(size_type sz, const value_type& c = value_type()) {
		size_type i = sz;
		if (_size >= i)
			while (i != _size) _alloc.destroy(&_arr[i++]);
		else
			while (i != _size) push_back(c);
		_size = sz;
	}

    void swap(vector& x) {
		ft::swap(_size, x._size);
		ft::swap(_cap, x._cap);
		ft::swap(_arr, x._arr);
		ft::swap(_alloc, x._alloc);
	}

private:
	iterator move_right(const_iterator pos, size_type n) {
		size_type s = _size + n;
		difference_type diff = pos._M_current - _arr;
		reserve(s);

		pos = begin() + diff;
		iterator ret = _arr + diff;
		iterator e = end() - 1;

		while (e >= pos) {
			e[n] = *e;
			e--;
		}
		_size = s;
		return ret;
	}
};

template< class T, class Alloc >
bool operator==( const ft::vector<T,Alloc>& lhs, const ft ::vector<T,Alloc>& rhs ) {
	if (lhs.size() != rhs.size()) return false;
	return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template< class T, class Alloc >
bool operator!=( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) { return !(lhs == rhs); }
template< class T, class Alloc >
bool operator>=( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) { return !(lhs < rhs); }
template< class T, class Alloc >
bool operator>( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) { return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()); }
template< class T, class Alloc >
bool operator<=( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) { return !(lhs > rhs); }
template< class T, class Alloc >
bool operator<( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) { return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
// specialized algorithms:
template< class T, class Alloc >
void swap(ft ::vector<T, Alloc>& x, ft ::vector<T, Alloc>& y) { x.swap(y); }

}

#endif