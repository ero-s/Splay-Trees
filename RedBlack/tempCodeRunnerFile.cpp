#include "binarytree.hpp"
#include <iostream>
using namespace std;
int main() {
    BSTree* tree = new BSTree();
    // build initial tree
    tree->insert(50);
    tree->insert(30);
    tree->insert(70);
    tree->insert(20);
    tree->insert(40);
    tree->insert(60);
    tree->insert(80);

    // print
    tree->print();

    // searches
    tree->search(25);
    tree->search(30);

    // deletes and prints
    tree->remove(20);
    tree->print();

    tree->remove(30);
    tree->print();

    tree->remove(50);
    tree->print();

    // more inserts and a print
    tree->insert(65);
    tree->insert(55);
    tree->print();

    // final delete and print
    tree->remove(70);
    tree->print();

    return 0;
};