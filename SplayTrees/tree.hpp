#include "node.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;
class BSTree {
    node* root;
    int size;

    node* create_node(int num, node* parent) {
        node* n = (node*) malloc( sizeof(node) );
        n->element = num;
        n->parent = parent;
        n->right = NULL;
        n->left = NULL;
        return n;
    }

    bool search(node* curr, int num) {
        if (curr == NULL) {
            return false;
        }
        if (num == curr->element) {
            restructure(curr);
            return true;
        }

        if (num < curr->element) {
            return search(curr->left, num);
        }
        return search(curr->right, num);
    }

    node* search_node(node* curr, int num) {
        if (num == curr->element) {
        	restructure(curr);
            return curr;
        }

        if (num < curr->element) {
            if (curr->left != NULL) {
                return search_node(curr->left, num);
            }
        }
        if (curr->right != NULL) {
            return search_node(curr->right, num);
        }
        return curr;
    }

    public:
    BSTree() {
        root = NULL;
        size = 0;
    }

    bool search(int num) {
        return search(root, num);
    }

    bool insert(int num) {
        if (root == NULL) {
            root = create_node(num, NULL);
            size++;
            return true;
        } else {
            node* parent = search_node(root, num);
            if (parent->element != num) {
                node* newest = create_node(num, parent);
                if (parent->element < num) {
                    parent->right = newest;
                } else {
                    parent->left = newest;
                }
                restructure(newest);
                size++;
                return true;
            }
        }
        return false;
    }

    bool remove(int num) {
		// tree is empty return 
      	if (isEmpty()) {
        	return false;
      	}

	  	// use search_node method to splay up the node and set it as rem_node, return false if not equal to num
      	node* rem_node = search_node(root, num);
      	if (rem_node->element != num) {
       		return false;
      	}

		// get amount of children of rem_node by checking if both arent null
      	int children = 0;
      	if (rem_node->right) {
        	children = 1;
     	}
      	if (rem_node->left) {
        	if (children == 1) {
          		children = 2;
        	} else {
          	children = -1;
        	}
      	}

		// if there are no children
      	if (children == 0) {
			// get its parent and set to null
        	node* parent = rem_node->parent;
        	if (!parent) {
          		root = NULL;
        	} else {
          		if (rem_node == parent->left) {
            		parent->left = NULL;
          		} else {
            		parent->right = NULL;
          		}
        	}

			// free rem_node and decrement size
        	free(rem_node);
        	size--;

      	}
		// if it either has left or right child
		else if (children == -1 || children == 1) {

			// get nodes parent and child
			node* parent = rem_node->parent;
			node* child;
			if (children == -1) {
				child = rem_node->left;
			} else {
				child = rem_node->right;
			}

			// set the child's parent to the node's parent
			child->parent = parent;
			if (!parent) {
				root = child;
			} else {
				if (parent->left == rem_node) {
					parent->left = child;
				} else {
					parent->right = child;
				}
			}

			// free rem_node and decrement size
			free(rem_node);
			size--;
    	} 
		// if it has both children
	  	else {
			// get its right child to get the rightmost node
        	node* right_st = rem_node->right;
        	while (right_st->left != NULL) {
          		right_st = right_st->left;
        	}
			
			//store the right_st's element to temp to remove it and assign it to the node to be removed
        	int temp = right_st->element;
        	remove(temp);
        	rem_node->element = temp;
      	}
      return true;
    }

    void zigleft(node* curr) {
		// set node x as parent of curr and y as parent of x
        node* y = curr->parent;
        node* x = y->parent;
        
		// set root as curr if y is root, else if x->left is y, set x->left as curr, do the opposite if otherwise
        if(y == root){
            root = curr;
        }else if(x->left == y){
            x->left = curr;
        }else{
            x->right = curr;
        }
        
		// set curr->parent as y->parent and y->right as curr->left and if curr has left, set its parent to y
        curr->parent = y->parent;
        y->right = curr->left;
        if(curr->left){
            curr->left->parent = y;
        }

		// set curr->left to y and y->parent as curr
        curr->left = y;
        y->parent = curr;
    }

    void zigright(node* curr) { // same as zigleft but interchange pointers on 3rd part
        node* y = curr->parent;
        node* x = y->parent;
        
        if(y == root){
            root = curr;
        }else if(x->right == y){
            x->right = curr;
        }else{
            x->left = curr;
        }
        
        curr->parent = y->parent;
        y->left = curr->right;
        if(curr->right){
            curr->right->parent = y;
        }
        curr->right = y;
        y->parent = curr;
    }

    void restructure(node* child) {
		// get nodes par and gp 
        node* par;
        if(child->parent){
            par = child->parent;
        }
        
        bool ptoc_right = false;
        if (par->right == child) {
            ptoc_right = true;
        }

        node* gp;
        if(par->parent){
            gp = par->parent;
        }
		// if gp is null, check if par is left or right child to perform either zigleft or zigright
        else{
            if(par->left == child){
                zigright(child);
                cout<<"ZIGRIGHT"<<endl;
            }
            else{
                zigleft(child); 
                cout<<"ZIGLEFT"<<endl;
            }
            return;
        }

        bool gtop_right = false;
        if (gp->right == par) {
            gtop_right = true;
        }

	
		// configure cases for zigzig(left/right) and zigzag(left/right)
		if (gtop_right && ptoc_right) {
			zigleft(par);
			zigleft(child);
			
			cout<<"ZIGZIGLEFT"<<endl;
			if(child != root) restructure(child);
		}

		else if (gtop_right && !ptoc_right) {
			zigright(child);
			zigleft(child);
			
			cout<<"ZIGZAGLEFT"<<endl;
			if(child != root) restructure(child);
		}

		else if (!gtop_right && !ptoc_right) {
			zigright(par);
			zigright(child);
			
			cout<<"ZIGZIGRIGHT"<<endl;
			if(child != root) restructure(child);
		}

		else {
			zigleft(child);
			zigright(child);
			
			cout<<"ZIGZAGRIGHT"<<endl;
			if(child != root) restructure(child);
		}

		return;
    }


    // WARNING. Do not modify the methods below.
    // Doing so will nullify your score for this activity.

    bool isEmpty() {
        return size == 0;
    }
    
    void print() {
		cout << "Size: " << size << endl;
		if (!root) {
			cout << "EMPTY" << endl;
			return;
		}
		node* curr = root;
		print_node("", root, false);
			cout << "Status: " << check_parent(root, NULL) << endl;
    }

    void print_node(string prefix, node* n, bool isLeft) {
		cout << prefix;
			cout << (isLeft ? "+--L: " : "+--R: " );
			cout << n->element << endl;
		if (n->left) {
			print_node(prefix + "|   ", n->left, true);
		}
		if (n->right) {
			print_node(prefix + "|   ", n->right, false);
		}
    }

    bool check_parent(node* curr, node* par) {
        if (!curr) {
            return true;
        }
        if (curr->parent != par) {
            if (!curr->parent) {
                cout << "Illegal parent of " << curr->element << ": NULL -- must be " << par->element << endl;
            } else if (!par) {
                cout << "Illegal parent of " << curr->element << ": " << curr->parent->element << "must be NULL" << endl;
            } else {
                cout << "Illegal parent of " << curr->element << ": " << curr->parent->element << " -- must be " << par->element << endl;
            }
            return false;
        }
        return check_parent(curr->left, curr) && check_parent(curr->right, curr);
    }
};