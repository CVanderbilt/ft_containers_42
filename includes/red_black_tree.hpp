#ifndef RED_BLACK_TREE_HPP
# define RED_BLACK_TREE_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"
# include "utils.hpp"

namespace ft {

template<class T, class Allocator = std::allocator<T> >
class _red_black_tree_node {

private:
	Allocator _alloc;
public:
	T *d;
	_red_black_tree_node *l;
	_red_black_tree_node *r;
	_red_black_tree_node *p;
	bool isRed;

	_red_black_tree_node(T v, Allocator alloc):
		_alloc(alloc),
		d(NULL),
		l(NULL),
		r(NULL),
		p(NULL),
		isRed(true)
	{
		d = _alloc.allocate(1);
		_alloc.construct(d, v);
	}
	_red_black_tree_node(const _red_black_tree_node &other):
		_alloc(other._alloc),
		l(other.l),
		r(other.r),
		p(other.p),
		isRed(other.isRed)
	{
		d = _alloc.allocate(1);
		_alloc.construct(d, *other.d);
	}

	T getValue() { return *d; }

	~_red_black_tree_node() {
		_alloc.destroy(d);
		_alloc.deallocate(d, 1);
	}
};


template <
	class T=int, class Compare = std::less<T>,
	class ValueAllocator = std::allocator<T>
>
class _red_black_tree
{
public:
	typedef _red_black_tree_node<T> t_node;
private:

	typedef std::allocator<t_node> NodeAllocator;

	t_node			*r;
	Compare			_cmp;
	size_t			_size;
	NodeAllocator	_alloc;
	ValueAllocator	_valloc;

public:

	_red_black_tree(
		Compare cmp = Compare(),
		const ValueAllocator valloc = ValueAllocator(),
		const NodeAllocator alloc = NodeAllocator())
		: r(NULL), _cmp(cmp), _size(0), _alloc(alloc), _valloc(valloc)
	{}
	_red_black_tree(const _red_black_tree& tree):
		r(NULL), _cmp(tree._cmp), _size(0), _alloc(tree._alloc), _valloc(tree._valloc)
	{
		for (const_iterator it = tree.begin(); it != tree.end(); it++)
			insert(*it);
	}
	~_red_black_tree() { clear(); }

	_red_black_tree& operator= (const _red_black_tree& other) {
		clear();
		for (const_iterator it = other.begin(); it != end(); it++)
			insert(*it);
		return *this;
	}

	template <bool Const = false>
	class Iterator:
	public ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type > {
	public:

		typedef typename ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type>::value_type	value_type;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::iterator_category									iterator_category;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::difference_type										difference_type;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::pointer												pointer;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::reference											reference;

		t_node *_node;
		t_node *_root;
		//constructors/destructor
		Iterator(t_node *root = NULL):
			_root(root), _node(getLeftMost(root)) {}
		Iterator(t_node *node, t_node *root)
			: _root(root), _node(node)
		{
			if (!_root && _node) {
				_root = _node;
				t_node *aux = _node->p;
				while (aux) {
					_root = aux;
					aux = aux->p;
				}
			}
		}
		Iterator(const Iterator<Const>& x): _root(x._root), _node(x._node) {}
		template <bool B>
		Iterator(const Iterator<B>& x, typename ft::enable_if<!B>::type* = 0): _root(x._root), _node(x._node) {}

		Iterator& operator=(const Iterator& x) { _node = x._node; _root = x._root; return *this; }

		template <bool B> bool operator== (const Iterator<B>& x) const { return _node == x._node; }
		template <bool B> bool operator!= (const Iterator<B>& x) const { return _node != x._node; }

		value_type& operator* () { return (*_node->d); }
		value_type* operator-> () { return (_node->d); }

		// prefix
		Iterator& operator++ () { _node = getNextNode(_node); return *this; }
		Iterator operator-- () { _node = getPrevNode(_node); return *this; }
		
		// postfix
		Iterator operator++ (int) { Iterator ret(*this); _node = getNextNode(_node); return ret; }
		Iterator operator-- (int) { Iterator ret(*this); _node = getPrevNode(_node); return ret; }

	private:
		t_node *getLeftMost(t_node *node) {
			if (node) {	
				while (node->l)
					node = node->l;
			}
			return node;
		}

		t_node *getRightMost(t_node *node) {
			while (node->r)
				node = node->r;
			return node;
		}

		t_node *getNextNode(t_node *node) {
			t_node *aux;
			if (!node)
				return NULL;

			if (node->r)
				return getLeftMost(node->r);

			do
			{
				aux = node;
				node = node->p;
			} while (node && node->r == aux);
			return node;
		}

		t_node *getPrevNode(t_node *node) {
			t_node *aux;

			if (!node)
				return getRightMost(_root);

			if (node->l)
				return getRightMost(node->l);

			do
			{
				aux = node;
				node = node->p;
			} while (node && node->l == aux);
			return node;
		}
	};

	typedef Iterator<false> iterator;
	typedef Iterator<true> const_iterator;

	friend bool operator== (const iterator& a, const iterator& b) { return a._node == b._node; }
	friend bool operator!= (const iterator& a, const iterator& b) { return !(a == b); }

	iterator get(const T target) { 
		t_node *node = getNode(target);
		return iterator(node, r); }
	const_iterator get(const T target) const { t_node *node = getNode(target); return const_iterator(node, r); }

	iterator begin() { return iterator(r); }
	const_iterator begin() const { return iterator(r); }
	iterator end() { return iterator(NULL, r); }
	const_iterator end() const { return iterator(NULL, r); }

	bool empty() const { return _size == 0; }
	size_t size() const { return _size; }
	size_t max_size() const;

	ft::pair<iterator, bool> insertAndReturnIterator(const T& v) {
		bool check;
		t_node* node = insert(v, &check);

		return ft::make_pair(iterator(node, r), check);
	}

	t_node *insert(const T& v, iterator hint, bool *check = NULL);

	t_node *insert(const T& v, bool *check = NULL) {
		//Base case, nothing in the tree
		if (!r) {
			r = createNode(v);
			_size = 1;
			return r;
		}
		//Search to find the node's correct place
		t_node *aux = r;
		t_node *p = NULL;
		do
		{
			p = aux;
			if (_cmp(v, *aux->d)) {
				aux = aux->l;
			} else if (_cmp(*aux->d, v)) {
				aux = aux->r;
			} else {
				if (check) *check = true;
				return aux;
			}
		} while (aux);
		t_node *n = createNode(v);
		n->p = p;
		if (_cmp(v, *p->d))
			p->l = n;
		else
			p->r = n;
		fix_tree(n);
		_size++;
		if (check) *check = false;
		return (n);
	}

	void erase(iterator it);//erase
	void clear() {
		t_node *n = begin()._node;
		size_t s = 0;
		while (n) {
			if (n->l) n = n->l;
			else if (n->r) n = n->r;
			else { 
				s++;
				n = eraseNodeWithNoChildren(n);
			}
		}
		_size = 0;
	}

	void swap(_red_black_tree& tree);


private:
	t_node *eraseNodeWithTwoChildren(t_node *erasedNode, iterator& it) {
		t_node *successorNode = (++it)._node;
		t_node *ret = successorNode;

		if (successorNode->p != erasedNode) {
			successorNode->p->l = successorNode->r;
			if (successorNode->r)
				successorNode->r->p = successorNode->p;
			successorNode->r = erasedNode->r;
			if (successorNode->r)
				successorNode->r->p = successorNode;
			ret = successorNode->p;
		}

		successorNode->p = erasedNode->p;
		successorNode->l = erasedNode->l;
		if (successorNode->l)
			successorNode->l->p = successorNode;

		if (erasedNode->p) {
			if (erasedNode->p->r == erasedNode)
				erasedNode->p->r = successorNode;
			else
				erasedNode->p->l = successorNode;
		}

		_alloc.destroy(erasedNode);
		_alloc.deallocate(erasedNode, 1);

		return ret;
	}

	t_node* eraseNodeWithOneChildren(t_node *_node) {
		t_node *parent = _node->p;

		t_node *child = _node->r ? _node->r : _node->l;

		child->p = parent;
		if (parent) {
			if (parent->r == _node) {
				parent->r = child;
			} else {
				parent->l = child;
			}
		} else
			this->r = child;

		_alloc.destroy(_node);
		_alloc.deallocate(_node, 1);

		return parent;			
	}

	t_node* eraseNodeWithNoChildren(t_node *_node) {
		t_node *parent = _node->p;

		if (parent) {
			if (parent->r == _node) {
				parent->r = NULL;
			} else {
				parent->l = NULL;
			}
		} else
			this->r = NULL;

		_alloc.destroy(_node);
		_alloc.deallocate(_node, 1);

		return parent;
	}

	t_node *getNode(const T& n) {
		t_node *ret;
		ret = r;
		while(ret) {
			if (_cmp(n, *ret->d)) {
				ret = ret->l;
			} else if (_cmp(*ret->d, n)) {
				ret = ret->r;
			} else {
				return ret;
			}
		}
		return ret;
	}

	t_node *createNode(T v) {
		/*avl_node<T> *n = this->_alloc.allocate(1);
		_alloc.construct(n, avl_node<T>(v, parent, _valloc));*/
		t_node *newNode = this->_alloc.allocate(1);
		_alloc.construct(newNode, t_node(v, _valloc));
		return newNode;
	}
	//posiblemente no funcione
	void right_rotate(t_node *n) {
		t_node *sibling = n->l;
		n->l = sibling->r;
		//turn sibling's left subtree into node's right subtree
		if (sibling->r)
			sibling->r->p = n;
		sibling->p = n->p;
		if (n->p == NULL)
			r = sibling;
		else {
			if (n == n->p->r)
				n->p->r = sibling;
			else
				n->p->l = sibling;
		}
		sibling->r = n;
		n->p = sibling;
	}

	void left_rotate(t_node *n) {
		t_node *sibling = n->r;
		n->r = sibling->l;
		//turn sibling's left subtree into node's right subtree
		if (sibling->l)
			sibling->l->p = n;
		sibling->p = n->p;
		if (n->p == NULL)
			r = sibling;
		else {
			if (n == n->p->l)
				n->p->l = sibling;
			else
				n->p->r = sibling;
		}
		sibling->l = n;
		n->p = sibling;
	}

	void fix_tree(t_node *n) {
		if (!n || !n->p || !n->p->p)
			return ;
		while (n != r && n->p->isRed) {
			if (n->p == n->p->p->l) {
				t_node *uncle = n->p->p->r;
				if (uncle && uncle->isRed) {
					//case 1
					n->p->isRed = false;
					uncle->isRed = false;
					n->p->p->isRed = true;
					n = n->p->p;
				} else {
					if (n == n->p->r) {
						//case 2
						n = n->p;
						left_rotate(n);
					}
					//case 3
					n->p->isRed = false;
					n->p->p->isRed = true;
					right_rotate(n->p->p);
				}
			} else {
				t_node *uncle = n->p->p->l;
				if (uncle && uncle->isRed) {
					//case 1
					n->p->isRed = false;
					uncle->isRed = false;
					n->p->p->isRed = true;
					n = n->p->p;
				} else {
					if (n == n->p->l) {
						//case 2
						n = n->p;
						right_rotate(n);
					}
					//case 3
					n->p->isRed = false;
					n->p->p->isRed = true;
					left_rotate(n->p->p);
				}
			}
		}
		r->isRed = false;
	}
};

}
#endif