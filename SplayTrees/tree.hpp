#include "node.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;
class BSTree {
    node* root;
    int size;

    node* create_node(int num, node* parent) {
        node* n = new node;
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

        free(rem_node);
        size--;
      } else { // TWO CHILDREN
        node* right_st = rem_node->right;
        while (right_st->left != NULL) {
          right_st = right_st->left;
        }

        int temp = right_st->element;
        remove(temp);
        rem_node->element = temp;
      }
      return true;
    }

    // TODO implementation of rotate operation of x where
    //  |
    //  y
    //   \
    //    x <- curr
    void zigleft(node* curr) {
        node* y = curr->parent;
        node* x = y->parent;
        
        if(y == root){
            root = curr;
        }else if(x->left == y){
            x->left = curr;
        }else{
            x->right = curr;
        }
        
        curr->parent = y->parent;
        y->right = curr->left;
        if(curr->left){
            curr->left->parent = y;
        }
        curr->left = y;
        y->parent = curr;
    }

    // TODO implementation of rotate operation of x where
    //   |
    //   y
    //  /
    // x <- curr
    void zigright(node* curr) {
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

    // GIVEN the child (or x), find the parent (or y), and the grandparent if any (or z).
    // Splay the child to the root recursively or iteratively.
    void restructure(node* child) {
        node* par; // parent
        // TODO find parent
        if(child->parent){
            par = child->parent;
        }

        // This is an indicator of the placement of parent to child (ptoc)
        bool ptoc_right = false;
        if (par->right == child) {
            ptoc_right = true;
        }

        node* gp;
        // TODO find grandparent. If gp does not exist, proceed to doing ZIGLEFT or ZIGRIGHT.
        if(par->parent){
            gp = par->parent;
        }
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

        // This is an indicator of the placement of grandparent to parent (gtop)
        bool gtop_right = false;
        if (gp->right == par) {
            gtop_right = true;
        }

        // FOR THE FOLLOWING: Write in each of the if statements a console output
        // on its corresponding operation (ZIGZIGLEFT, ZIGZIGRIGHT, ZIGZAGLEFT, or ZIGZAGRIGHT)

      // z
      //  \
      //   y
      //    \
      //     x
      if (gtop_right && ptoc_right) {
        // TODO call to either zigleft or zigright or both
        zigleft(par);
        zigleft(child);
        
        cout<<"ZIGZIGLEFT"<<endl;
        if(child != root) restructure(child);
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
        if(child != root) restructure(child);
      }

      //     z
      //    /
      //   y
      //  /
      // x
      else if (!gtop_right && !ptoc_right) {
        // TODO call to either zigleft or zigright or both
        zigright(par);
        zigright(child);
        
        cout<<"ZIGZIGRIGHT"<<endl;
        if(child != root) restructure(child);
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
        if(child != root) restructure(child);
      }

      return;
    }

    // WARNING. Do not modify the methods below.
    // Doing so will nullify your score for this activity.
    void print() {
        if (isEmpty()) {
            cout << "EMPTY" << endl;
            return;
        }
        cout << "PRE-ORDER: ";
        print_preorder(root);
        cout << endl << "IN-ORDER: ";
        print_inorder(root);
        cout << endl << "POST-ORDER: ";
        print_postorder(root);
        cout << endl << "STATUS: " << check_parent(root, NULL) << endl;
    }

    bool isEmpty() {
        return size == 0;
    }

    void print_preorder(node* curr) {
        cout << curr->element << " ";
        if (curr->left != NULL) {
            print_preorder(curr->left);
        }
        if (curr->right != NULL) {
            print_preorder(curr->right);
        }
    }

    void print_inorder(node* curr) {
        if (curr->left != NULL) {
            print_inorder(curr->left);
        }
        cout << curr->element << " ";
        if (curr->right != NULL) {
            print_inorder(curr->right);
        }
    }

    void print_postorder(node* curr) {
        if (curr->left != NULL) {
            print_postorder(curr->left);
        }
        if (curr->right != NULL) {
            print_postorder(curr->right);
        }
        cout << curr->element << " ";
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