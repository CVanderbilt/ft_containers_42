#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <utility>
#include "avl_tree.hpp"

#define pow2(n) (1 << (n))
/*
struct avl {
   int d;
   struct avl *l;
   struct avl *r;
}*r;

class avl_tree {
   public:
      int height(avl *);
      int difference(avl *);
      avl *rr_rotat(avl *);
      avl *ll_rotat(avl *);
      avl *lr_rotat(avl*);
      avl *rl_rotat(avl *);
      avl * balance(avl *);
      avl * insert(avl*, int);
      void show(avl*, int);
      void inorder(avl *);
      void preorder(avl *);
      void postorder(avl*);
      avl_tree() {
         r = NULL;
      }
};
int avl_tree::height(avl *t) {
   int h = 0;
   if (t != NULL) {
      int l_height = height(t->l);
      int r_height = height(t->r);
      int max_height = std::max(l_height, r_height);
      h = max_height + 1;
   }
   return h;
}
int avl_tree::difference(avl *t) {
   int l_height = height(t->l);
   int r_height = height(t->r);
   int b_factor = l_height - r_height;
   return b_factor;
}
avl *avl_tree::rr_rotat(avl *parent) {
   avl *t;
   t = parent->r;
   parent->r = t->l;
   t->l = parent;
   std::cout<<"Right-Right Rotation";
   return t;
}
avl *avl_tree::ll_rotat(avl *parent) {
   avl *t;
   t = parent->l;
   parent->l = t->r;
   t->r = parent;
   std::cout<<"Left-Left Rotation";
   return t;
}
avl *avl_tree::lr_rotat(avl *parent) {
   avl *t;
   t = parent->l;
   parent->l = rr_rotat(t);
   std::cout<<"Left-Right Rotation";
   return ll_rotat(parent);
}
avl *avl_tree::rl_rotat(avl *parent) {
   avl *t;
   t = parent->r;
   parent->r = ll_rotat(t);
   std::cout<<"Right-Left Rotation";
   return rr_rotat(parent);
}
avl *avl_tree::balance(avl *t) {
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
avl *avl_tree::insert(avl *r, int v) {
   if (r == NULL) {
      r = new avl;
      r->d = v;
      r->l = NULL;
      r->r = NULL;
      return r;
   } else if (v< r->d) {
      r->l = insert(r->l, v);
      r = balance(r);
   } else if (v >= r->d) {
      r->r = insert(r->r, v);
      r = balance(r);
   } return r;
}
void avl_tree::show(avl *p, int l) {
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
void avl_tree::inorder(avl *t) {
   if (t == NULL)
      return;
      inorder(t->l);
      std::cout << t->d << " ";
      inorder(t->r);
}
void avl_tree::preorder(avl *t) {
   if (t == NULL)
      return;
      std::cout << t->d << " ";
      preorder(t->l);
      preorder(t->r);
}
void avl_tree::postorder(avl *t) {
   if (t == NULL)
      return;
      postorder(t ->l);
      postorder(t ->r);
      std::cout << t->d << " ";
}*/
struct avl {
   		int d;
  		struct avl *l;
   		struct avl *r;
	}*r;

int main() {
	int c, i;
   
	ft::avl_tree<int> avl;

	//*
	avl.insert(6);
	avl.insert(7);
	avl.insert(8);
	avl.insert(5);
	avl.insert(3);
	avl.insert(2);
	avl.insert(100);
	avl.insert(4);
	avl.insert(10);
	avl.insert(9);
	avl.insert(1);
	avl.insert(11);
	avl.insert(-2);

	avl.insert(-6);
	avl.insert(-7);
	avl.insert(-8);
	avl.insert(-5);
	avl.insert(-3);
	avl.insert(20);
	avl.insert(-100);
	avl.insert(-4);
	avl.insert(-10);
	avl.insert(-9);
	avl.insert(-1);
	avl.insert(-11);
	avl.insert(-20);
	//*/

	avl.printBT();
		//std::cout << *it++ << ", ";

	std::cout << std::endl;
	///*
	while (1) {
		std::cout << "0.Show size" << std::endl;
		std::cout << "1.Insert Element into the tree" << std::endl;
		std::cout << "2.show Balanced AVL Tree" << std::endl;
		std::cout << "3.InOrder traversal" << std::endl;
		std::cout << "4.PreOrder traversal" << std::endl;
		std::cout << "5.PostOrder traversal" << std::endl;
		std::cout << "6.Show as a triangle" << std::endl;
		std::cout << "7.Search value" << std::endl;
		std::cout << "8.Erase node with specific value" << std::endl;
		std::cout << "9.Loop with iterators" << std::endl;
		std::cout << "10.Exit" << std::endl;
		std::cout << "Enter your Choice: ";
		std::cin >> c;
    	switch (c) {
			case 0: {
				std::cout << "Size: " << avl.size() << std::endl;
				break;
			}
        	case 1: {
        		std::cout << "Enter value to be inserted: ";
        		std::cin >> i;
				avl.insert(i);
        		break;
			}
			case 2: {
				std::cout << "Balanced AVL Tree:" << std::endl;
				avl.show();
				std::cout<< std::endl;
				break;
			}
			case 3: {
				std::cout << "Inorder Traversal:" << std::endl;
				avl.inorder();
				std::cout << std::endl;
				break;
			}
			case 4: {
				std::cout << "Preorder Traversal:" << std::endl;
				avl.preorder();
				std::cout << std::endl;
				break;
			}
			case 5: {
				std::cout << "Postorder Traversal:" << std::endl;
				avl.postorder();
				std::cout << std::endl;
				break;
			}
			case 6: {
				std::cout << "Showing as a triangle" << std::endl;
				avl.printBT();
				break;
			}
			case 7: {
				std::cout << "Enter value to search: ";
				std::cin >> i;
				avl.printBT();
				avl.get(i);
				break;
			}
			case 8: {
				std::cout << "Enter value to erase: ";
				std::cin >> i;
				ft::avl_tree<int>::Iterator it = avl.begin();
				while (it != avl.end()) {
					if (*it == i) {
						avl.printBT();
						avl.erase(it);
						//it.erase();
						break;
					}
					it++;
					//TODO: add test de erase(it++)
				}
				avl.printBT();
				break;
			}
			case 9: {
				std::cout << "Looping with iterators: " << std::endl;
				ft::avl_tree<int>::Iterator it = avl.begin();
				while (it != avl.end()) std::cout << *it++ << " ";
				std::cout << std::endl;
				break ;
			}
        	case 10: {
            	exit(1);
			}
	    	default: {
            	std::cout << "Wrong Choice" << std::endl;
			}
      }
   }//*/
   return 0;
}