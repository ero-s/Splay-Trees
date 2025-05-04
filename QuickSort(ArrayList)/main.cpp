#include <iostream>

using namespace std;

int* Array; //Refactored kay samok ang squiqqly lines kay ambiguous ang "array" nga name, naa may array sa <tuple>
int n;
void print();
void quickSort(int, int);

int main(void) {
    // Hey there, start typing your C++ code here...
    cout << "Enter number of elements: ";
    cin >> n;
    Array = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        cout << "Enter element " << i+1 << ": ";
        cin >> Array[i];
    }

    print();
    quickSort(0, n-1);
    cout << "Finish: ";
    print();

    return 0;
}
int partition(int low, int high) {
    int pivotVal = Array[low];
    int green = low;

    for(int i = low + 1; i <= high; i++){
        if(Array[i] < pivotVal){
            ++green;
            swap(Array[i], Array[green]);
            print();
        }
    }

    swap(Array[low], Array[green]);
    print();
    return green;
  }

void quickSort(int low, int high) {
    if (low >= high) return;
    int p = partition(low, high);
    quickSort(low, p - 1);
    quickSort(p + 1, high);
  }

void print() {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << Array[i];
        if (i < n-1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}