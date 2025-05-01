#include "mybinarytree.hpp"

class BST {
	BinaryTree* tree = new MyBinaryTree();

	public:
	bool search(int num) {
		return search_node(tree->getRoot(), num);
	}

	bool search_node(node* n, int num) {
		if (n == NULL) {
			return false;
		}
		if (n->elem == num) {
			return true;
		}
		if (num > n->elem) {
			// proceed to right
			return search_node(n->right, num);
		} else {
			return search_node(n->left, num);
		}
	}

	node* insert(int num) {
		node* n = tree->getRoot();
		if (n == NULL) {
			return tree->addRoot(num);
		}
		return insert_node(n, num);
	}

	node* insert_node(node* n, int num) {
		if (n == NULL) {
			return NULL;
		}
		if (n->elem == num) {
			return NULL;
		}
		if (num > n->elem) {
			if (!n->right) {
				return tree->addRight(n, num);
			} else {
				return insert_node(n->right, num);
			}
		} else {
			if (!n->left) {
				return tree->addLeft(n, num);
			} else {
				return insert_node(n->left, num);
			}
		}
	}

    bool remove(int num) {
        return remove_node(tree->getRoot(), num);
    }

	bool remove_node(node* n, int num) {
		if (n == NULL) {
			return false;
		}
		if (n->elem == num) {
            if (n->left && n->right) {
                node* r = n->right;
                while (r->left) {
                    r = r->left;
                }
                int rem = tree->remove(r);
                n->elem = rem;
            } else {
    			tree->remove(n);
            }
            return true;
		}
		if (num > n->elem) {
			return remove_node(n->right, num);
		} else {
			return remove_node(n->left, num);
		}
	}

    // GIVEN the grandparent (or z), find the parent (or y), and the child (or x).
    bool restructure(node* gp) {
        node* par; // parent
        // TODO find parent
        if(gp->left && gp->right){
            par = gp->left;
            if(gp->left->height() < gp->right->height()) par = gp->right;
            if(gp->left->height() == gp->right->height()){
                par = gp->right;
            }
        }
        else if(!gp->right){
            par = gp->left;
        } 
        else if(!gp->left){
            par = gp->right;
        }
        
        
        // This is an indicator of the placement of grandparent to parent (gtop)
        bool gtop_right = false;
        if (gp->right == par) {
            gtop_right = true;
        }

        // TODO find child
        node* child;
        if(par->left && par->right){
            child = par->left;
            if(par->left->height() < par->right->height()) child = par->right;
            if(par->left->height() == par->right->height()){
                if(par == gp->right){
                    child = par->right;
                } 
                else{
                    child = par->left;
                }
            }
        }
        else if(!par->right){
            child = par->left;
        } 
        else if(!par->left){
            child = par->right;
        }
    
        // This is an indicator of the placement of parent to child (ptoc)
        bool ptoc_right = false;
        if (par->right == child) {
            ptoc_right = true;
        }

        // FOR THE FOLLOWING: Write in each of the if statements a console output
        // on its corresponding operation (ZIGLEFT, ZIGRIGHT, ZIGZAGLEFT, or ZIGZAGRIGHT)

        // z
        //  \
        //   y
        //    \
        //     x
        if (gtop_right && ptoc_right) {
            // TODO call to either zigleft or zigright or both
            zigleft(par);
            cout<<"ZIGLEFT"<<endl;
        }

        // z
        //   \
        //     y
        //    /
        //   x
        else if (gtop_right && !ptoc_right) {
            // TODO call to either zigleft or zigright or both

            zigright(child);
            zigleft(child);
            cout<<"ZIGZAGLEFT"<<endl;
        }

        //     z
        //    /
        //   y
        //  /
        // x
        else if (!gtop_right && !ptoc_right) {
            // TODO call to either zigleft or zigright or both
            zigright(par);
            cout<<"ZIGRIGHT"<<endl;
        }

        //      z
        //    /
        //  y
        //   \
        //    x
        else {
            // TODO call to either zigleft or zigright or both
            zigleft(child);
            zigright(child);
            cout<<"ZIGZAGRIGHT"<<endl;
        }

        return true;
    }

    void zigleft(node* curr) {
        tree->zigleft(curr);
    }

    void zigright(node* curr) {
        tree->zigright(curr);
    }

	void print() {
		tree->print();
	}
};