#ifndef RED_BLACK_TREE_HPP
# define RED_BLACK_TREE_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"
# include "utils.hpp"

# define __RED true
# define __BLACK false

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
	bool color; // 1/true -> red, 0/false -> black

	_red_black_tree_node(T v, Allocator alloc):
		_alloc(alloc),
		d(NULL),
		l(NULL),
		r(NULL),
		p(NULL),
		color(__RED)
	{
		d = _alloc.allocate(1);
		_alloc.construct(d, v);
	}
	_red_black_tree_node(const _red_black_tree_node &other):
		_alloc(other._alloc),
		l(other.l),
		r(other.r),
		p(other.p),
		color(other.color)
	{
		d = _alloc.allocate(1);
		_alloc.construct(d, *other.d);
	}

	void replaceValue(T v) { 
		_alloc.destroy(d);
		_alloc.construct(d, v);
	}

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
	t_node			*tnull;
	Compare			_cmp;
	size_t			_size;
	NodeAllocator	_alloc;
	ValueAllocator	_valloc;

public:

	_red_black_tree(
		Compare cmp = Compare(),
		const ValueAllocator valloc = ValueAllocator(),
		const NodeAllocator alloc = NodeAllocator())
		: r(NULL), tnull(NULL), _cmp(cmp), _size(0), _alloc(alloc), _valloc(valloc)
	{
		tnull = createNode(T());
		tnull->color = __BLACK;
		r = tnull;
	}
	_red_black_tree(const _red_black_tree& tree):
		r(NULL), tnull(NULL), _cmp(tree._cmp), _size(0), _alloc(tree._alloc), _valloc(tree._valloc)
	{
		tnull = createNode(T());
		tnull->color = __BLACK;
		r = tnull;
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
	private:
	public:

		typedef typename ft::iterator<ft::bidirectional_iterator_tag, typename std::conditional<Const, const T, T>::type>::value_type	value_type;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::iterator_category									iterator_category;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::difference_type										difference_type;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::pointer												pointer;
		typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::reference											reference;

		t_node *_tnull;
		t_node *_node;
		t_node *_root;
		//constructors/destructor
		Iterator(t_node *tnull = NULL, t_node *root = NULL):
			_tnull(tnull), _root(root), _node(getLeftMost(root)) {}
		Iterator(t_node *tnull, t_node *node, t_node *root)
			: _tnull(tnull), _root(root), _node(node)
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
		Iterator(const Iterator<Const>& x): _tnull(x._tnull), _root(x._root), _node(x._node) {}
		template <bool B>
		Iterator(const Iterator<B>& x, typename ft::enable_if<!B>::type* = 0): _tnull(x._tnull), _root(x._root), _node(x._node) {}

		Iterator& operator=(const Iterator& x) { _tnull = x._tnull; _node = x._node; _root = x._root; return *this; }

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
			if (node && node != _tnull)
				while (node->l != _tnull)
					node = node->l;
			return node;
		}

		t_node *getRightMost(t_node *node) {
			if (node && node != _tnull)
				while (node->r != _tnull)
					node = node->r;
			return node;
		}

		t_node *getNextNode(t_node *node) {
			t_node *aux;
			if (!node || node == _tnull)
				return NULL;

			if (node->r != _tnull)
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

			if (!node || node == _tnull)
				return getRightMost(_root);

			if (node->l != _tnull)
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
		return iterator(tnull, node, r); }
	const_iterator get(const T target) const { t_node *node = getNode(target); return const_iterator(tnull, node, r); }

	iterator begin() { return iterator(tnull, r); }
	const_iterator begin() const { return iterator(tnull, r); }
	iterator end() { return iterator(tnull, NULL, r); }
	const_iterator end() const { return iterator(tnull, NULL, r); }

	bool empty() const { return _size == 0; }
	size_t size() const { return _size; }
	size_t max_size() const; //TODO

	ft::pair<iterator, bool> insertAndReturnIterator(const T& v) {
		bool check;
		t_node* node = insert(v, &check);

		return ft::make_pair(iterator(tnull, node, r), check);
	}

	t_node *insert(const T& v, iterator hint, bool *check = NULL); //TODO
	/*t_node *insert(const T& v, bool *check = NULL) {
		return _insert(v, check);
	}*/

	void erase(iterator it) {
		deleteNodeHelper(it._node);
	}

	void clear() {
		t_node *n = begin()._node;
		size_t s = 0;
		while (n) {
			if (n->l != tnull) n = n->l;
			else if (n->r != tnull) n = n->r;
			else { 
				s++;
				n = eraseNodeWithNoChildren(n);
			}
		}
		_size = 0;
	}

	void swap(_red_black_tree& tree) {
		ft::swap(tnull, tree.tnull);
		ft::swap(_alloc, tree._alloc);
		ft::swap(_size, tree._size);
		ft::swap(_cmp, tree._cmp);
		ft::swap(_valloc, tree._valloc);
		ft::swap(r, tree.r);
	}


private:

	typedef t_node* NodePtr;

	t_node* eraseNodeWithNoChildren(t_node *_node) {
		t_node *parent = _node->p;

		if (parent) {
			if (parent->r == _node) {
				parent->r = tnull;
			} else {
				parent->l = tnull;
			}
		} else
			this->r = tnull;

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
		newNode->l = tnull;
		newNode->r = tnull;
		return newNode;
	}

	//utils (rotations, trasplants, minimum, maximum)
	void rbTransplant(NodePtr u, NodePtr v){
		if (!u->p) {
			r = v;
		} else if (u == u->p->l){
			u->p->l = v;
		} else {
			u->p->r = v;
		}
		v->p = u->p;
	}
	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->l != tnull) {
			node = node->l;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->r != tnull) {
			node = node->r;
		}
		return node;
	}
	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->r;
		x->r = y->l;
		if (y->l != tnull) {
			y->l->p = x;
		}
		y->p = x->p;
		if (x->p == nullptr) {
			this->r = y;
		} else if (x == x->p->l) {
			x->p->l = y;
		} else {
			x->p->r = y;
		}
		y->l = x;
		x->p = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->l;
		x->l = y->r;
		if (y->r != tnull) {
			y->r->p = x;
		}
		y->p = x->p;
		if (x->p == nullptr) {
			this->r = y;
		} else if (x == x->p->r) {
			x->p->r = y;
		} else {
			x->p->l = y;
		}
		y->r = x;
		x->p = y;
	}
	//insertion + insertion fix
	NodePtr insert(T key, bool *check = NULL) {
		NodePtr y = nullptr;
		NodePtr x = r;

	if (key.first == 5894)
		std::cout << "kk" << std::endl;

		while (x != tnull) {
			y = x;
			if (_cmp(key, *x->d)) {
				x = x->l;
			} else if (_cmp(*x->d, key)){
				x = x->r;
			} else {
				x->replaceValue(key);
				if (check) *check = true;
				return x;
			}
		}

		if (check) *check = false;

		NodePtr node = createNode(key);
		node->p = y;
		_size++;
		if (!y) {
			r = node;
			node->color = __BLACK;
			return r;
		} else if (_cmp(*node->d, *y->d))
			y->l = node;
		else
			y->r = node;

		if (node->p->p)
			fixInsert(node);
		return node;
	}

	void fixInsert(NodePtr k){
		NodePtr u;
		while (k->p->color == __RED) {
			if (k->p == k->p->p->r) {
				u = k->p->p->l; // uncle
				if (u->color == __RED) {
					// case 3.1
					u->color = __BLACK;
					k->p->color = __BLACK;
					k->p->p->color = __RED;
					k = k->p->p;
				} else {
					if (k == k->p->l) {
						// case 3.2.2
						k = k->p;
						rightRotate(k);
					}
					// case 3.2.1
					k->p->color = __BLACK;
					k->p->p->color = __RED;
					leftRotate(k->p->p);
				}
			} else {
				u = k->p->p->r; // uncle

				if (u->color == __RED) {
					// mirror case 3.1
					u->color = __BLACK;
					k->p->color = __BLACK;
					k->p->p->color = __RED;
					k = k->p->p;	
				} else {
					if (k == k->p->r) {
						// mirror case 3.2.2
						k = k->p;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->p->color = __BLACK;
					k->p->p->color = __RED;
					rightRotate(k->p->p);
				}
			}
			if (k == r) {
				break;
			}
		}
		r->color = __BLACK;
	}
	//deletion + deletion fix
	void deleteNodeHelper(NodePtr z) {
		NodePtr x, y;

		_size--;
		y = z;
		int y_original_color = y->color;
		if (z->l == tnull) {
			x = z->r;
			rbTransplant(z, z->r);
		} else if (z->r == tnull) {
			x = z->l;
			rbTransplant(z, z->l);
		} else {
			y = minimum(z->r);
			y_original_color = y->color;
			x = y->r;
			if (y->p == z) {
				x->p = y;
			} else {
				rbTransplant(y, y->r);
				y->r = z->r;
				y->r->p = y;
			}

			rbTransplant(z, y);
			y->l = z->l;
			y->l->p = y;
			y->color = z->color;
		}
		//delete z;
		_alloc.destroy(z);
		_alloc.deallocate(z, 1);
		if (y_original_color == __BLACK){
			fixDelete(x);
		}
	}

	void fixDelete(NodePtr x) {
		if (x == tnull)
			std::cout << "exactamente aqui" << std::endl;
		NodePtr s;
		while (x != r && x->color == __BLACK) {
			if (x == x->p->l) {
				s = x->p->r;
				if (s->color == __RED) {
					// case 3.1
					s->color = __BLACK;
					x->p->color = __RED;
					leftRotate(x->p);
					s = x->p->r;
				}

				if (s->l->color == __BLACK && s->r->color == __BLACK) {
					// case 3.2
					s->color = __RED;
					x = x->p;
				} else {
					if (s->r->color == __BLACK) {
						// case 3.3
						s->l->color = __BLACK;
						s->color = __RED;
						rightRotate(s);
						s = x->p->r;
					} 

					// case 3.4
					s->color = x->p->color;
					x->p->color = __BLACK;
					s->r->color = __BLACK;
					leftRotate(x->p);
					x = r;
				}
			} else {
				s = x->p->l;
				if (s->color == __RED) {
					// case 3.1
					s->color = __BLACK;
					x->p->color = __RED;
					rightRotate(x->p);
					s = x->p->l;
				}

				if (s->r->color == __BLACK && s->r->color == __BLACK) {
					// case 3.2
					s->color = __RED;
					x = x->p;
				} else {
					if (s->l->color == __BLACK) {
						// case 3.3
						s->r->color = __BLACK;
						s->color = __RED;
						leftRotate(s);
						s = x->p->l;
					} 

					// case 3.4
					s->color = x->p->color;
					x->p->color = __BLACK;
					s->l->color = __BLACK;
					rightRotate(x->p);
					x = r;
				}
			} 
		}
		x->color = __BLACK;
	}
};

}
#endif