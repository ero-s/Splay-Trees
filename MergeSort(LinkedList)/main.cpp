#include <iostream>
#include "linkedlist.h"

using namespace std;

LinkedList* mergeSort(LinkedList*);

// WARNING! Do not modify this main function!
// Doing so will nullify your score for this activity.
int main(void) {
    LinkedList* list = new LinkedList();

    int length;
    cout << "Input length: ";
    cin >> length;

    int input;
    for (int i = 0; i < length; i++) {
        cout << "Enter element " << i + 1 << ": ";
        cin >> input;
        list->add(input);
    }

    cout << "Original list: ";
    list->print();

    list = mergeSort(list);

    cout << "Final sorted list: ";
    list->print();

    return 0;
}

// Helper to split a list into two halves
void splitList(LinkedList* original, LinkedList*& left, LinkedList*& right) {
    if (!original->head || !original->head->next) {
        left = original;
        right = new LinkedList();
        return;
    }

    node* slow = original->head;
    node* fast = original->head;
    node* prev = nullptr;

    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    // Cut at midpoint
    prev->next = nullptr;

    left = new LinkedList();
    right = new LinkedList();

    left->head = original->head;
    right->head = slow;
}

// Merge two sorted linked lists
LinkedList* merge(LinkedList* left, LinkedList* right) {
    LinkedList* result = new LinkedList();
    node dummy;
    node* tail = &dummy;
    dummy.next = nullptr;

    node* a = left->head;
    node* b = right->head;

    while (a && b) {
        if (a->element < b->element) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    tail->next = (a ? a : b);
    result->head = dummy.next;
    return result;
}

// Recursive merge sort
LinkedList* mergeSort(LinkedList* list) {
    if (!list->head || !list->head->next) {
        return list;
    }

    LinkedList *left = nullptr, *right = nullptr;
    splitList(list, left, right);

    left = mergeSort(left);
    right = mergeSort(right);

    return merge(left, right);
}
