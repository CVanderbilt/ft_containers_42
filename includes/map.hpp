#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include "avl_tree.hpp"

namespace ft{


template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<ft::pair<const Key, T> >
> class map {
	public:
	/*
	*	Member types
	*/
	typedef Key 											key_type;
	typedef T 												mapped_type;
	typedef ft::pair<const key_type, mapped_type>			value_type;
	typedef std::size_t										size_type;
	typedef std::ptrdiff_t									difference_type;
	typedef Compare											key_compare;
	typedef Allocator										allocator_type;
	typedef value_type&										reference;
	typedef const value_type&								const_reference;
	typedef typename allocator_type::pointer				pointer;
	typedef typename allocator_type::const_pointer			const_pointer;
	//todo implement reverse iterator

	protected:
	/*
	*	Member class
	*/
	class value_compare : std::binary_function<value_type, value_type, bool>
	{
		friend class map<key_type, mapped_type, key_compare, allocator_type>;

		protected:
		key_compare comp;
		value_compare (key_compare c) : comp(c) {}

		public:

		bool operator() (const_reference lhs, const_reference rhs) const
		{
			return (comp(lhs.first, rhs.first));
		}
	};

	typedef avl_tree<value_type, value_compare, allocator_type>	bst;

	value_compare _cmp;
	bst	_bst;

	public:

	typedef typename bst::Iterator iterator;
	//const iterator missing

	/*
	*	Constructor todo: check explicit keyword, check if I have to add map() default constructor
	*/
	map (const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type()):
		_cmp(comp),
		_bst(bst(_cmp, alloc))
	{}

	template< class InputIt >
	map (InputIt first, InputIt last,
		const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type() ):
		_cmp(comp),
		_bst(comp, alloc)
	{
		this->insert(first, last);
	}

	map (const map& other):
		_cmp(key_compare()),
		_bst(other._bst) //TODO make sure this is being copied correctly
	{}

	~map() {}

	map& operator=(const map& other) {
		this->_cmp = other._cmp;
		this->_bst = other._bst;

		return *this;
	}

	allocator_type get_allocator() const {
		return _bst._valloc;
	}

	mapped_type& at(const key_type& key) {
		typename bst::Iterator it = this->getIteratorToPair(key);

		if (it == this->_bst.end())
			throw std::out_of_range("Temporal exception message"); //check exception message

		return (*it).second;
	}

	const mapped_type& at(const key_type& key) const {
		typename bst::Iterator it = this->getIteratorToPair(key);

		if (it == this->_bst.end())
			throw std::out_of_range("Temporal exception message"); //check exception message

		return (*it).second;
	}

	T& operator[] (const key_type& key) {
		typename bst::Iterator it = this->getIteratorToPair(key);

		it = it == this->_bst.end() ? insert(ft::make_pair(key, mapped_type())).first : it; 
		return (*it).second;
	}
	//todo: add logic containing duplicated code in both at methods and operator[] method

	iterator begin() { return this->_bst.begin(); }
	//const_iterator begin() { ... }
	iterator end() { return this->_bst.end(); }
	//const_iterator end() { ... }

	//iterator rbegin() { return this->_bst.begin(); }
	//const_iterator begin() { ... }
	//iterator rend() { return this->_bst.end(); }
	//const_iterator end() { ... }

	bool empty() const { return this->_bst.empty(); }
	size_type size() const { return this->_bst.size(); }
	size_type max_size() const { return this->_bst.max_size(); }
	void clear() { this->_bst.clear(); }

	ft::pair<iterator, bool> insert( const_reference value ) { //1
		return (this->_bst.insertAndReturnIterator(value));
	}

	iterator insert( iterator hint, const_reference value ) { //4
		return (this->_bst.insertAndReturnIterator(value, hint._node).first);
	}

	template< class InputIt >
	void insert( InputIt first, InputIt last ) { //7
		for (; first != last; first++)
			this->_bst.insert(first);
	}

	void erase( iterator pos ) {
		this->_bst.erase(pos);
	}

	void erase( iterator first, iterator last ) {
		for (iterator it = first; it != last; it++)
			this->erase(it);
	}

	size_type erase( const key_type& key ) {
		this->erase(this->getIteratorToPair(key));
	}

	private:
	typename bst::Iterator getIteratorToPair(const key_type& key) {
		value_type toSearch = ft::make_pair(key, mapped_type());
		return this->_bst.get(toSearch);
	}
};
}

#endif