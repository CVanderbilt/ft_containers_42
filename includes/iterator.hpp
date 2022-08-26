#ifndef ITERATOR_HPP
# define ITERATOR_HPP

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
		typedef std::random_access_iterator_tag iterator_category;
	};
	//and for pointers to const as
	template<class T> struct iterator_traits<const T*>
	{
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef std::random_access_iterator_tag iterator_category;
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
}

#endif