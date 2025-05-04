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
    // if no original head and head next, set left as original and right as a new LinkedList then return
    if (!original->head || !original->head->next) {
        left = original;
        right = new LinkedList();
        return;
    }

    // instantiate nodes slow and fast as original->head and prev as nullptr
    node* slow = original->head;
    node* fast = original->head;
    node* prev = nullptr;

    // while fast and fast->next isnt null, set prev to slow, slow to slow->next, and fast to fast->next->next
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    // Cut at midpoint by setting prev->next to null ptr and making left and right into new LinkedLists
    prev->next = nullptr;
    left = new LinkedList();
    right = new LinkedList();

    // set left->head to original->head and right->head to slow
    left->head = original->head;
    right->head = slow;
}

// Merge two sorted linked lists
LinkedList* merge(LinkedList* left, LinkedList* right) {
    // instantiate result Linked list, a dummy node with node* tail as address of dummy and dummy->next as nullptr
    LinkedList* result = new LinkedList();
    node dummy;
    node* tail = &dummy;
    dummy.next = nullptr;

    //make nodes for left->head and right ->head
    node* a = left->head;
    node* b = right->head;

    // while a and b arent null, if a->elem < b->elem, tail->next = a and a = a->next, else tail->next = b and b = b->next, set tail to tail->next
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

    // set tail->next to which ever node isnt null and result->head to dummy->next to return result
    tail->next = (a ? a : b);
    result->head = dummy.next;
    return result;
}

// Recursive merge sort
LinkedList* mergeSort(LinkedList* list) {
    // return list if there is no head or head->next
    if (!list->head || !list->head->next) {
        return list;
    }

    // make LinkedLists left and right as nullptr to perform split
    LinkedList *left = nullptr, *right = nullptr;
    splitList(list, left, right);

    // perform and set left and right lists using mergeSort
    left = mergeSort(left);
    right = mergeSort(right);

    // return the merge of left and right
    return merge(left, right);
}
