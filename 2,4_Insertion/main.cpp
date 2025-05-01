#include "btree24.hpp"
#include <iostream>
using namespace std;
int main() {
    BTree24* tree = new BTree24();

    char op;
    int num;
    do {
        cout << "Operation: ";
        cin >> op;
        switch (op) {
            case 'i':
                cin >> num;
                tree->insert(num);
                break;
            case 'p':
                tree->print();
                break;
            case 'x':
                cout << "Exiting";
                break;
            default:
                cout << "Invalid operation";
        }
    } while (op != 'x');
    return 0;
};