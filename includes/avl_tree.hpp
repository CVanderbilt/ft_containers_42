#ifndef AVL_TREE_HPP
# define AVL_TREE_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"
# include "utils.hpp"

namespace ft
{

template<class T, class Allocator = std::allocator<T> >
class avl_node {

private:
	Allocator _alloc;
public:
	T *d;
	avl_node *l;
	avl_node *r;
	avl_node *p;

	avl_node(T v, avl_node *parent, Allocator alloc = Allocator()):
		_alloc(alloc), d(NULL), l(NULL), r(NULL), p(parent)
	{
		d = _alloc.allocate(1);
		_alloc.construct(d, v);
	}

	avl_node(const avl_node& n):
		_alloc(n._alloc), d(NULL), l(NULL), r(NULL), p(n.p)
	{
		d = _alloc.allocate(1);
		_alloc.construct(d, *n.d);
	}

	~avl_node() {
		_alloc.destroy(d);
		_alloc.deallocate(d, 1);
	}
};

template <class T> //T will be reference to T(in map) or const reference to T == map::reference or map::const_reference
class Avl_Tree_Iterator: public ft::iterator<ft::bidirectional_iterator_tag, T> {

public:

	typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::value_type					value_type;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::iterator_category	iterator_category;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::difference_type		difference_type;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::pointer				pointer;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, value_type>::reference			reference;

	typedef avl_node<T> avl;
		
	avl *_node;

private:

	avl *_root;

public:
	Avl_Tree_Iterator(avl *root = NULL):
		_root(root), _node(getLeftMost(root))
	{} //equivalente a begin() -> con root seteamos el nodo a leftmost de esa root
	Avl_Tree_Iterator(avl *node, avl *root):
		_root(root), _node(node)
	{
		if (!_root && _node) {
			avl *parent = _node->p;
			root = _node->p;
			while(parent) {
				root = parent;
				parent = parent->p;
			}
		}
	}

	value_type& operator* () { //returning reference, needed to be able to modify ref returned in map::operator[]
		return (*_node->d);
	}

	// preincrement
	Avl_Tree_Iterator& operator++ () {
		_node = getNextNode(_node);
		return *this;
	}
	
	// postincrement
	Avl_Tree_Iterator operator++ (int) {
		Avl_Tree_Iterator ret(*this);

		_node = getNextNode(_node);
		return ret;
	}
		
	//TODO: fix end() - 1 
	// predecrement. move backward to largest value < current value
	Avl_Tree_Iterator operator-- () {
		std::cout << "--1" << std::endl;
		_node = getPrevNode(_node);
		return *this;
	}
		
	// postdecrement
	Avl_Tree_Iterator operator-- (int) {
		std::cout << "--2" << std::endl;
		Avl_Tree_Iterator ret(*this);

		_node = getPrevNode(_node);
		return ret;
	}

	friend bool operator== (const Avl_Tree_Iterator& a, const Avl_Tree_Iterator& b) {
		if (a._node && b._node)
			std::cout << "Comparing (" << (*(a._node->d)).first << "," << (*(b._node->d)).first << ")" << std::endl;
		return a._node == b._node;
	};
	friend bool operator!= (const Avl_Tree_Iterator& a, const Avl_Tree_Iterator& b) { return !(a == b); };

private:
		
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

template <
	class T=int, class Compare = std::less<T>,
	class ValueAllocator = std::allocator<T>,
	class NodeAllocator = std::allocator<avl_node<T> >
>
class avl_tree
{
private:

	typedef avl_node<T> avl;

	avl				*r;
	Compare			_cmp;
	size_t			_size;
	NodeAllocator	_alloc;
	ValueAllocator	_valloc;

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
		const ValueAllocator valloc = ValueAllocator(),
		const NodeAllocator alloc = NodeAllocator())
		: r(NULL), _cmp(cmp), _size(0), _alloc(alloc), _valloc(valloc)
	{}

	avl_tree(const avl_tree& tree)
	: r(NULL), _cmp(tree._cmp), _size(0), _alloc(tree._alloc), _valloc(tree._valloc) {
		for (iterator it = tree.begin(); it != tree.end(); it++) {
			this->insert(*it);
		}
	}

	~avl_tree() {
		this->clear();
	}

	avl_tree& operator= (const avl_tree& other) {
		this->clear();
		for (iterator it = other.begin(); it != other.end(); it++) {
			this->insert(*it);
		}
		return *this;
	}

	void clear() {
		for (iterator it = this->begin(); it != this->end();) {
			this->erase(it++);
		}
	}

void ft(const std::string& prefix, const avl* node, bool isLeft, int kk) const{
		//std::cout << "test ultimo" << std::endl;
		//exit(0);
		if( node != nullptr )
		{
			//std::cout << "entra" << std::endl;
			std::cout << prefix;
			std::string parentMsg = "p(NULL)";
			if (node->p) {
				parentMsg = "p(" + std::to_string((int)(node->p->d)->first) + ")";
				std::cout << (isLeft ? "├L─" : "└R─") << parentMsg;
			} else {
				std::cout << "* -> " << parentMsg;
			}
			int diff = difference(node);
			std::string suffix = diff > 1 || diff < -1 ? "!!!!!!!!!!" : "";

			//std::cout << "+++" << std::endl;
			std::cout << "v(" << (node->d)->first << ')';
			//std::cout << "---" << std::endl;

			std::cout << " [diff (" << difference(node) << ")" << suffix << "]";

			std::cout << std::endl;

			// enter the next tree level - left and right branch
			ft( prefix + (isLeft ? "│   " : "    "), node->l, true, kk);
			ft( prefix + (isLeft ? "│   " : "    "), node->r, false, kk);
		}
	}


	void printBT() const {
		std::cout << "printing as a triangle" << std::endl;
		std::cout << "r: " << r << std::endl;
		avl *kk(r);
		avl *kk2(r);
		std::cout << kk2 << std::endl;
		std::cout << "======" << std::endl;
		ft("", r, false, 2);
	}

	void show() {
		if (!r)
			std::cout << "Tree is Empty" << std::endl;
		else
			show(r, 1);
	}

	//balancing called from this function
	avl *insert(const T& v, bool *check = NULL) {
		avl *parent = NULL;
		avl *n = r;
		bool isRight = true;
		while (n != NULL) {
			if (_cmp(v, *n->d)) {
				parent = n;
				n = n->l;
				isRight = false;
			} else if (_cmp(*n->d, v)) {
				parent = n;
				n = n->r;
				isRight = true;
			} else
			{
				if (check) *check = true;
				return n;
			}
		}

		if (check) *check = false;

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

	size_t size() const {
		return _size;
	}

	size_t max_size() const {
		return (size_t) -1;
	}

public:
	typedef Avl_Tree_Iterator<T> iterator;
	typedef Avl_Tree_Iterator<const T> const_iterator;

	/*
	*	Elemental access
	*/
	iterator get(T target) {
		avl *node = getNode(target);

		return iterator(node, r);
	}

	iterator begin() {
		//std::cout << "begin normal" << std::endl;
		return iterator(r);
	}

	iterator begin() const {
		//std::cout << "begin const" << std::endl;
		return iterator(r);
	}

	iterator end() {
		//std::cout << "end normal" << std::endl;
		/*iterator ret = iterator(r);
		ret._node = NULL;
		std::cout << "node updated" << std::endl;
		return ret;*/
		return iterator(NULL, r);
	}

	iterator end() const {
		//std::cout << "end const" << std::endl;
		//this->printBT();
		return iterator(NULL, r);
	}

	ft::pair<iterator, bool> insertAndReturnIterator(const T& v) {
		bool check;
		//std::cout << "inserting: " << v.first << ":" << v.second << std::endl;
		avl* node = insert(v, &check);

		return ft::make_pair(iterator(node, r), check);
	}

	void erase(iterator it) {
		if (!it._node)
			return ;
		avl *n = erase_node(it);
		r = n ? balance_iter(n) : r;
		_size--;
	}

	bool empty() { return this->_size == 0; }

	avl *insertBehind(const T& v, avl *node, bool nodeIsRight) {
		avl *left = NULL;
		avl *right = NULL;

		if (this->_cmp(v, *(node->d))) {
			right = node;
		} else if (this->_cmp(*(node->d), v)) {
			left = node;
		} else { //hinted node equals newNode -> overwrite
			*(node->d) = v;
			return node;
		}
		avl * newNode = this->createNode(v, node->p);
		newNode->r = right;
		newNode->l = left;

		if (nodeIsRight)
			node->p->r = newNode;
		else
			node->p->l = newNode;
		node->p = newNode;
		this->_size++;
		return node;
	}

	avl *insert(const T& v, iterator hint, bool *check = NULL) {
		avl *new_node;

		avl *hinted_node = hint._node;
		if (!hinted_node) return insert(v, check);

		avl *parent_node = hinted_node->p;
		if (!parent_node) return insert(v, check);

		avl **parent_child = parent_node->r == hinted_node ? &(parent_node->r) : &(parent_node->l);

		if (parent_node->r == hinted_node) {
			if (_cmp(v, *(hinted_node->d))) {
				new_node = createNode(v, parent_node);
				parent_node->r = new_node;
				new_node->r = hinted_node;
				hinted_node->p = new_node;
				this->_size++;
				return new_node;
			}
		} else {
			if (_cmp(*(hinted_node->d), v)) {
				new_node = createNode(v, parent_node);
				parent_node->l = new_node;
				new_node->l = hinted_node;
				hinted_node->p = new_node;
				this->_size++;
				return new_node;
			}
		}
		return insert(v, check);
	}

private:

	avl *erase_node(iterator it) { //cambiar esto por una friend function
			if (!it._node)
				return NULL;
			avl *b;
			if (it._node->r && it._node->l)
				return eraseNodeWithTwoChildren(it._node, it);
			else if (!it._node->r && !it._node->l)
				return eraseNodeWithNoChildren(it._node);
			else
				return eraseNodeWithOneChildren(it._node);
		}

	avl *eraseNodeWithTwoChildren(avl *erasedNode, iterator& it) {
		avl *successorNode = (++it)._node;
		avl *ret = successorNode;

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

	avl* eraseNodeWithOneChildren(avl *_node) {
		avl *parent = _node->p;

		avl *child = _node->r ? _node->r : _node->l;

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

	avl* eraseNodeWithNoChildren(avl *_node) {
		avl *parent = _node->p;

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

	int height(avl *t) const {
		int h = 0;

		if (t != NULL) {
			int l_height = height(t->l);
			int r_height = height(t->r);
			int max_height = std::max(l_height, r_height);
			h = max_height + 1;
		}
		return h;
	}

	int difference (const avl *t) const {
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

	void printBT(const std::string& prefix, const avl* node, bool isLeft) const{
		if( node != nullptr )
		{
			std::cout << prefix;
			std::string parentMsg = "p(NULL)";
			if (node->p) {
				parentMsg = "p(-)";
				std::cout << (isLeft ? "├L─" : "└R─") << parentMsg;
			} else {
				std::cout << "* -> " << parentMsg;
			}

			int diff = difference(node);

			std::string suffix = diff > 1 || diff < -1 ? "!!!!!!!!!!" : "";

			std::cout << "v(" << ((*(node->p->d)).first) << ')';

			std::cout << " [diff (" << difference(node) << ")" << suffix << "]";

			std::cout << std::endl;

			// enter the next tree level - left and right branch
			printBT( prefix + (isLeft ? "│   " : "    "), node->l, true);
			printBT( prefix + (isLeft ? "│   " : "    "), node->r, false);
		}
	}

	avl *createNode(T v, avl *parent) {
		avl_node<T> *n = this->_alloc.allocate(1);
		_alloc.construct(n, avl_node<T>(v, parent, _valloc));
		n->l = NULL;
		n->r = NULL;
		n->p = parent;

		return n;
	}

	avl *getNode(T v) {
		avl *node = r;

		while (node) {
			if (_cmp(v, *node->d)) {
				node = node->l;
			} else if (_cmp(*node->d, v)) {
				node = node->r;
			} else {
				return node;
			}
		}

		return NULL;
	}

};

}

#endif