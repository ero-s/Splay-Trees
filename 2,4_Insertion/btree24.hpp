#include "node.hpp"
#include <iostream>
using namespace std;

class BTree24 {
	node* root;
	node* search(int num){
	    node* curr = root;
	    while(curr->children_size != 0){
	        int size = curr->size;
	        for(int i = 0; i < size; i++){
	            if(curr->keys[i] == num){
	                return nullptr;
	            }
	            if(num < curr->keys[i]){
	                curr = curr->children[i];
	                break;
	            }else if(i == size-1){
	                int last_child = curr->children_size - 1;
	                curr = curr->children[last_child];
	            }
	        }
	    }
	    for(int i = 0; i < curr->size; i++){
	        if(curr->keys[i] == num){
	            return nullptr;
	        }
	    }
	    return curr;
	}

	public:
	BTree24() {
		root = nullptr;
	}

	bool insert(int num) {
	    if(!root){
	        root = new node();
	        root->addKey(num);
	        return true;
	    }
	    node* searched = search(num);
	    if(!searched) return false;
	    searched->addKey(num);
	    if(searched->isFull()){
	        split(searched);
	    }
	    return true;
	}
	
	void split(node* curr){
	    node* left = new node();
	    node* right = new node();
	    left->addKey(curr->keys[0]);
	    left->addKey(curr->keys[1]);
	    right->addKey(curr->keys[3]);
	    node* parent;
	    
	    if(curr == root){
	        parent = new node();
	        parent->addKey(curr->keys[2]);
	        parent->setChildrenSplit(left, right, curr);
	        root = parent;
	    }
	    else{
	        parent = curr->parent;
	        parent->addKey(curr->keys[2]);
	        parent->setChildrenSplit(left, right, curr);
	        if(parent->isFull())split(parent);
	    }
	}

	// WARNING. Do not modify these methods.
    // Doing so will nullify your score for this activity.
	void print_node(string s, node* n) {
		cout << s << ": ";
		for (int i = 0; i < n->size; i++) {
			cout << n->keys[i] << " ";
		}
		cout << endl;
		for (int i = 0; i <= n->size; i++) {
			if (n->children[i]) {
				print_node("Child " + to_string(i+1) + " of " + s,n->children[i]);
			}
		}
	}
    bool check_parent(node* curr, node* par) {
        if (!curr) {
            return true;
        }
        if (curr->parent != par) {
            if (!curr->parent) {
                cout << "Illegal parent of [" << curr->keys[0] << ",...]: NULL -- must be [" << par->keys[0] << ",...]" << endl;
            } else if (!par) {
                cout << "Illegal parent of [" << curr->keys[0] << ",...]: [" << curr->parent->keys[0] << ",...] -- must be NULL" << endl;
            } else {
                cout << "Illegal parent of [" << curr->keys[0] << ",...]: [" << curr->parent->keys[0] << ",...] -- must be [" << par->keys[0] << ",...]" << endl;
            }
            return false;
        }
        bool res = true;
        for (int i = 0; i < curr->size; i++) {
        	res &= check_parent(curr->children[i], curr);
		}
        return res;
    }

	void print() {
		print_node("Root", root);
		check_parent(root, NULL);
	}
};