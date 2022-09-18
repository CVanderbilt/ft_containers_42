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

	typedef typename bst::Iterator			iterator;
	typedef const typename bst::Iterator	const_iterator;
	/*
	*	Constructor todo: check explicit keyword, check if I have to add map() default constructor
	*/
	explicit map (const key_compare& comp = key_compare(),
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
		//todo use insert with iterators to insert all the elements inside [first, last)
	}

	map (const map& other):
		_bst(other._bst) //TODO make sure this is being copied correctly
	{}

	~map() {}

	map& operator= (const map& other) {
		this->_cmp = other._cmp;
		this->_bst = other._bst;
	}
};
}

#endif