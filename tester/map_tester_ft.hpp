#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "../includes/map.hpp"
#include "progressBar.hpp"

// data structure that represents a node in the tree
struct Node {
	int data; // holds the key
	Node *parent; // pointer to the parent
	Node *left; // pointer to left child
	Node *right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
};

typedef Node *NodePtr;

// class RBTree implements the operations in Red Black Tree
class RBTree {
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

	void preOrderHelper(NodePtr node) {
		if (node != TNULL) {
			std::cout<<node->data<<" ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		} 
	}

	void inOrderHelper(NodePtr node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			std::cout<<node->data<<" ";
			inOrderHelper(node->right);
		} 
	}

	void postOrderHelper(NodePtr node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			std::cout<<node->data<<" ";
		} 
	}

	NodePtr searchTreeHelper(NodePtr node, int key) {
		if (node == TNULL || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		} 
		return searchTreeHelper(node->right, key);
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

				if (!s->left || !s->right) return ;
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

	void deleteNodeHelper(NodePtr node, int key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL){
			if (node->data == key) {
				z = node;
				break ;
			}

			if (node->data < key) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			std::cout<<"Couldn't find key in the tree"<<std::endl;
			return;
		}

		int l = z->left->data;
		int r = z->right->data;

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
		
		std::cout << "l: " << l << "; r: " << r << std::endl;

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

	void printHelper(NodePtr root, std::string indent, bool last) {
		// print the tree structure on the screen
	   	if (root != TNULL) {
		   std::cout<<indent;
		   if (last) {
		      std::cout<<"R----";
		      indent += "     ";
		   } else {
		      std::cout<<"L----";
		      indent += "|    ";
		   }
            
           std::string sColor = root->color?"RED":"BLACK";
		   std::cout<<root->data<<"("<<sColor<<")"<<std::endl;
		   printHelper(root->left, indent, false);
		   printHelper(root->right, indent, true);
		}
		// cout<<root->left->data<<endl;
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		TNULL->data = 123;
		root = TNULL;
		_size = 0;
	}

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

	// search the tree for the key k
	// and return the corresponding node
	NodePtr searchTree(int k) {
		return searchTreeHelper(this->root, k);
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
	void insert(int key, std::ofstream& __out) {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->data = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			} else if (node->data > x->data) {
				x = x->right;
			} else
				return ;
		}
		__out << "inserting: " << key << std::endl;
		_size++;
		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->data < y->data) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	NodePtr getRoot(){
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void prettyPrint() {
	    if (root) {
    		printHelper(this->root, "", true);
	    }
	}

};

class MapTesterFt {
public:

	RBTree control;

	typedef ft::map<int, std::string>		t_map;
	typedef ft::pair<int, std::string>		t_pair;
	typedef t_map::iterator					iterator;
	typedef t_map::const_iterator			const_iterator;
	typedef t_map::const_reverse_iterator	const_reverse_iterator;

	enum constructor { DEFAULT, COPY, RANGE };

	MapTesterFt(): control(), method(DEFAULT), size(100), headers(false), _stop(false), __out("ft.txt") {}
	~MapTesterFt() { 
		std::cerr << "deleting..." << std::endl;
		delete cnt;
		std::cerr << "deleted" << std::endl;
		if (_stop) {
			//__out << "check for leaks now, press enter to end test" << std::endl;
			getchar();
		}
	}

	MapTesterFt& withIterators() { method = RANGE; return *this; }
	MapTesterFt& withHeaders() {  headers = true; return *this; }
	MapTesterFt& withStop() {  _stop = true; return *this; }
	MapTesterFt& withCopy() { method = COPY; return *this; }
	MapTesterFt& withSize(size_t s) {
		size = s;
		if (size < 10) {
			__out << "size set to 10 (minimum)" << std::endl;
			size = 10; }
		return *this;
	}

	void execute(std::string testName) {
		
		printHeader("Construction");
		construct_map();
		printHeader("Construction completed");
		printMap();
		for (iterator it = cnt->begin(); it != cnt->end(); it++)
			__out << "(" << it->first << ":" << it->second << ")" << std::endl;

		if (testName == "" || testName.find("capacity") != std::string::npos) { printHeader("capacity"); test_capacity(); }
		if (testName == "" || testName.find("element_access") != std::string::npos) { printHeader("element_access"); test_element_access(); }
		if (testName == "" || testName.find("operator_equal") != std::string::npos) { printHeader("operator_equal"); test_equal_operator(); }
		if (testName == "" || testName.find("iterators") != std::string::npos) { printHeader("iterators"); test_iterators(); }
		if (testName == "" || testName.find("erase") != std::string::npos) { printHeader("erase"); test_erase(); }
		if (testName == "" || testName.find("swap") != std::string::npos) { printHeader("swap"); test_swap(); }
	}

	iterator getRandomIterator(int chance = 1) {
		size_t pos = generateNumber(cnt->size());
		iterator ret = cnt->begin();

		for (int i = 0; i < pos; i++)
			ret++;
		return ret;
	}

private:

	constructor method;
	size_t size;
	t_map *cnt;
	bool headers;
	bool _stop;
	std::ofstream __out;
	
	void error(std::string str) { __out << "[ERROR]: " << str << std::endl; }
	void printMap() { 
		for (iterator it = cnt->begin(); it != cnt->end(); it++) __out << it->first << it->second; __out << std::endl;
	}
	void printHeader(std::string testName) { if (headers) __out << "[TEST]: " << testName << std::endl; }
	int generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
	std::string generateWord(size_t s = 50) const {
		std::string abc = "abcdefghijklmnopqrstuvwxyz1234567890";
		std::string ret = "";
		for (int i = 0; i < s; i++)
			ret += abc.c_str()[generateNumber(abc.length())];
		return ret;
	}

	void construct_map() {
		switch (method)
		{
		case COPY:
			construct_map_copy();
			break;
		case RANGE:
			construct_map_iterator();
			break;
		default:
			construct_map_default();
			break;
		}
	}
	
	void construct_map_default() {
		__out << "default map constructor" << std::endl;
		std::cerr << "default map constructor" << std::endl;
		cnt = new t_map();

		ProgressBar *bar = new ProgressBar();
		for (int i = 0; i < size; i++) {
			bar->setProgress(i, size);
			(*cnt)[generateNumber()] = generateWord();
		}
		delete bar; std::cerr << std::endl;
	}

	void construct_map_iterator() {
		__out << "range map constructor" << std::endl;
		std::cerr << "range map constructor" << std::endl;
		std::vector<t_pair> v;
		std::cerr << "filling vector" << std::endl;
		ProgressBar *bar = new ProgressBar();
		cnt = new t_map();
		__out << "===========" << std::endl;
		for (int i = 0; i < size; i++) {
			if (i == 68)
				std::cout << "again" << std::endl;
			bar->setProgress(i, size);
			int number = generateNumber();
			if (number == 9713)
				std::cout << "missing" << std::endl;
			std::string word = generateWord();
			//v.push_back(t_pair(number, word));
			control.insert(number, __out);
			cnt->insert(ft::make_pair(number, word));
		}
		__out << "===========" << std::endl;
		delete bar; std::cerr << std::endl;
		std::cerr << "calling range iterator (without loading bar)" << std::endl;

	}

	void construct_map_copy() {
		__out << "copy map constructor" << std::endl;
		std::cerr << "copy map constructor" << std::endl;
		std::cerr << "calling default map constructor (to be copied)" << std::endl;
		construct_map_default();
		t_map *aux = cnt;
		std::cerr << "calling copy constructor" << std::endl;
		cnt = new t_map(*aux);
		std::cerr << "destroying temporary map (to be copied)" << std::endl;
		delete aux;
	}

	void test_equal_operator() {
		t_map newMap = *cnt;
		
		__out << "operator= test" << std::endl;
		std::cerr << "operator= test" << std::endl;
		const_iterator it1 = newMap.begin();
		const_iterator it2 = cnt->begin();
		for (; it1 != newMap.end(); it1++)
		{
			__out << it1->first << it2->first << "|" << std::endl;
			if (it1->first != it2->first || it1->second != it2->second)
				error("after using operator= iterator contents doesnt match");
			it2++;
		}
		//if (it1 != it2)
		if (it2 != cnt->end())
			error("after looping both maps, both iterators are not equal to map.end()");
	}

	void test_iterators() {
		__out << "iterators test" << std::endl;
		std::cerr << "iterators test" << std::endl;
		iterator it = cnt->begin();
		std::string diff = "-----";
		size_t i = 0;
		size_t s = cnt->size();

		std::cerr << "iterating through map" << std::endl;
		ProgressBar *bar = new ProgressBar();
		for (const_iterator cit = cnt->begin(); cit != cnt->end(); cit++) {
			bar->setProgress(i++, s);
			__out << cit->first << ":" << cit->second << std::endl;
		}
		delete bar; std::cerr << std::endl;
		
		it->second = diff;
		it++;
		it++;
		it++;
		it->second = diff;
		it = cnt->end();
		it--;
		it--;
		it->second = diff;

		std::cerr << "reverse iterating through map" << std::endl;
		bar = new ProgressBar();
		i = 0;
		for (const_reverse_iterator rit = cnt->rbegin(); rit != cnt->rend(); rit++) {
			bar->setProgress(i++, s);
			__out << rit->first << ":" << rit->second << std::endl;
		}
		delete bar; std::cerr << std::endl;
	}

	void test_capacity() {
		t_map emptyMap;
		__out << "Capacity test" << std::endl;
		std::cerr << "Capacity test" << std::endl;

		__out << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
		__out << cnt->empty() << ", " << cnt->size() << std::endl;
	}

	void test_element_access() {
		size_t max_extra_elements = cnt->size() / 10;
		size_t extra_elements = 0;
		__out << "test element access" << std::endl;
		std::cerr << "test element access" << std::endl;
		ProgressBar *bar = new ProgressBar();
		std::string str = "+++++";
		for (int i = 0; i < size; i++) {
			bar->setProgress((float)i / (float)size);
			int n = generateNumber(size);

			try { (*cnt).at(n).append("foundWithAt"); }
			catch(const std::exception& e) {
				if (extra_elements < max_extra_elements) {
					(*cnt)[n] = str;
					(*cnt).at(n).append("notFoundWithAt");
					extra_elements++;
				}
			}
		}
		delete bar; std::cerr << std::endl;
	}

	//modifiers tested implicitly
	void test_erase() {
		size_t mapSize = cnt->size();

		/*control.inorder(); std::cout << std::endl;
		for (iterator it = cnt->begin(); it != cnt->end(); it++)
			std::cout << it->first << " ";*/
		std::cout << std::endl;

		__out << "Test erase: (" << mapSize << ")" << std::endl;
		std::cerr << "Test erase: (" << mapSize << ")" << std::endl;
		size_t n = cnt->size() / 4;

		std::cerr << "Erase single elements" << std::endl;
		//ProgressBar *bar = new ProgressBar();
		for (int i = 0; i < n; i++) {
			//bar->setProgress(i, n);
			iterator it = getRandomIterator();
			control.deleteNode(it->first);
			cnt->erase(it);
			int key = generateNumber();
			//__out << "deleting by key: " << key << std::endl;
			//__out << cnt->erase(key);
			//if (!(i % 20))
		}
		//delete bar; std::cerr << std::endl;
		__out << std::endl;

		iterator it1 = getRandomIterator();
		iterator it2 = getRandomIterator();
		iterator first, last;
		if (it1->first < it2->first)
		{
			first = it1;
			last = it2;
		} else {
			first = it2;
			last = it1;
		}
		std::cerr << "print map" << std::endl;
		printMap();
		std::cerr << "iterator erase" << std::endl;
		for (iterator i1 = first; i1 != last; i1++)
		{
			//else
				//std::cout << i1->first << std::endl;
			if (i1->second == "bfayy9ndgsiy6ywlyomkss27zr3wmnrrtd4o3qxdd31q8h9f3w")
				std::cout << i1->first << ":" << i1->second << std::endl;
			control.deleteNode(i1->first);
		}
		/*control.inorder(); std::cout << std::endl;
		for (iterator it = cnt->begin(); it != cnt->end(); it++)
			std::cout << it->first << " ";*/
		std::cout << std::endl;
		cnt->erase(first, last);
		std::cerr << "print map" << std::endl;
		printMap();
		std::cerr << std::endl;
	}

	void test_swap() {
		__out << "Test swap" << std::endl;
		std::cerr << "Test swap" << std::endl;
		t_map mapa;
		for (int i = 0; i < size / 2; i++)
			mapa[generateNumber()] = generateWord();

		cnt->swap(mapa);
		void *ptr = cnt;
		printMap();
		if (ptr != cnt)
			error("swap modified the memory pointer of the map somehow!!!");
		mapa.swap(*cnt);
		printMap();
	}
	
	/*
	// observers:
	//allocator_type get_allocator() const
	//key_compare    key_comp() const
	//value_compare  value_comp()    const

	// map operations:
	//iterator find(const key_type& k)
	//const_iterator find(const key_type& k) const

	//size_type      count(const key_type& k)
	
	//iterator lower_bound(const key_type& k)
	//const_iterator lower_bound(const key_type& k) const

	//iterator upper_bound(const key_type& k)
	//const_iterator upper_bound(const key_type& k) const

	//pair<iterator,iterator>             equal_range(const key_type& k)
	//pair<const_iterator,const_iterator> equal_range(const key_type& k) const

	*/


};