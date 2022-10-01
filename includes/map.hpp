#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include "avl_tree.hpp"

namespace ft{


template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T> >
> class map {
	public:
	/*
	*	Member types
	*/
	typedef Key 											key_type;
	typedef T 												mapped_type;
	typedef std::pair<const Key, T>							value_type;
	typedef std::size_t										size_type;
	typedef std::ptrdiff_t									difference_type;
	typedef Compare											key_compare;
	typedef Allocator										allocator_type;
	typedef value_type&										reference;
	typedef const value_type&								const_reference;
	typedef typename Allocator::pointer						pointer;
	typedef typename Allocator::const_pointer				const_pointer;
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
		value_compare (Compare c) : comp(c) {}

		public:

		bool operator() (const value_type& lhs, const value_type& rhs) const
		{
			return (comp(lhs.first, rhs.first));
		}
	};

	typedef avl_tree<value_type, value_compare, Allocator>	bst;

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
		const Compare& comp = Compare(),
		const Allocator& alloc = Allocator() ):
		_cmp(comp),
		_bst(comp, alloc)
	{
		this->insert(first, last);
	}

	map (const map& other):
		_bst(other._bst) //TODO make sure this is being copied correctly
	{}

	~map() {}

	map& operator=(const map& other) {
		this->_cmp = other._cmp;
		this->_bst = other._bst;
	}

	allocator_type get_allocator() const {
		return _bst._valloc;
	}

	mapped_type& at(const Key& key) {
		typename bst::Iterator it = this->getIteratorToPair(key);

		if (it == this->_bst.end())
			throw std::out_of_range("Temporal exception message"); //check exception message

		return (*it).second;
	}

	const mapped_type& at(const Key& key) const {
		typename bst::Iterator it = this->getIteratorToPair(key);

		if (it == this->_bst.end())
			throw std::out_of_range("Temporal exception message"); //check exception message

		return (*it).second;
	}

	mapped_type& operator[] (const Key& key) {
		typename bst::Iterator it = this->getIteratorToPair(key);

		if (it == this->_bst.end())
		//crear el elemento a devolver
			throw std::out_of_range("Temporal exception message (next commented line will be added instead of this one when make pair is created"); //check exception message
			//return insert(std::make_pair(key, T())).first->second;
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

	//todo replace std::pair with ft::pair
	std::pair<iterator, bool> insert( const value_type& value ) { //1
		return (this->_bst.insertAndReturnIterator(value));
	}

	iterator insert( iterator hint, const value_type& value ) { //4
		return (this->_bst.insertAndReturnIterator(value, hint._node));
	}

	template< class InputIt >
	void insert( InputIt first, InputIt last ) { //7
		for (; first != last; first++)
			this->_bst.insert(first);
	}

	private:
	typename bst::Iterator getIteratorToPair(const Key& key) {
		value_type toSearch;
		toSearch.first = key;
		return this->_bst.get(toSearch);
	}
};
}

#endif