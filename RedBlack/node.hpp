#include <iostream>
using namespace std;

struct node {
    int elem;
    node* left;
    node* right;
    node* parent;
    bool isRed;
};