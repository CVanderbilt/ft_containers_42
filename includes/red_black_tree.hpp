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

template <
	class T=int, class Compare = std::less<T>,
	class ValueAllocator = std::allocator<T>
>
class _red_black_tree
{
public:
	struct Node {
		T *data;
		Node *parent;
		Node *left;
		Node *right;
		int color;
	};

	typedef Node *NodePtr;

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

		NodePtr _tnull;
		NodePtr _node;
		NodePtr _root;
		//constructors/destructor
		Iterator(NodePtr tnull = NULL, NodePtr root = NULL):
			_tnull(tnull), _root(root), _node(getLeftMost(root)) {}
		Iterator(NodePtr tnull, NodePtr node, NodePtr root)
			: _tnull(tnull), _root(root), _node(node)
		{
			if (!_root && _node) {
				_root = _node;
				NodePtr aux = _node->parent;
				while (aux) {
					_root = aux;
					aux = aux->parent;
				}
			}
		}
		Iterator(const Iterator<Const>& x): _tnull(x._tnull), _root(x._root), _node(x._node) {}
		template <bool B>
		Iterator(const Iterator<B>& x, typename ft::enable_if<!B>::type* = 0): _tnull(x._tnull), _root(x._root), _node(x._node) {}

		Iterator& operator=(const Iterator& x) { _tnull = x._tnull; _node = x._node; _root = x._root; return *this; }

		template <bool B> bool operator== (const Iterator<B>& x) const { return _node == x._node; }
		template <bool B> bool operator!= (const Iterator<B>& x) const { return _node != x._node; }

		value_type& operator* () { return (*_node->data); }
		value_type* operator-> () { return (_node->data); }

		// prefix
		Iterator& operator++ () { _node = getNextNode(_node); return *this; }
		Iterator operator-- () { _node = getPrevNode(_node); return *this; }
		
		// postfix
		Iterator operator++ (int) { Iterator ret(*this); _node = getNextNode(_node); return ret; }
		Iterator operator-- (int) { Iterator ret(*this); _node = getPrevNode(_node); return ret; }

	private:
		NodePtr getLeftMost(NodePtr node) {
			if (node && node != _tnull)
				while (node->left != _tnull)
					node = node->left;
			return node;
		}

		NodePtr getRightMost(NodePtr node) {
			if (node && node != _tnull)
				while (node->right != _tnull)
					node = node->right;
			return node;
		}

		NodePtr getNextNode(NodePtr node) {
			NodePtr aux;
			if (!node || node == _tnull)
				return _tnull;

			if (node->right != _tnull)
				return getLeftMost(node->right);

			do
			{
				aux = node;
				node = node->parent;
			} while (node && node->right == aux);
			return node ? node : _tnull;
		}

		NodePtr getPrevNode(NodePtr node) {
			NodePtr aux;

			if (!node || node == _tnull)
				return getRightMost(_root);

			if (node->left != _tnull)
				return getRightMost(node->left);

			do
			{
				aux = node;
				node = node->parent;
			} while (node && node->left == aux);
			return node ? node : _tnull;
		}
	};

	typedef Iterator<false> iterator;
	typedef Iterator<true> const_iterator;

	friend bool operator== (const iterator& a, const iterator& b) { return a._node == b._node; }
	friend bool operator!= (const iterator& a, const iterator& b) { return !(a == b); }


private:
	NodePtr root;
	NodePtr TNULL;
	Compare cmp;
	size_t _size;
	ValueAllocator alloc;
	std::allocator<Node> nalloc;

	//do not call with a null root
	template <class _Key, class _Compare>
	iterator lowerBound(const _Key& __v, NodePtr __root, NodePtr __result, _Compare __cmp) {
		while (__root != TNULL)
		{
			if (!__cmp(__root->data->first, __v))
			{
				__result = __root;
				__root = __root->left;
			}
			else
				__root = __root->right;
		}
		return iterator(TNULL, __result, root);
	}

	//do not call with a null root
	template <class _Key, class _Compare>
	iterator upperBound(const _Key&__v, NodePtr __root, NodePtr __result, _Compare __cmp) {
		while (__root != TNULL)
		{
			if (__cmp(__v, __root->data->first))
			{
				__result = __root;
				__root = __root->left;
			}
			else
				__root = __root->right;
		}
		return iterator(TNULL, __result, root);
	}
	// For balancing the tree after deletion
	void deleteFix(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
			if (s->color == 1) {
				s->color = 0;
				x->parent->color = 1;
				leftRotate(x->parent);
				s = x->parent->right;
			}

			if (!s->left || !s->right) return ;
			if (s->left->color == 0 && s->right->color == 0) {
				s->color = 1;
				x = x->parent;
			} else {
				if (s->right->color == 0) {
					s->left->color = 0;
					s->color = 1;
					rightRotate(s);
					s = x->parent->right;
				}

				s->color = x->parent->color;
				x->parent->color = 0;
				s->right->color = 0;
				leftRotate(x->parent);
				x = root;
			}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (!s->left || !s->right) return ;
				if (s->right->color == 0 && s->right->color == 0) {
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}

	void rbTransplant(NodePtr u, NodePtr v) {
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left) {
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	NodePtr getNode(T value) const {
		NodePtr n = root;
		while (n != TNULL) {
			if (cmp(value, *n->data))
				n = n->left;
			else if (cmp(*n->data, value))
				n = n->right;
			else
				break ;
		}
		return n;
	}

	size_t deleteNodeHelper(NodePtr z) {
		NodePtr x, y;

		NodePtr l = z->left;
		NodePtr r = z->right;

		if (z == TNULL)
			return 0;
		_size--;
		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		
		alloc.destroy(z->data);
		alloc.deallocate(z->data, 1);
		nalloc.deallocate(z, 1);

		if (y_original_color == 0) {
			deleteFix(x);
		}
		return 1;
	}

  // For balancing the tree after insertion
	void insertFix(NodePtr k) {
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left;
				if (u->color == 1) {
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						k = k->parent;
						rightRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right;

				if (u->color == 1) {
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->right) {
						k = k->parent;
						leftRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

public:
	_red_black_tree(Compare _cmp = Compare(), ValueAllocator _alloc = ValueAllocator()):
		cmp(_cmp), _size(0), alloc(_alloc)
	{
		TNULL = nalloc.allocate(1);
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		TNULL->data = NULL;
		root = TNULL;
	}

	_red_black_tree(const _red_black_tree& t):
		cmp(t.cmp), _size(0), alloc(t.alloc)
	{
		TNULL = nalloc.allocate(1);
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		TNULL->data = NULL;
		root = TNULL;
		for (const_iterator it = t.begin(); it != t.end(); it++){ insert(*it); }
	}

	~_red_black_tree() {
		clear();
		nalloc.deallocate(TNULL, 1);
	}

	_red_black_tree& operator=(const _red_black_tree& t) {
		clear();
		cmp = t.cmp;
		for (const_iterator it = t.begin(); it != t.end(); it++)
			insert(*it);
		return *this;
	}

	ft::pair<iterator, bool> insertAndReturnIterator(T v) {
		bool check;
		NodePtr n = this->insert(v, &check);
		return ft::make_pair(iterator(TNULL, n, root), check);
	}

	size_t size() const { return _size; }
	bool empty() const { return _size == 0; }
	size_t max_size() const { return nalloc.max_size(); }

	iterator begin() { return iterator(TNULL, root); }
	const_iterator begin() const { return const_iterator(TNULL, root); }
	iterator end() { return iterator(TNULL, TNULL, root); }
	const_iterator end() const { return iterator(TNULL, TNULL, root); }

	iterator get(T v) { return iterator(TNULL, getNode(v), root); }
	const_iterator get(T v) const { return const_iterator(TNULL, getNode(v), root); }

	size_t erase(iterator it) {
		return this->deleteNodeHelper(it._node);
	}

	void clear() {
		if (_size == 0)
			return ;
		NodePtr n = begin()._node;
		while (n != TNULL) {
			if (n->left != TNULL) n = n->left;
			else if (n->right != TNULL) n = n->right;
			else { 
				//delete n, its a childless node
				alloc.destroy(n->data);
				alloc.deallocate(n->data, 1);
				NodePtr aux = n->parent;
				nalloc.deallocate(n, 1);
				if (!aux) break ;
				if (aux->left == n) aux->left = TNULL;
				else aux->right = TNULL;
				n = aux;
			}
		}
		_size = 0;
	}

	void swap(_red_black_tree t) {
		ft::swap(t.alloc, alloc);
		ft::swap(t.cmp, cmp);
		ft::swap(t._size, _size);
		ft::swap(t.TNULL, TNULL);
		ft::swap(t.root, root);
	}

	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	NodePtr successor(NodePtr x) {
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	NodePtr predecessor(NodePtr x) {
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// Inserting a node
	NodePtr insert(T key, bool *check = NULL) {

		NodePtr y = nullptr;
		NodePtr x = root;

		while (x != TNULL) {
			y = x;
			if (cmp(key, *x->data)) {
				x = x->left;
			} else if (cmp(*x->data, key)) {
				x = x->right;
			} else {
				if (check) *check = true;
				alloc.destroy(x->data);
				alloc.construct(x->data, key);
				return x;
			} 
		}
		if (check) *check = false;
		NodePtr node = nalloc.allocate(1);
		node->parent = nullptr;
		node->data = alloc.allocate(1);
		alloc.construct(node->data, key);
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1;

		_size++;

		node->parent = y;
		if (y == nullptr) {
			root = node;
			node->color = 0;
			_size = 1;
			return node;
		} else if (cmp(key, *y->data)) {
			y->left = node;
		} else {
			y->right = node;
		}

		if (node->parent->parent == nullptr) {
			return node;
		}

		insertFix(node);
		return node;
	}

	NodePtr getRoot() {
		return this->root;
	}

	template <class _Key, class _Compare>
	iterator lowerBound(const _Key& __v, _Compare __cmp) { return lowerBound(__v, root, TNULL, __cmp); }
	template <class _Key, class _Compare>
	iterator upperBound(const _Key&__v, _Compare __cmp) { return upperBound(__v, root, TNULL, __cmp); }

};

}
#endif