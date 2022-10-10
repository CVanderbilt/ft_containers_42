#include <iostream>
#include <string>

namespace ft {

template <
	class T, class Compare = std::less<T>,
	class ValueAllocator = std::allocator<T>
>
class _red_black_tree_
{
	public:
	Compare cmp;
	ValueAllocator alloc;
	_red_black_tree_(Compare _cmp = Compare(), ValueAllocator _alloc = ValueAllocator()):
		cmp(_cmp),
		alloc(_alloc)
	{
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		TNULL->data = alloc.allocate(1);
		alloc.construct(TNULL->data, ft::make_pair(123, "123"));
		root = TNULL;
		_size = 0;
	}

	struct Node {
		T *data; // holds the key
		Node *parent; // pointer to the parent
		Node *left; // pointer to left child
		Node *right; // pointer to right child
		int color; // 1 -> Red, 0 -> Black
	};

	typedef Node *NodePtr;

		public:
	size_t _size;
	private:
		NodePtr root;
		NodePtr TNULL;

		// initializes the nodes with appropirate values
		// all the pointers are set to point to the null pointer
		void initializeNULLNode(NodePtr node, NodePtr parent) {
			node->data = 0;
			node->parent = parent;
			node->left = nullptr;
			node->right = nullptr;
			node->color = 0;
		}

		// fix the rb tree modified by the delete operation
		void fixDelete(NodePtr x) {
			NodePtr s;
			while (x != root && x->color == 0) {
				if (x == x->parent->left) {
					s = x->parent->right;
					if (s->color == 1) {
						// case 3.1
						s->color = 0;
						x->parent->color = 1;
						leftRotate(x->parent);
						s = x->parent->right;
					}
					if (!s->left || !s->right) return ;
					if (s->left->color == 0 && s->right->color == 0) {
						// case 3.2
						s->color = 1;
						x = x->parent;
					} else {
						if (s->right->color == 0) {
							// case 3.3
							s->left->color = 0;
							s->color = 1;
							rightRotate(s);
							s = x->parent->right;
						} 

						// case 3.4
						s->color = x->parent->color;
						x->parent->color = 0;
						s->right->color = 0;
						leftRotate(x->parent);
						x = root;
					}
				} else {
					s = x->parent->left;
					if (s->color == 1) {
						// case 3.1
						s->color = 0;
						x->parent->color = 1;
						rightRotate(x->parent);
						s = x->parent->left;
					}

					if (s->right->color == 0 && s->right->color == 0) {
						// case 3.2
						s->color = 1;
						x = x->parent;
					} else {
						if (s->left->color == 0) {
							// case 3.3
							s->right->color = 0;
							s->color = 1;
							leftRotate(s);
							s = x->parent->left;
						} 

						// case 3.4
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


		void rbTransplant(NodePtr u, NodePtr v){
			if (u->parent == nullptr) {
				root = v;
			} else if (u == u->parent->left){
				u->parent->left = v;
			} else {
				u->parent->right = v;
			}
			v->parent = u->parent;
		}

		void deleteNodeHelper(NodePtr node, T key) {
			// find the node containing key
			NodePtr z = TNULL;
			NodePtr x, y;
			while (node != TNULL){
				if (cmp(key, *node->data)) {
					node = node->left;
				} else if (cmp(*node->data, key)) {
					node = node->right;
				} else {
					z = node;
					break ;
				}
			}

			if (z == TNULL) {
				std::cout<<"Couldn't find key in the tree"<<std::endl;
				return;
			}

			T *l = z->left->data;
			T *r = z->right->data;

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
			delete z;
			
			std::cout << "l: " << l->first << "; r: " << r->first << std::endl;

			if (y_original_color == 0){
				fixDelete(x);
			}
		}
		
		// fix the red-black tree
		void fixInsert(NodePtr k){
			NodePtr u;
			while (k->parent->color == 1) {
				if (k->parent == k->parent->parent->right) {
					u = k->parent->parent->left; // uncle
					if (u->color == 1) {
						// case 3.1
						u->color = 0;
						k->parent->color = 0;
						k->parent->parent->color = 1;
						k = k->parent->parent;
					} else {
						if (k == k->parent->left) {
							// case 3.2.2
							k = k->parent;
							rightRotate(k);
						}
						// case 3.2.1
						k->parent->color = 0;
						k->parent->parent->color = 1;
						leftRotate(k->parent->parent);
					}
				} else {
					u = k->parent->parent->right; // uncle

					if (u->color == 1) {
						// mirror case 3.1
						u->color = 0;
						k->parent->color = 0;
						k->parent->parent->color = 1;
						k = k->parent->parent;	
					} else {
						if (k == k->parent->right) {
							// mirror case 3.2.2
							k = k->parent;
							leftRotate(k);
						}
						// mirror case 3.2.1
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

		// Pre-Order traversal
		// Node->Left Subtree->Right Subtree
		void preorder() {
			preOrderHelper(this->root);
		}

		// In-Order traversal
		// Left Subtree -> Node -> Right Subtree
		void inorder() {
			inOrderHelper(this->root);
		}

		// Post-Order traversal
		// Left Subtree -> Right Subtree -> Node
		void postorder() {
			postOrderHelper(this->root);
		}

		// find the node with the minimum key
		NodePtr minimum(NodePtr node) {
			while (node->left != TNULL) {
				node = node->left;
			}
			return node;
		}

		// find the node with the maximum key
		NodePtr maximum(NodePtr node) {
			while (node->right != TNULL) {
				node = node->right;
			}
			return node;
		}

		// find the successor of a given node
		NodePtr successor(NodePtr x) {
			// if the right subtree is not null,
			// the successor is the leftmost node in the
			// right subtree
			if (x->right != TNULL) {
				return minimum(x->right);
			}

			// else it is the lowest ancestor of x whose
			// left child is also an ancestor of x.
			NodePtr y = x->parent;
			while (y != TNULL && x == y->right) {
				x = y;
				y = y->parent;
			}
			return y;
		}

		// find the predecessor of a given node
		NodePtr predecessor(NodePtr x) {
			// if the left subtree is not null,
			// the predecessor is the rightmost node in the 
			// left subtree
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

		// rotate left at node x
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

		// rotate right at node x
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

		// insert the key to the tree in its appropriate position
		// and fix the tree
		NodePtr insert(T key, bool *check = NULL) {
			// Ordinary Binary Search Insertion
			NodePtr node = new Node;
			node->parent = nullptr;
			node->data = alloc.allocate(1);
			alloc.construct(node->data, key);
			node->left = TNULL;
			node->right = TNULL;
			node->color = 1; // new node must be red

			NodePtr y = nullptr;
			NodePtr x = this->root;

			while (x != TNULL) {
				y = x;
				if (cmp(*node->data, *x->data)) {
					x = x->left;
				} else if (cmp(*x->data, *node->data)) {
					x = x->right;
				} else {
					if (check) *check = true;
					return x;
				}
			}
			if (check) *check = false;
			_size++;
			// y is parent of x
			node->parent = y;
			if (y == nullptr) {
				root = node;
			} else if (cmp(*node->data, *y->data)) {
				y->left = node;
			} else {
				y->right = node;
			}

			// if new node is a root node, simply return
			if (node->parent == nullptr){
				node->color = 0;
				return node;
			}

			// if the grandparent is null, simply return
			if (node->parent->parent == nullptr) {
				return node;
			}

			// Fix the tree
			fixInsert(node);
			return node;
		}

		NodePtr getRoot(){
			return this->root;
		}

		// delete the node from the tree
		void deleteNode(T data) {
			deleteNodeHelper(this->root, data);
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


	iterator begin() { return iterator(TNULL, root); }
	const_iterator begin() const { return const_iterator(TNULL, root); }
	iterator end() { return iterator(TNULL, TNULL, root); }
	const_iterator end() const { return iterator(TNULL, TNULL, root); }

	size_t size() const { return _size; }
	bool empty() const { return _size == 0; }

	ft::pair<iterator, bool> insertAndReturnIterator(T v) {
		bool check;
		NodePtr n = this->insert(v, &check);
		return ft::make_pair(iterator(TNULL, n, root), check);
	}

	iterator get(T v) { return iterator(TNULL, getNode(v), root); }
	NodePtr getNode(T v) {
		NodePtr ret = root;
		while (ret != TNULL) {
			if (cmp(v, *ret->data))
				ret = ret->left;
			else if (cmp(*ret->data, v))
				ret = ret->right;
			else
				break ;
		}
		return ret;
	}

	void erase(iterator pos) { deleteNode(*pos._node->data); }

	void swap(_red_black_tree_& t) {
		ft::swap(t.alloc, alloc);
		ft::swap(t.cmp, cmp);
		ft::swap(t.root, root);
		ft::swap(t.TNULL, TNULL);
		ft::swap(t._size, _size);
	}
};

}