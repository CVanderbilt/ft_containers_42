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
	typedef Key 						key_type;
	typedef T 							mapped_type;
	typedef std::pair<const Key, T>		value_type;
	typedef std::size_t					size_type;
	typedef std::ptrdiff_t				difference_type;
	typedef Compare						key_compare;
	typedef Allocator					allocator_type;
	typedef value_type&					reference;
	typedef const value_type&			const_reference;
	typedef typename Allocator::pointer			pointer;
	typedef typename Allocator::const_pointer	const_pointer;
	/*iterator 	LegacyBidirectionalIterator to value_type
	const_iterator 	LegacyBidirectionalIterator to const value_type
	reverse_iterator 	std::reverse_iterator<iterator>
	const_reverse_iterator 	std::reverse_iterator<const_iterator>*/

	protected:
	allocator_type			_alloc;
	key_compare				_comp;
	avl_tree <value_type>	_bst;

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

	public:
	/*
	*	Constructor todo: check explicit keyword, check if I have to add map() default constructor
	*/
	explicit map (const key_compare& comp = key_compare(),
		const allocator_type& alloc = allocator_type()):
		_alloc(alloc),
		_comp(comp),
		_bst()
	{}
};
}

#endif