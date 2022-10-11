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
    typedef T											value_type;
    typedef Allocator									allocator_type;
    typedef typename allocator_type::reference			reference;
    typedef typename allocator_type::const_reference	const_reference;
    typedef typename allocator_type::size_type			size_type;
    typedef typename allocator_type::difference_type	difference_type;
    typedef typename allocator_type::pointer			pointer;
    typedef typename allocator_type::const_pointer		const_pointer;
    typedef pointer										iterator;
    typedef const_pointer								const_iterator;
    typedef std::reverse_iterator<iterator>				reverse_iterator;
    typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
private:
	pointer			_arr;
	size_type		_size;
	allocator_type	_alloc;
public:
    vector();
    explicit vector(const allocator_type& alloc);
    explicit vector(size_type n, const value_type& value = T(), const allocator_type& = allocator_type());
    template <class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type& = allocator_type());
    vector(const vector& x);
    ~vector();
    vector& operator=(const vector& x);
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const value_type& u);

    allocator_type get_allocator() const;

    iterator               begin();
    const_iterator         begin() const;
    iterator               end();
    const_iterator         end() const;

    reverse_iterator       rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator       rend();
    const_reverse_iterator rend() const;

    size_type size() const;
    size_type max_size() const;
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type n);

    reference       operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference       at(size_type n);
    const_reference at(size_type n) const;

    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;

    value_type*       data();
    const value_type* data() const;

    void push_back(const value_type& x);
    void pop_back();

    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, size_type n, const value_type& x);
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

    void clear();

    void resize(size_type sz, const value_type& c = value_type());

    void swap(vector& x);

};

template< class T, class Alloc >
bool operator==( const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator!=( const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator>=( const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator>( const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator<=( const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs );
template< class T, class Alloc >
bool operator<( const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs );
// specialized algorithms:
template< class T, class Alloc >
void swap(std::vector<T, Alloc>& x, std::vector<T, Alloc>& y);

}

#endif