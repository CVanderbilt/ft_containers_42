#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"
# include "utils.hpp"
# include "avl_tree.hpp"

namespace ft {

template <
	class Key,
	class T,
	class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> >
>
class map
{
public:
	// types:
	typedef Key											key_type;
	typedef T											mapped_type;
	typedef pair<const key_type, mapped_type>			value_type;
	typedef Compare										key_compare;
	typedef Allocator									allocator_type;
	typedef typename allocator_type::reference			reference;
	typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;
	typedef typename allocator_type::size_type			size_type;
	typedef typename allocator_type::difference_type	difference_type;

	//typedef implementation-defined						iterator;
	//typedef implementation-defined						const_iterator;
	//typedef std::reverse_iterator<iterator>				reverse_iterator;
	//typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;

	class value_compare:
		public std::binary_function<value_type, value_type, bool>
	{
		friend class map;
	protected:
		key_compare comp;

		value_compare(key_compare c): comp(c) {}
	public:
		bool operator()(const_reference& x, const_reference& y) const { return comp(x.first, y.first); }
	};

private:

	typedef avl_tree<value_type, value_compare, allocator_type>	bst;

	value_compare	_cmp;
	bst				_bst;
public:
	
	typedef typename bst::iterator iterator;
	typedef typename bst::const_iterator const_iterator;
	typedef ft::reverse_iterator<iterator> reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>  const_reverse_iterator;

	// construct/copy/destroy:
	map(const Compare& comp = key_compare(), const Allocator& alloc = Allocator()):
		_cmp(comp),
		_bst(bst(_cmp, alloc))
	{}

	template <class InputIt>
	map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()):
		_cmp(comp),
		_bst(comp, alloc) {
			insert(first, last);
	}
	map (const map& other):
		_cmp(key_compare()),
		_bst(other._bst)
	{}

	~map() {}

	map& operator=(const map& other) {
		_cmp = other._cmp;
		_bst = other._bst;

		return *this;
	}

	// iterators:
	iterator begin() { return _bst.begin(); }
	const_iterator begin() const { return _bst.begin(); }
	iterator end() { return _bst.end(); }
	const_iterator end() const { return _bst.end(); }

	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

	// capacity:
	bool      empty()    const { return _bst.empty(); }
	size_type size()     const { return _bst.size(); }
	size_type max_size() const { return _bst.size(); }

	// element access:
	mapped_type& operator[](const key_type& key) {
		iterator it = getIteratorToPair(key);

		it = it == _bst.end() ? insert(ft::make_pair(key, mapped_type())).first : it; 
		return (*it).second;
	}

	mapped_type& at(const key_type& key) {
		iterator it = getIteratorToPair(key);

		if (it == _bst.end())
			throw std::out_of_range("map::at:  key not found");

		return (*it).second;
	}

	const mapped_type& at(const key_type& key) const {
		const_iterator it = getIteratorToPair(key);

		if (it == _bst.end())
			throw std::out_of_range("map::at:  key not found");

		return (*it).second;
	}
    // modifiers:
	pair<iterator, bool> insert(const value_type& v) { return _bst.insertAndReturnIterator(v); }
	iterator insert(iterator position, const value_type& v) { return _bst.insertAndReturnIterator(position, v); }
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		for (; first != last; first++)
			_bst.insert(first);
	}

	iterator  erase(iterator pos) { _bst.erase(pos); }
	size_type erase(const key_type& key) { erase(getIteratorToPair(key)); }
	iterator  erase(const_iterator first, const_iterator last) {
		for (iterator it = first; it != last; it++)
			erase(it);
	}
	void clear() { _bst.clear(); }

	void swap(map& m) {
		_bst.swap(m._bst);
		ft::swap(_cmp, m._cmp);
	}

	// observers:
	allocator_type get_allocator() const { return _bst.valloc; }
	key_compare    key_comp() const { return _cmp.comp; }
	value_compare  value_comp()    const { return _cmp; }

	// map operations:
	iterator find(const key_type& k) { return getIteratorToPair(k); }
	const_iterator find(const key_type& k) const { return getIteratorToPair(k); }

	size_type      count(const key_type& k) const {
		return find(k) == end() ? 0 : 1;
	}
	
	iterator lower_bound(const key_type& k) { return calculateLowerBound(k); }
	const_iterator lower_bound(const key_type& k) const { return calculateLowerBound(k); }

	iterator upper_bound(const key_type& k) { return calculateUpperBound(k); }
	const_iterator upper_bound(const key_type& k) const { return calculateUpperBound(k); }

	// TODO: pair<iterator,iterator>             equal_range(const key_type& k);
	// TODO: pair<const_iterator,const_iterator> equal_range(const key_type& k) const;

private:
	iterator calculateLowerBound(const key_type& key) {
		key_compare cmp = key_compare();
		iterator it;
		for (it = begin(); it != end(); it++)
			if (cmp(key, it->first))
				break ;
		return it;
	}

	iterator calculateUpperBound(const key_type& key) {
		key_compare cmp = key_compare();
		reverse_iterator it;
		for (it = rbegin(); it != begin(); it++)
			if (cmp(it->first, key))
				break ;
		return it.base();
	}

	iterator getIteratorToPair(const key_type& key) {
		value_type toSearch = ft::make_pair(key, mapped_type());
		return this->_bst.get(toSearch);
	}

	const_iterator getIteratorToPair(const key_type& key) const {
		value_type toSearch = ft::make_pair(key, mapped_type());
		return this->_bst.get(toSearch);
	}
};

template <class Key, class T, class Compare, class Allocator>
bool
operator==(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y)
{ return x._bst == y._bst; }

template <class Key, class T, class Compare, class Allocator>
bool
operator< (const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y)
{ return x._bst < y._bst; }

template <class Key, class T, class Compare, class Allocator>
bool
operator!=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y)
{ return x._bst != y._bst; }

template <class Key, class T, class Compare, class Allocator>
bool
operator> (const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y)
{ return x._bst > y._bst; }

template <class Key, class T, class Compare, class Allocator>
bool
operator>=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y)
{ return x._bst >= y._bst; }

template <class Key, class T, class Compare, class Allocator>
bool
operator<=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y)
{ return x._bst <= y._bst; }

// specialized algorithms:
/* TODO: template <class Key, class T, class Compare, class Allocator>
void
swap(map<Key, T, Compare, Allocator>& x, map<Key, T, Compare, Allocator>& y);*/

}

#endif