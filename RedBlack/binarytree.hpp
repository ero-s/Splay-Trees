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

	   // void removeNode(node* n) {
    //     Color origColor = n->color;
    //     node* x         = (n->right ? n->right : n->left);
    //     node* p         = n->parent;
    
    //     // ---- splice as before ----
    //     if (root == n) {
    //         root = x;
    //         if (x) x->parent = NULL;
    //     } else {
    //         if (p->left == n)      p->left  = x;
    //         else                    p->right = x;
    //         if (x) x->parent = p;
    //     }
    //     delete n;
    //     size--;
    
    //     // only if we removed a BLACK node do we need to fix double-black
    //     if (origColor == BLACK) {
    //         resolveDoubleBlack(x, p);
    //     }
    // }
    
    // bool _delete(int num) {
    //     return remove(root, num);
    // }
    
    // bool remove(node* n, int elem) {
    //     if (!n) return false;
    //     if (elem < n->elem)      return remove(n->left,  elem);
    //     else if (elem > n->elem) return remove(n->right, elem);
    //     else {
    //         if (n->left && n->right) {
    //             // two-child case: swap with successor
    //             node* tmp = n->right;
    //             while (tmp->left) tmp = tmp->left;
    //             n->elem = tmp->elem;
    //             return remove(n->right, tmp->elem);
    //         } else {
    //             removeNode(n);
    //             return true;
    //         }
    //     }
    // }
    
    // void resolveDoubleBlack(node* x, node* p) {
    //     // x may be NULL (treat as black), but we always have p
    //     while (x != root && (x == NULL || x->color == BLACK)) {
    
    //         // are we on the left or right of p?
    //         bool isLeft = (p->left == x);
    
    //         node* w = isLeft ? p->right : p->left;
    
    //         // --- Case 1: red sibling ---
    //         if (w && w->color == RED) {
    //             w->color = BLACK;
    //             p->color = RED;
    //             if (isLeft) zigLeft(w);
    //             else         zigRight(w);
    //             // new sibling
    //             w = isLeft ? p->right : p->left;
    //         }
    
    //         // --- Case 2: black sibling, both kids black ---
    //         if (w
    //             && (!w->left  || w->left->color  == BLACK)
    //             && (!w->right || w->right->color == BLACK))
    //         {
    //             if (w) w->color = RED;
    //             x = p;
    //             p = x->parent;
    //         }
    //         else {
    //             // ensure we have up-to-date w
    //             w = isLeft ? p->right : p->left;
    
    //             // --- Case 3: sibling black, near kid red, far kid black ---
    //             if (isLeft
    //                 && w
    //                 && (!w->right || w->right->color == BLACK)
    //                 &&  w->left && w->left->color == RED)
    //             {
    //                 w->left->color = BLACK;
    //                 w->color       = RED;
    //                 zigRight(w->left);
    //                 w = p->right;
    //             }
    //             else if (!isLeft
    //                      && w
    //                      && (!w->left || w->left->color == BLACK)
    //                      &&  w->right && w->right->color == RED)
    //             {
    //                 w->right->color = BLACK;
    //                 w->color        = RED;
    //                 zigLeft(w->right);
    //                 w = p->left;
    //             }
    
    //             // --- Case 4: sibling black, far kid red ---
    //             if (w) w->color = p->color;
    //             p->color = BLACK;
    //             if (isLeft && w->right)     w->right->color = BLACK;
    //             else if (!isLeft && w->left) w->left->color  = BLACK;
    
    //             if (isLeft)     zigLeft(w);
    //             else             zigRight(w);
    
    //             x = root;  // done
    //             break;
    //         }
    //     }
    
    //     if (x) x->color = BLACK;
    // }

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
