#include "node.hpp"
#include <iostream>
#include <stdexcept>
#include <queue>

using namespace std;

class BinaryTree {
public:
    node* root;
    int size;

    bool isLeft(node* n) {
        node* p = n->parent;
        return (p && n == p->left);
    }

    BinaryTree() : root(nullptr), size(0) {}

    node* addRoot(int num) {
        root = new node;
        root->elem = num;
        root->isRed = false;
        root->parent = nullptr;
        root->left = nullptr;
        root->right = nullptr;
        size++;
        return root;
    }

    node* addLeft(node* p, int num) {
        node* n = new node;
        n->elem = num;
        n->isRed = true;
        n->parent = p;
        n->left = nullptr;
        n->right = nullptr;
        p->left = n;
        size++;
        return n;
    }

    node* addRight(node* p, int num) {
        node* n = new node;
        n->elem = num;
        n->isRed = true;
        n->parent = p;
        n->left = nullptr;
        n->right = nullptr;
        p->right = n;
        size++;
        return n;
    }

    bool insert_node(node* n, int num) {
        if (!n) {
            rebalance_insert(addRoot(num));
            return true;
        }
        if (num < n->elem) {
            if (n->left) {
                return insert_node(n->left, num);
            }
            node* inserted = addLeft(n, num);
            rebalance_insert(inserted);
            return true;
        } else if (num > n->elem) {
            if (n->right) {
                return insert_node(n->right, num);
            }
            node* inserted = addRight(n, num);
            rebalance_insert(inserted);
            return true;
        }
        return false;  // duplicate
    }

    bool insert(int num) {
        return insert_node(root, num);
    }

    void rebalance_insert(node* n) {
        if (!n) return;
        if (n == root) {
            n->isRed = false;
            return;
        }
        node* p = n->parent;
        node* gp = p ? p->parent : nullptr;
        if (!p || !gp) return;

        if (p->isRed) {
            node* uncle = isLeft(p) ? gp->right : gp->left;
            if (uncle && uncle->isRed) {
                cout<<"INSERTION VIOLATION: Case 2"<<endl;
                p->isRed = false;
                uncle->isRed = false;
                gp->isRed = true;
                rebalance_insert(gp);
            } else {
                cout<<"INSERTION VIOLATION: Case 1"<<endl;
                node* m = restructure(n);
                if (m) {
                    m->isRed = false;
                    if (m->left) m->left->isRed = true;
                    if (m->right) m->right->isRed = true;
                }
            }
        }
    }

    node* zigleft(node* curr) {
        node* y = curr->parent;
        node* x = y->parent;
        if (y == root) {
            root = curr;
        } else if (x->left == y) {
            x->left = curr;
        } else {
            x->right = curr;
        }
        curr->parent = y->parent;
        y->right = curr->left;
        if (curr->left) curr->left->parent = y;
        curr->left = y;
        y->parent = curr;
        return curr;
    }

    node* zigright(node* curr) {
        node* y = curr->parent;
        node* x = y->parent;
        if (y == root) {
            root = curr;
        } else if (x->right == y) {
            x->right = curr;
        } else {
            x->left = curr;
        }
        curr->parent = y->parent;
        y->left = curr->right;
        if (curr->right) curr->right->parent = y;
        curr->right = y;
        y->parent = curr;
        return curr;
    }

    node* restructure(node* x) {
        node* y = x->parent;
        node* z = y ? y->parent : nullptr;
        if (!y || !z) return nullptr;
        // LL case
        if (y == z->left && x == y->left) {
            zigright(y);
            return y;
        }
        // RR case
        if (y == z->right && x == y->right) {
            zigleft(y);
            return y;
        }
        // RL case
        if (y == z->right && x == y->left) {
            zigright(x);
            zigleft(x);
            return x;
        }
        // LR case
        if (y == z->left && x == y->right) {
            zigleft(x);
            zigright(x);
            return x;
        }
        return nullptr;
    }

	void removeNode(node* n) {
        bool color = n->isRed;
        node* x         = (n->right ? n->right : n->left);
        node* p         = n->parent;
    
        // ---- splice as before ----
        if (root == n) {
            root = x;
            if (x) x->parent = NULL;
        } else {
            if (p->left == n)      p->left  = x;
            else                    p->right = x;
            if (x) x->parent = p;
        }
        delete n;
        size--;
    
        // only if we removed a BLACK node do we need to fix double-black
        if (!color) {
            resolveDoubleBlack(x, p);
        }
    }
    
    bool _delete(int num) {
        return remove(root, num);
    }
    
    bool remove(node* n, int elem) {
        if (!n) return false;
        if (elem < n->elem)      return remove(n->left,  elem);
        else if (elem > n->elem) return remove(n->right, elem);
        else {
            if (n->left && n->right) {
                // two-child case: swap with successor
                node* tmp = n->right;
                while (tmp->left) tmp = tmp->left;
                n->elem = tmp->elem;
                return remove(n->right, tmp->elem);
            } else {
                removeNode(n);
                return true;
            }
        }
    }
    
    void resolveDoubleBlack(node* x, node* p) {
        while (x != root && (x == NULL || !x->isRed)) {
            bool isLeft = (p->left == x);
            node* w = isLeft ? p->right : p->left;
    
            // IF SIBLING IS RED
            if (w && w->isRed) {
                w->isRed = false;
                p->isRed = true;
                if (isLeft) zigright(w);
                else         zigleft(w);
                w = isLeft ? p->right : p->left;
            }
    
            // IF SIBLING IS BLACK WITH BOTH BLACK CHILDREN
            if (w
                && (!w->left  || !w->left->isRed)
                && (!w->right || !w->right->isRed))
            {
                if (w) w->isRed = true;
                x = p;
                p = x->parent;
            }
            else { // IF SIBLING IS BLACK WITH 1 RED AND 1 BLACK CHILD
                w = isLeft ? p->right : p->left;
    
                // NEAR KID RED
                if (isLeft
                    && w
                    && (!w->right || !w->right->isRed)
                    &&  w->left && w->left->isRed)
                {
                    w->left->isRed = false;
                    w->isRed = true;
                    zigright(w->left);
                    w = p->right;
                }
                else if (!isLeft
                         && w
                         && (!w->left || !w->left->isRed)
                         &&  w->right && w->right->isRed)
                {
                    w->right->isRed = false;
                    w->isRed = true;
                    zigleft(w->right);
                    w = p->left;
                }
    
                // FAR KID RED
                if (w) w->isRed = p->isRed;
                p->isRed = false;
                if (isLeft && w->right)     w->right->isRed = false;
                else if (!isLeft && w->left) w->left->isRed  = false;
    
                if (isLeft)     zigright(w);
                else             zigleft(w);
                x = root;  
                break;
            }
        }
        if (x) x->isRed = false;
    }

    void print() {
        cout << "Size: " << size << endl;
        if (!root) {
            cout << "EMPTY" << endl;
            return;
        }
        cout << root->elem << (root->isRed?"(R)":"(B)") << " (root)" << endl;
        if (root->left) print_node("    ", root->left, true);
        if (root->right) print_node("    ", root->right, false);
    }

    void print_node(const string& prefix, node* n, bool isLeft) {
        cout << prefix << (isLeft?"+--L: ":"+--R: ")
             << n->elem << (n->isRed?"(R)":"(B)") << endl;
        string childPrefix = prefix + "    ";
        if (n->left) print_node(childPrefix, n->left, true);
        if (n->right) print_node(childPrefix, n->right, false);
    }
};
