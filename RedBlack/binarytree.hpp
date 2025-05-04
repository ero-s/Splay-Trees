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
        n->is_red = true;
        return n;
    }

    bool search(node* curr, int num) {
        if (curr == NULL) {
            return false;
        }
        if (num == curr->element) {
            return true;
        }

        if (num < curr->element) {
            return search(curr->left, num);
        }
        return search(curr->right, num);
    }

    node* search_node(node* curr, int num) {
        if (num == curr->element) {
            return curr;
        }

        if (num < curr->element) {
            if (curr->left != NULL) {
                return search_node(curr->left, num);
            }
            return curr;
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
            root->is_red = false;
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
                size++;
                node* par = newest->parent;
                // CHECK for double red violation
                while (newest->is_red && par->is_red) {
                    node* sibling = NULL;
                    if (par->parent->left == par) {
                        sibling = par->parent->right;
                    } else {
                        sibling = par->parent->left;
                    }
                    // CASE 1: Sibling s of y is BLACK
                    if (sibling == NULL || sibling->is_red == false) {
                        cout << "INSERTION Violation: Case 1" << endl;
                        node* b = restructure(newest, true);
                        b->is_red = false;
                        b->left->is_red = true;
                        b->right->is_red = true;
                        break;
                    } else if (sibling->is_red == true) {
                        cout << "INSERTION Violation: Case 2" << endl;
                        sibling->is_red = false;
                        par->is_red = false;
                        if (par->parent != root) {
                            par->parent->is_red = true;
                            newest = par->parent;
                            par = newest->parent;
                        }
                    }
                }
                return true;
            }
        }
        print();
        return false;
    }
    //Case 1 -> Print restructure;

    void deleteFix(node* n){
        // set N as currNode and loop while currNode has parent and currNode isnt black
        node* currNode = n;
        while(currNode->parent && !currNode->is_red){
            node* sibling;

            // if sibling is right
            if(currNode->parent->left == currNode && currNode->parent->right){
                sibling = currNode->parent->right;
                if(sibling->is_red){
                    // if sibling is red, set color of currNode as red and sibling's as black, perform restructure passing sibling then update sibling
                    cout << "DELETION Violation: Case 3\n";
                    currNode->parent->is_red = true;
                    sibling->is_red = false;
                    restructure(sibling, false);
                    sibling = currNode->parent->right;
                }

                if(
                        (!sibling->right && !sibling->left) ||
                        (sibling->right && sibling->left && !sibling->right->is_red && !sibling->left->is_red))
                {
                    // if sibling's children are all black, set sibling as red and currNode to its parent
                    cout << "DELETION Violation: Case 2\n";
                    sibling->is_red = true;
                    currNode = currNode->parent;
                }else{
                    cout << "DELETION Violation: Case 1\n";
                    // if sibling has a right child , set its color to the color of parent, then set its right child's and it parent's color to black, then restructure passing the right child
                    if(sibling->right) {
                        sibling->is_red = currNode->parent->is_red;
                        sibling->right->is_red = currNode->parent->is_red = false;
                        restructure(sibling->right, true);
                    }else if(sibling->left && sibling->left->is_red){
                        // if sibling has a left child and is red, set its color to red, then set its right child's and it parent's color to black, then restructure passing the left child
                       sibling->left->is_red = true;
                       currNode->parent->is_red = sibling->is_red = false;
                       restructure(sibling->left, true);

                        // if sibling has a left child, set its child's color to the color of its parent, then set its color to black 
                       if(sibling->left)sibling->left->is_red = currNode->parent->is_red;
                        sibling->is_red = false;
                    }else{

                        // else, set its color to the color of the parent, then set hte parent's color to black, then restructure passing sibling
                        sibling->is_red = currNode->parent->is_red;
                        currNode->parent->is_red = false;
                        restructure(sibling, true);
                    }
                    currNode = root;
                }
            }else if(currNode->parent->right == currNode && currNode->parent->left){
                // same thing but in reverse
                sibling = currNode->parent->left;
                if(sibling->is_red){
                    cout << "DELETION Violation: Case 3\n";
                    currNode->parent->is_red = true;
                    sibling->is_red = false;
                    restructure(sibling, false);
                    sibling = currNode->parent->left;
                }
                if(
                        (!sibling->right && !sibling->left) ||
                        (sibling->right && sibling->left && !sibling->right->is_red && !sibling->left->is_red))
                {
                    cout << "DELETION Violation: Case 2\n";
                    sibling->is_red = true;
                    currNode = currNode->parent;
                }else{
                    cout << "DELETION Violation: Case 1\n";
                    if(sibling->left){
                        sibling->is_red = currNode->parent->is_red;
                        sibling->left->is_red = currNode->parent->is_red = false;
                        restructure(sibling->left, true);
                    }else if(sibling->right && sibling->right->is_red){
                        sibling->right->is_red = true;
                        currNode->parent->is_red = sibling->is_red = false;
                        restructure(sibling->right, true);
                        if(sibling->right)
                            sibling->right->is_red = currNode->parent->is_red;
                        sibling->is_red = false;
                    }else {
                        sibling->is_red = currNode->parent->is_red;
                        currNode->parent->is_red = false;
                        restructure(sibling, true);
                    }
                    currNode = root;
                }
            }else
                // else, set currNode to its parent
                currNode = currNode->parent;
        }
        // set it to black
        currNode->is_red = false;
    }

    bool remove(int num) {
        if (isEmpty()) {
            return false;
        }
        node* rem_node = search_node(root, num);
        if (rem_node->element != num) {
            return false;
        }

        // FIND the number of children.
        int children = 0;
        // 0 - no children
        // -1 - left child only
        // 1 - right child only
        // 2 - both children
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

        if (children == 0) { // NO CHILDREN
            node* parent = rem_node->parent;

            if(!rem_node->is_red){
                deleteFix(rem_node);
            }

            if (!parent) {
                root = NULL;
            } else {
                if (rem_node == parent->left) {
                    parent->left = NULL;
                } else {
                    parent->right = NULL;
                }
            }


            free(rem_node);
            size--;
        } else if (children == -1 || children == 1) { // ONE CHILD
            node* parent = rem_node->parent;
            node* child;
            if (children == -1) {
                child = rem_node->left;
            } else {
                child = rem_node->right;
            }

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

            deleteFix(child);

            free(rem_node);
            size--;
        } else { // TWO CHILDREN
            node* restructureNode = rem_node;

            node* right_st = rem_node->right;
            while (right_st->left != NULL) {
                right_st = right_st->left;
            }

            if(right_st != rem_node->right){
                restructureNode = right_st;
            }
                
            if(!restructureNode->is_red){
                deleteFix(restructureNode);
            }


            int temp = right_st->element;
            remove(temp);
            rem_node->element = temp;
        }

        return true;
    }

    void nodeTransplant(node* par, node* curr){

        if(par == root){
            root = curr;
        }
        else if(par->parent->left == par){
            par->parent->left = curr;
        }
        else{
            par->parent->right = curr;
        }
        if(curr){
            curr->parent = par->parent;
        }
    }

    // implementation of rotate operation of x where
    //   |
    //   y
    //  /
    // x <- curr
    void zigright(node* curr) {
        node *right = curr->right, *parent = curr->parent;
        nodeTransplant(parent, curr);
        curr->right = parent;
        parent->parent = curr;
        parent->left = right;
        if(right)right->parent = parent;
    }

    void zigleft(node* curr) {
        node *left = curr->left, *parent = curr->parent;
        nodeTransplant(parent, curr);
        curr->left = parent;
        parent->parent = curr;
        parent->right = left;
        if(left)left->parent = parent;
    }

    // Given the child, find its parent and grandparent. Assume that both are present.
    // Return the parent node (or b) after the restructure.
    node* restructure(node* child, bool print) {
        node* par = child->parent;
        // This is an indicator of the placement of parent to child (ptoc)

        bool ptoc_right = false;
        if (par->right == child) {
            ptoc_right = true;
        }

        node* gp = par->parent;
        // This is an indicator of the placement of grandparent to parent (gtop)
        bool gtop_right = false;
        if (gp && gp->right == par) {
            gtop_right = true;
        }
        if (gp && gtop_right && ptoc_right) {

            if(print)
                cout << "ZIGLEFT\n";
            zigleft(par);
            return par;
        }

        else if (gp && gtop_right) {
            if(print)
                cout << "ZIGZAGLEFT\n";
            zigright(child);
            zigleft(child);
            return child;
        }
        else if (gp && !ptoc_right) {
            if(print)
                cout << "ZIGRIGHT\n";
            zigright(par);
            return par;
        }
        else if(gp){
            if(print)
                cout << "ZIGZAGRIGHT\n";
            zigleft(child);
            zigright(child);
            return child;

        }else if(ptoc_right){
            if(print)
                cout << "ZIGLEFT\n";
            zigleft(child);
            return child;
        }
        else {
            if(print)
                cout << "ZIGRIGHT\n";
            zigright(child);
            return child;
        }
    }

    // WARNING. Do not modify these methods below.
    // Doing so will nullify your score for this activity.
    void print() {
		cout << "Size: " << size << endl;
		if (!root) {
			cout << "EMPTY" << endl;
			return;
		}
		node* curr = root;
		print_node("", root, false);
    }

    void print_node(string prefix, node* n, bool isLeft) {
		cout << prefix;
			cout << (isLeft ? "+--L: " : "+--R: " );
			cout << n->element;
            if(n->is_red){
                cout<<"(Red)";
            }else{
                cout<<"(Black)";
            }
            cout<<endl;
		if (n->left) {
			print_node(prefix + "|   ", n->left, true);
		}
		if (n->right) {
			print_node(prefix + "|   ", n->right, false);
		}
    }

    bool isEmpty() {
        return size == 0;
    }

    // WARNING. Do not modify this method.
    // Doing so will nullify your score for this activity.
    bool check_health(node* curr, node* parent) {
        bool health = curr->parent == parent;
        if (curr->left != NULL) {
            health &= check_health(curr->left, curr);
        }
        if (curr->right != NULL) {
            health &= check_health(curr->right, curr);
        }
        return health;
    }
};