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
	template <class T=int, class Compare = std::less<T> >
	class avl_tree {

	private:

	struct avl {
		T d;
		struct avl *l;
		struct avl *r;
		struct avl *p;
		bool isRight;
	};
	public:
	/*
	*	Elemental access
	*/
	T get(T target) {
		return get(target, r, "searching in root");
	}

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

	avl_tree(Compare cmp) {
		r = NULL;
		_cmp = cmp;
		_size = 0;
	}

	avl_tree() {
		r = NULL;
		_cmp = Compare();
		_size = 0;
	}

	avl_tree(const avl_tree& tree) { //TODO: use deep copy
		r = NULL;
		_cmp = tree._cmp;
		_size = 0;
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

	avl *getNode(T v) {
		avl *aux = r;

		while (aux) {
			if (aux->d == v) 
				return aux;
			if (_cmp(v, aux->d))
				aux = aux->l;
			else
				aux = aux->r;
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
		avl *_node;

		public:
		Iterator(): _node(NULL) {}
		Iterator(avl *node): _node(node) {}
		Iterator(const Iterator& it): _node(it._node) {}

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

			delete(oldNode);
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
			delete(_node);

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

			delete(_node);
			//delete(_node->d); TODO: revisar esto

			return parent;
		}
		
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

		Iterator static begin(avl *root) { return Iterator(getLeftMost(root)); }
		Iterator static end(avl *root) { return Iterator(); }

		bool operator== (const Iterator& rhs) const { return _node == rhs._node; }
		
		bool operator!= (const Iterator& rhs) const {
			return _node != rhs._node;
		}
		
		const T& operator* () const {
			return _node->d;
		}
		
		static avl *getLeftMost(avl *node) {
			if (node) {	
				while (node->l)
					node = node->l;
			}
			return node;
		}

		static avl *getRightMost(avl *node) {
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
	};

	Iterator begin() {
		return Iterator::begin(r);
	}

	Iterator end() {
		return Iterator::end(r);
	}

	void erase(Iterator it) {
		avl *n = it.erase();
		r = balance_iter(n);
		_size--;
	}

private:

	struct avl	*r;
	Compare		_cmp;
	size_t		_size;

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
		avl *n = new avl; //todo replace using allocator instead
		n->d = v;
		n->l = NULL;
		n->r = NULL;
		n->p = parent;

		return n;
	}

	T get(T target, struct avl *node, std::string msg) {
		if (!node) {
			return (T)NULL;
		}
		if (_cmp(target, node->d)){
			return get(target, node->l, "searching in left child");
		}
		if (target == node->d) {
			return node->d;
		}
		return get(target, node->r, "searching in right child");
	}
	};
}

#endif