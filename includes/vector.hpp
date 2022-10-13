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
    typedef ft::__normal_iterator<pointer, vector>							iterator;
    typedef ft::__normal_iterator<const_pointer, vector>					const_iterator;
	 /*typedef ft::iterator<ft::iterator_traits<pointer>, value_type>			iterator;
    typedef ft::iterator<ft::iterator_traits<const_pointer>, value_type >	const_iterator;*/
    typedef ft ::reverse_iterator<iterator>									reverse_iterator;
    typedef ft ::reverse_iterator<const_iterator>							const_reverse_iterator;
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

    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;

    value_type*       data();
    const value_type* data() const;

    void push_back(const value_type& x) {
		if (_size == _cap)
			reserve(_cap > 2 ? _cap * _rf : _cap + 1);
		_arr[_size++] = x;
	}
    void pop_back();

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