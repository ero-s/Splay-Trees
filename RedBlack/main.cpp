#include "binarytree.hpp"
#include <iostream>
using namespace std;

int main(){
    BinaryTree* tree = new BinaryTree();
    int choice;
    int num;
    
    do{
        cout << "1. insert" <<endl;
        cout << "2. delete" <<endl;
        cout << "3. print" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;
        
        switch(choice){
            case 1:
                cout << "Enter element to insert: ";
                cin >> num;
                
                tree->insert(num);
                cout << endl;
                break;
            case 2:
                cout << "Enter element to delete: ";
                cin >> num;
                tree->_delete(num);
                cout << endl;
                break;
            case 3:
                tree->print();
                cout << endl << endl;
                break;     
        }
    }while(choice != 0);

    return 0;
}