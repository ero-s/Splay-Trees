#include <iostream>
#include <set>
#include <climits>

struct node {
	node* parent;
	int keys[4];
	node** children = (node**)calloc(5, sizeof(node*));
	int children_size = 0;
	int size = 0;
	
	void addKey(int num){
	    if(size == 0){
	        keys[0] = num;
	        size++;
	        return;
	    }
	    for(int i = 0; i < size; i++){
	       if(num<keys[i]){
	           for(int j = size; j > i; j--){
	                keys[j] = keys[j-1];
	           }
	           keys[i] = num;
	           size++;
	           return;
	       }
        }
        keys[size++] = num;
	}
	
	bool isFull(){
	    return size == 4;
	}
	bool isFullChildren(){
	    return children_size == 5;
	}
	
	void setChildrenSplit(node* left, node* right, node* curr){
	    if(children_size == 0){
	        children[0] = left;
	        children[1] = right;
	        children_size += 2;
	    }
	    else{
	        for(int i = 0 ; i < children_size; i++){
	            if(curr == children[i]){
	                for(int j = children_size; j > i; j--){
	                    children[j] = children[j-1];
	                }
	                children[i] = left;
	                children[i+1] = right;
	                break;
	            }   
	        }
	        children_size++;
	    }
	    if(curr->children_size != 0){
	        orphanChild(left, 0, curr, 0);
	        orphanChild(left, 1, curr, 1);
	        orphanChild(left, 2, curr, 2);
	        orphanChild(right, 0, curr, 3);
	        orphanChild(right, 1, curr, 4);
	    }
	    left->parent = this;
	    right->parent = this;
	    free(curr);
	}
	
	void orphanChild(node* newParent, int pos1, node* oldParent, int pos2){
	    node* child = oldParent->children[pos2];
	    newParent->children[pos1] = child;
	    child->parent = newParent;
	    newParent->children_size++;
	}
};