#ifndef AVL_TREE_HPP
# define AVL_TREE_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"

namespace ft
{
	//add a destructor and check if it is being called + implement logic to maybe call destructor of d (? maybe also automatic)
	template<class T>
	class avl_node {
		public:
		T d;
		avl_node *l;
		avl_node *r;
		avl_node *p;
	};


	template <
		class T=int, class Compare = std::less<T>,
		class Allocator = std::allocator<avl_node<T> >
	>
	class avl_tree
	{
	public:

	typedef avl_node<T> avl;

	private:

	avl			*r;
	Compare		_cmp;
	size_t		_size;
	Allocator	_alloc;

	public:

	void show(avl *p, int l) {
		int i;
		if (p != NULL) {
			show(p->r, l+ 1);
			std::cout<<" ";
			if (p == r)
				std::cout << "Root -> ";
			for (i = 0; i < l&& p != r; i++)
				std::cout << " ";
				std::cout << p->d;
				show(p->l, l + 1);
		}
	}

	avl_tree(
		Compare cmp = Compare(),
		const Allocator alloc = Allocator())
		: r(NULL), _cmp(cmp), _size(0), _alloc(alloc)
	{}

	avl_tree(const avl_tree& tree)
	: r(NULL), _cmp(tree._cmp), _size(0), _alloc(tree._alloc) {
		for(Iterator it = tree.begin(); it != tree.end(); it++) {
			this->insert(*it);
		}
	}

	void printBT() {
		printBT("", r, false);    
	}

	void show() {
		if (!r)
			std::cout << "Tree is Empty" << std::endl;
		else
			show(r, 1);
	}

	/*
	*	Should be private, we don't need to expose internal logic
	*/
	avl *getNode(T v) {
		avl *node = r;

		while (node) {
			if (_cmp(v, node->d)) {
				node = node->l;
			} else if (v == node->d) {
				return node;
			} else {
				node = node->r;
			}
		}

		return NULL;
	}

	//balancing called from this function
	avl *insert(T v) {
		avl *parent = NULL;
		avl *n = r;
		bool isRight = true;
		while (n != NULL) {
			if (_cmp(v, n->d)) {
				parent = n;
				n = n->l;
				isRight = false;
			} else if (v != n->d) {
				parent = n;
				n = n->r;
				isRight = true;
			} else
				return n;
		}

		n = createNode(v, parent);

		if (parent == NULL) {
			r = n;
			_size = 0;
		}
		else if (isRight)
			parent->r = n;
		else
			parent->l = n;
		
		_size++;
		if (parent) //todo maybe this if can be removed when we dont need to use output of balance_iter
			r = balance_iter(parent);
		return n;
	}

	size_t size() {
		return _size;
	}

	size_t max_size() {
		return (size_t) -1;
	}


	class Iterator:
	public ft::iterator<ft::bidirectional_iterator_tag, T> {
		
		private:
		avl *_root;
		avl *_node;
		Allocator _alloc;

		public:
		Iterator(avl *root = NULL, Allocator alloc = Allocator()):
			_root(root), _node(getLeftMost(root)), _alloc(alloc) {}
		Iterator(avl *node, avl *root = NULL, Allocator alloc = Allocator())
			: _root(root), _node(node), _alloc(alloc) {}
		Iterator(const Iterator& it): _node(it._node), _alloc(it._alloc) {}
		
		avl *erase() {
			if (!_node)
				return NULL;
			avl *b;
			if (_node->r && _node->l)
				return eraseNodeWithTwoChildren();
			else if (!_node->r && !_node->l)
				return eraseNodeWithNoChildren();
			else
				return eraseNodeWithOneChildren();
		}

		bool operator== (const Iterator& rhs) const { return _node == rhs._node; }
		
		bool operator!= (const Iterator& rhs) const {
			return _node != rhs._node;
		}
		
		const T& operator* () const {
			return _node->d;
		}

		// preincrement
		Iterator& operator++ () {
			_node = getNextNode(_node);
			return *this;
		}
		
		// postincrement
		Iterator operator++ (int) {
			Iterator ret(*this);

			_node = getNextNode(_node);
			return ret;
		}
		
		//TODO: fix end() - 1 
		// predecrement. move backward to largest value < current value
		Iterator operator-- () {
			_node = getPrevNode(_node);
			return *this;
		}
		
		// postdecrement
		Iterator operator-- (int) {
			Iterator ret(*this);

			_node = getPrevNode(_node);
			return ret;
		}

		private:

		avl *eraseNodeWithTwoChildren() {
			Iterator it(*this);

			avl *oldNode = _node;
			avl *newNode = (++it)._node;
			avl *parentNewNode = newNode->p;
			avl *parentOldNode = _node->p;

			_node = newNode;
			newNode->p = oldNode->p;

			if (parentOldNode) {
				if (parentOldNode->l == oldNode)
					parentOldNode->l = newNode;
				else
					parentOldNode->r = newNode;
			}

			parentNewNode->l = newNode->r;
			if (newNode->r)
				newNode->r->p = parentNewNode;

			newNode->r = oldNode->r;
			newNode->r->p = newNode;
			newNode->l = oldNode->l;
			newNode->l->p = newNode;

			_alloc.deallocate(oldNode, 1);
			return (parentNewNode);
		}

		avl* eraseNodeWithOneChildren() {
			avl *parent = _node->p;

			avl *child = _node->r ? _node->r : _node->l;

			child->p = parent;
			if (parent) {
				if (parent->r == _node) {
					parent->r = child;
				} else {
					parent->l = child;
				}
			}

			//delete(_node->d); TODO: revisar esto
			_alloc.deallocate(_node, 1);

			return parent;			
		}

		avl* eraseNodeWithNoChildren() {
			avl *parent = _node->p;

			if (parent) {
				if (parent->r == _node) {
					parent->r = NULL;
				} else {
					parent->l = NULL;
				}
			}

			_alloc.deallocate(_node, 1);
			//delete(_node->d); TODO: revisar esto

			return parent;
		}
		
		avl *getLeftMost(avl *node) {
			if (node) {	
				while (node->l)
					node = node->l;
			}
			return node;
		}

		avl *getRightMost(avl *node) {
			while (node->r)
				node = node->r;
			return node;
		}

		avl *getNextNode(avl *node) {
			avl *aux;
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

		avl *getPrevNode(avl *node) {
			avl *aux;
			
			if (!node)
				return NULL;

			if (node->l)
				return getRightMost(node->r);

			do
			{
				aux = node;
				node = node->p;
			} while (node && node->l == aux);
			return node;
		}
	};

	public:
	/*
	*	Elemental access
	*/
	Iterator get(T target) {
		avl *node = getNode(target);

		return Iterator(node, r, _alloc);
	}

	Iterator begin() {
		return Iterator(r, _alloc);
	}

	Iterator end() {
		return Iterator(NULL, r, _alloc);
	}

	void erase(Iterator it) {
		avl *n = it.erase();
		r = balance_iter(n);
		_size--;
	}

	private:

	int height(avl *t) {
		int h = 0;

		if (t != NULL) {
			int l_height = height(t->l);
			int r_height = height(t->r);
			int max_height = std::max(l_height, r_height);
			h = max_height + 1;
		}
		return h;
	}

	int difference (const avl *t) {
		int l_height = height(t->l);
		int r_height = height(t->r);
		int b_factor = l_height - r_height;
		return b_factor;
	}

	avl *rr_rotat(avl *parent) {
		avl *t;
		
		t = parent->r;
		parent->r = t->l;
		t->l = parent;

		t->p = parent->p;
		parent->p = t;
		avl *old_tl = parent->r;
		if (old_tl)
			old_tl->p = parent;
		return t;
	}

	avl *ll_rotat(avl *parent) {
		avl *t;

		t = parent->l;
		parent->l = t->r;
		t->r = parent;

		t->p = parent->p;
		parent->p = t;
		avl *old_tr = parent->l;
		if (old_tr)
			old_tr->p = parent;
		return t;
	}

	avl *lr_rotat(avl *parent) {
		avl *t;
		t = parent->l;
		parent->l = rr_rotat(t);
		return ll_rotat(parent);
	}

	avl *rl_rotat(avl *parent) {
		avl *t;
		t = parent->r;
		parent->r = ll_rotat(t);
		return rr_rotat(parent);
	}

	avl *balance_rec(avl *t) {
		int bal_factor = difference(t);
		if (bal_factor > 1) {
			if (difference(t->l) > 0)
				t = ll_rotat(t);
			else
				t = lr_rotat(t);
		} else if (bal_factor < -1) {
			if (difference(t->r) > 0)
				t = rl_rotat(t);
			else
				t = rr_rotat(t);
		}
		return t;
	}

	avl *balance_iter(avl *t) {
		avl *aux = t;
		while (t) {
			avl *parent = t->p;
			bool isLeft = false;
			if (parent)
				isLeft = parent->l == t;
			t = balance_rec(t);
			if (parent) {
				if (isLeft)
					parent->l = t;
				else
					parent->r = t;
			}
			aux = t;
			t = t->p;
		}

		return aux;
	}

	void printBT(const std::string& prefix, const avl* node, bool isLeft) {
	if( node != nullptr )
    {
        std::cout << prefix;

		std::string parentMsg = "p(NULL)";
		if (node->p) {
			parentMsg = "p(" + std::to_string(node->p->d) + ")";
        	std::cout << (isLeft ? "├L─" : "└R─") << parentMsg;
		} else {
			std::cout << "* -> " << parentMsg;
		}

		int diff = difference(node);

		std::string suffix = diff > 1 || diff < -1 ? "!!!!!!!!!!" : "";

        std::cout << "v(" <<node->d << ')';

		std::cout << " [diff (" << difference(node) << ")" << suffix << "]";

		std::cout << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->l, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->r, false);
    }
}

	avl *createNode(T v, avl *parent) {
		avl_node<T> *n = this->_alloc.allocate(1);
		n->d = v;
		n->l = NULL;
		n->r = NULL;
		n->p = parent;

		return n;
	}

	};
}

#endif