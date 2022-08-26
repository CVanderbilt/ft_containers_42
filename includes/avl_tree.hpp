#ifndef AVL_TREE_HPP
# define AVL_TREE_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <algorithm>
# include <utility>
# include "iterator.hpp"

//# define DEBUG

namespace ft
{
	template <class T=int, class Compare = std::less<T> >
	class avl_tree {

	//todo: cambiar las rotaciones a privadas, en vez de devolver r modificar r (interno)

	private:

		struct avl {
			T d;
			struct avl *l;
			struct avl *r;
			struct avl *p;
			bool isRight;
		};

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

		/*
		*	Right rotation:
		*		- Left child (subtree) as root
		*		- Root as right child of new root (previously right child)
		*		- Righ child of new root as left child of new right child
		*/

		/*
		*	grandparent -> x
		*	parent -> y
		*	child -> z
		*/
		avl *rr_rotat(avl *parent) {
			avl *t;
			/*

		    p                                               
          /  \                              
		 /    \                              
       p->l   p->r                         t                 
               /\                          /\             
              /  \                        /  \            
            t->l t->r                   t->l t->r    
                 
		    p                t                                
          /  \              / \              
		 /    \            /   \              
       p->l   t->l        p   t->r

                              
               t               
              / \               
             /   \              
		    p    t->r                                           
          /  \                           
		 /    \                    
       p->l   t->l          

	   //changes in pointer to parents                                  
		t->l.p = p
		t.p = p.p
		p.p = t
                                                      
			*/
			
			t = parent->r;
			parent->r = t->l;
			t->l = parent;
			//changes in pointer to parents
			t->p = parent->p;
			parent->p = t;
			avl *old_tl = parent->r;
			if (old_tl)
				old_tl->p = parent;
			#ifdef DEBUG
			std::cout<<"Right-Right Rotation";
			#endif
			return t;
		}

		avl *ll_rotat(avl *parent) {
			avl *t;
			/*

		    p                                               
          /  \                              
		 /    \                              
       p->l   p->r                   t                 
        /\                          /\             
       /  \                        /  \            
     t->l t->r                   t->l t->r    
                 
		    p                t                                
          /  \              / \              
		 /    \            /   \              
       t->r   p->r       t->l   p

                              
               t               
              / \               
             /   \              
		   t->l   p                                              
                /  \                           
		       /    \                    
             t->r   p->r          

	   //changes in pointer to parents                                  
		t->r.p = p
		t.p = p.p
		p.p = t
                                                      
			*/

			t = parent->l;
			parent->l = t->r;
			t->r = parent;

			//changes in pointer to parents
			t->p = parent->p;
			parent->p = t;
			avl *old_tr = parent->l;
			if (old_tr)
				old_tr->p = parent;
			#ifdef DEBUG
			std::cout<<"Left-Left Rotation";
			#endif
			return t;
		}

		avl *lr_rotat(avl *parent) {
			avl *t;
			t = parent->l;
			parent->l = rr_rotat(t);
			#ifdef DEBUG
			std::cout<<"Left-Right Rotation";
			#endif
			return ll_rotat(parent);
		}

		avl *rl_rotat(avl *parent) {
			avl *t;
			t = parent->r;
			parent->r = ll_rotat(t);
			#ifdef DEBUG
			std::cout<<"Right-Left Rotation";
			#endif
			return rr_rotat(parent);
		}

		//esto solo balancea un nodo
		//TODO: hacer que además vaya subiendo hasta root balanceando todos los nodos
		/*
		avl * balance(avl *t) {
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
		}*/
/*
	avl *insert_rec(avl *r, int v) {
   if (r == NULL) {
      r = new avl;
      r->d = v;
      r->l = NULL;
      r->r = NULL;
      return r;
   } else if (v< r->d) {
      r->l = insert_rec(r->l, v);
      r = balance_rec(r);
   } else if (v >= r->d) {
      r->r = insert_rec(r->r, v);
      r = balance_rec(r);
   } return r;
}*/

	avl *balance_rec(avl *t) {
		T nodeVal;
		if (t) {
			nodeVal = t->d;
			std::cout << "Balancing: " << t->d << "; " << t;
		} else {
			std::cout << "Balancing NULL???";
		}
		int bal_factor = difference(t);
		bool didChanges = false;
		if (bal_factor > 1) {
			didChanges = true;
			if (difference(t->l) > 0) {
				std::cout << " llrotat feo aqui " << t << std::endl;
				t = ll_rotat(t);
			}
			else
				t = lr_rotat(t);
		} else if (bal_factor < -1) {
			didChanges = true;
			if (difference(t->r) > 0)
				t = rl_rotat(t);
			else
				t = rr_rotat(t);
		}
		if (didChanges) {
			std::cout << "  >>>with changes<<<" << std::endl;

			std::string nodeSig = "balancedNode(" + std::to_string(nodeVal) + ")";
			printBT(nodeSig, t, true);
		}
		else
			std::cout << "  >>>no changes<<<" << std::endl;
		return t;
	}

	avl *balance_iter(avl *t) {
		std::cout << "iter balancing ";

		if (t)
			std::cout << t->d;
		else
			std::cout << "NULL";

		std::cout << std::endl;
		avl *aux = t;
		while (t) {
			//std::string nodeSig = "balancedNode(" + std::to_string(t->d) + ")";
			avl *parent = t->p;
			bool isLeft = false;
			if (parent)
				isLeft = parent->l == t;
			t = balance_rec(t); //a lo mejor esto no funciona y tenemos que usar dobles punteros o algo
			//t es el nodo balanceado
			//lo seteamos como hijo de su padre (guardado previamente)
			if (parent) {
				if (isLeft)
					parent->l = t;
				else
					parent->r = t;
				std::cout << "set new node as " << (isLeft ? "left" : "right") << "child of " << parent->d << std::endl;
			}
			printBT("outOfRec", t, true);
			aux = t;
			t = t->p;
		}
		//t = aux;

		return aux;
	}

	void printBT(const std::string& prefix, const avl* node, bool isLeft) {
    //std::cout << "printBT" << std::endl;
	//getchar();
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

        // print the value of the node
        std::cout << "v(" <<node->d << ')';

		//if (difference(node) < -1 || difference(node) > 1)
			std::cout << " [diff (" << difference(node) << ")]";

		std::cout << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->l, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->r, false);
    }
}

		void balance(avl *t) {
			std::cout << "===========================================" << std::endl;
			std::cout << "===========================================" << std::endl;
			std::cout << "===========================================" << std::endl;
			std::cout << "rebalancing (" << t->d << ")" <<std::endl;
			std::cout << "before rebalancing" << std::endl;
			this->printBT("", r, false);
			avl *lastNode = NULL;
			while (t) {
				std::cout << "node: " << t->d << "; ";
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
				} else {
					std::cout << "no rebalancing needed" << std::endl;
				}

				lastNode = t;
				t = t->p;
			}
			if (lastNode && r)
				std::cout << "lastNode: " << lastNode->d << "; root: " << r->d << std::endl;
			else if (lastNode && !r)
				std::cout << "lastNode: " << lastNode->d << "; root: NULL" << std::endl;
			else
				std::cout << "lastNode: " << lastNode << "; root: " << r << std::endl;
			getchar();
			r = lastNode;
			this->printBT("", r, false);
		}

		//TODO: cambiar inserción recursiva a iterativa
		//- cambiar balancing a iterative balancing
		avl * insert_rec(avl *r, T v, avl *parent) {
			if (r == NULL) {
				std::cout << "---------> inserted " << v << std::endl;
				r = new avl; //todo replace using allocator instead
				r->d = v;
				r->l = NULL;
				r->r = NULL;
				r->p = parent;
				_size++;
				return r;
			//} else if (v < r->d) {
			} else if (_cmp(v, r->d)) {
				r->l = insert_rec(r->l, v, r);
				//printBT("preBalance", this->r, true);
				std::cout << "would balance " << r->d << std::endl;
				//r = balance_rec(r);
			//} else if (v >= r->d) {
			} else if (v != r->d) {
				//Si son iguales no hacemos nada porq ya está insertado el elemento
				r->r = insert_rec(r->r, v, r);
				//printBT("preBalance", this->r, true);
				std::cout << "would balance " << r->d << std::endl;
				//r = balance_rec(r);
			} else {
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
				std::cout << "IGUALES!!!!!!" << std::endl;
			}
			return r;
		}

// pass the root node of your binary tree
//printBT(root);

		T get(T target, struct avl *node, std::string msg) {
			if (!node) {
				#ifdef DEBUG
				std::cout << "not found, returning null" << std::endl;
				#endif
				return (T)NULL; // revisar como manda esto, seguramente mejor con una excepción
			}
			#ifdef DEBUG
			std::cout << "    " << msg << ": comparing >" << target << "< with node value>" << node->d << "<" << std::endl;
			#endif
			if (_cmp(target, node->d)){
				return get(target, node->l, "searching in left child");
			}
			if (target == node->d) {
				#ifdef DEBUG
				std::cout << "found node with value " << target << std::endl;
				#endif
				return node->d;
			}
			return get(target, node->r, "searching in right child");
		}
		//TODO: los métodos públicos de imprimir los borrareos
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

		void inorder(avl *t) {
			if (t == NULL)
				return;
			inorder(t->l);
			std::cout << t->d << " ";
			inorder(t->r);
		}

		void preorder(avl *t) {
			if (t == NULL)
				return;
			std::cout << t->d << " ";
			preorder(t->l);
			preorder(t->r);
		}

		void postorder(avl*t) {
			if (t == NULL)
				return;
			postorder(t->l);
			postorder(t->r);
			std::cout << t->d << " ";
		}

		//TODO: ver si hace falta (probablemente si) y añadir copy constructor etc
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

		void inorder() { inorder(r); }
		void preorder() { preorder(r); }
		void postorder() { postorder(r); }
		void show() {
			if (!r)
               std::cout << "Tree is Empty" << std::endl;
			else
				show(r, 1);

		}
		
		void panic(std::string msg) {
			for (int i = 0; i < 10; i++)
				std::cout << msg << std::endl;
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

		void insert(T v) {
			
			if (true) {
				r = insert_rec(r, v, NULL);
				printBT("beforeBalancing ", r, true);
				//buscar nodo con valor v
				avl *aux = getNode(v);
				if (!aux) panic("aux es null");
				r = balance_iter(aux);
				printBT("afterBalancing ", r, true);
				return ;
			}

			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;
			std::cout << "!!!!!!!!!!!" << std::endl;

			avl *aux = r;
			avl *parent = NULL;
			bool isRight;

			while (aux) {
				parent = aux;
				if (_cmp(v, aux->d)) {
					//left
					std::cout << "left, ";
					aux = aux->l;
					isRight = false;
				} else if (v != aux->d) {
					//right
					std::cout << "right, ";
					aux = aux->r;
					isRight = true;
				} else {
					std::cout << "already inserted" << std::endl;
					//already inserted
					aux->d = v;
					return ;
				}
			}

			aux = new avl; //todo replace using allocator instead
			aux->d = v;
			aux->l = NULL;
			aux->r = NULL;
			aux->p = parent;
			_size++;
			
			if (parent) {
				if (isRight) {
					std::cout << " set as new right child of " << parent->d << std::endl;
					parent->r = aux;
				} else  {
					std::cout << " set as new left child of " << parent->d << std::endl;
					parent->l = aux;
				}
			} else {
				std::cout << "no parent set as root" << std::endl;
				r = aux;
			}
			balance(aux);
			//getchar();
			//std::cout << "full rebalanced tree: " << std::endl;
			//this->printBT("", r, true);

/*
if (_cmp(target, node->d)){
return get(target, node->l, "searching in left child");
}
*/

		}

		size_t size() {
			return _size;
		}

		size_t max_size() {
			//TODO: implementar usando allocator max size 
			return (size_t) -1;
		}


		/*
		*	Iterator rules
		*	Inorder
		*	(All left children -> node -> right children)
		*
		*	From any node:
		*	 next -> leftmost node of right child (if right child doesnt have left children it will be the leftmost node)
		*	 if no right children next is the parent node
		*	Begin:
		*	 leftmost node
		*	End:
		*	 null; end - 1 -> rightmost node
		*	
		*/
		class Iterator:
		public ft::iterator<ft::bidirectional_iterator_tag, T> {
			
			private:
			avl *_node;

			public:
			Iterator(): _node(NULL) {}
			Iterator(avl *node): _node(node) {}
			Iterator(const Iterator& it): _node(it._node) {}
			void show() {
				std::cout << "show: " << _node << std::endl;
			}

			avl *eraseNodeWithTwoChildren() {
				std::cout << "erase node with two children" << std::endl;
				Iterator it(*this);

				avl *oldNode = _node;
				avl *newNode = (++it)._node;
				avl *parentNewNode = newNode->p;
				avl *parentOldNode = _node->p;

				std::cout << "oldNode: " << oldNode->d << std::endl;
				std::cout << "newNode: " << newNode->d << std::endl;
				std::cout << "parent newNode: " << parentNewNode->d << std::endl;

				std::cout << "replace node(" << _node->d << ") with newNode(" << newNode->d << ")" << std::endl;
				_node = newNode;
				newNode->p = oldNode->p;

				if (parentOldNode) {// si tiene padre apunta al nuevo hijo
					std::cout << "comparing parentOldNode left child (" << parentOldNode->l->d << "/" << parentOldNode->l << ") with oldNode (" << oldNode->d << "/" << oldNode << ")" << std::endl;
					if (parentOldNode->l == oldNode) {
						std::cout << "parent old node left child (" << parentOldNode->l->d << ") to newNode (" << newNode->d << ")" << std::endl;
						parentOldNode->l = newNode;
					} else {
						std::cout << "parent old node right child (" << parentOldNode->r->d << ") to newNode (" << newNode->d << ")" << std::endl;
						parentOldNode->r = newNode;
					}
				}

				//nuevo hijo no tiene hijo izq pero puede tener hijo der
				//newNode->r->d
				std::string newNodeR = newNode->r ? std::to_string(newNode->r->d) : "NULL";
				std::cout << "testLine: " << parentNewNode->l->d << std::endl;
				std::cout << "parent newNode left child from (" << parentNewNode->l->d << ") to newNode right child (" << newNodeR << ")" << std::endl;
				parentNewNode->l = newNode->r;

				std::cout << "newNode right child from (" << newNodeR << ") to oldNode right child (" << oldNode->r->d << ")" << std::endl;
				newNode->r = oldNode->r;
				newNode->r->p = newNode;
				std::cout << "newNode left child from (" << "fNULL" << ") to oldNode left child (" << oldNode->l->d << ")" << std::endl;
				newNode->l = oldNode->l;
				newNode->l->p = newNode;
				std::cout << "these 2 children set to point to newNode as father" << std::endl;
				//delete(oldNode->d); TODO: revisar esto
				delete(oldNode);

				return (parentNewNode);
				//cambiar siguiente por este (siguiente = leftmost(menor) del right child)
				//siguiente no tiene l pero puede tener r
				//  ponemos su r como l del padre
				//l y r de este son l y r del nuevo

				//borrar nodo actual (liberar _node.d y _node)
				//it = this + 1;
				//	it.node.p.l = it.node.r
				//	it.node.l/r/p = _node.l/r/p 
			}

			avl* eraseNodeWithOneChildren() {
				std::cout << "erase node with one children" << std::endl;
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
				std::cout << "erase node with no children" << std::endl;
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
				if (_node->r && _node->l) { //dos hijos
					return eraseNodeWithTwoChildren();
				} else if (!_node->r && !_node->l) { //sin hijos
					return eraseNodeWithNoChildren();
				} else { //un hijo
					return eraseNodeWithOneChildren();
				}
			}

			Iterator static begin(avl *root) { return Iterator(getLeftMost(root)); }
			Iterator static end(avl *root) { return Iterator(); }

			// comparison operators. just compare node pointers
			bool operator== (const Iterator& rhs) const { return _node == rhs._node; }
			
			bool operator!= (const Iterator& rhs) const {
				return _node != rhs._node;
			}
			
			// dereference operator. return a reference to
			// the value pointed to by nodePtr
			const T& operator* () const {
				return _node->d;
			}
			
			//don't call with null node!!!
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

/*


               A               
              / \               
             /   \              
		   B       C                                              
          /\       /\                           
		 /  \     /  \                    
        F    G   D    E          
            /\
           /  \
          H    I
*/

			avl *getNextNode(avl *node) {
				avl *aux;
				//std::cout << "getNextNode called" << std::endl;
				if (!node)
					return NULL;

				if (node->r)
					return getLeftMost(node->r);

				//std::cout << "traveling up" << std::endl;
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

			// preincrement. move forward to next larger value
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
			std::cout << "going to erase node with value " << *it << std::endl;
			//avl *r = it.erase();
			it.erase();
			std::cout << "with deletion but without rebalancing" << std::endl;
			printBT();
			std::cout << "===================================================" << std::endl;
			balance(r);
		}
	};
}


#endif