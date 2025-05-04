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
      if (isEmpty()) {
        return false;
      }
      node* rem_node = search_node(root, num);
      if (rem_node->element != num) {
        return false;
      }

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

      if (children == 0) {
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
      } else if (children == -1 || children == 1) {
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
      } else {
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

    void restructure(node* child) {
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