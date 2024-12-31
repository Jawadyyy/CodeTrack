#pragma once
#include <string>

template <typename T>
struct Node {
    T* data;
    Node* next;
    Node* prev;
};

template <typename T>
class DoublyLinkedList {
public:
    Node<T>* head;
    Node<T>* tail;

    DoublyLinkedList();
    ~DoublyLinkedList();

    void insertAtEnd(T* data);
    T* find(const std::string& key, bool (*compare)(const T*, const std::string&)) const;
    void clear();
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template <typename T>
void DoublyLinkedList<T>::insertAtEnd(T* data) {
    Node<T>* node = new Node<T>{data, nullptr, tail};
    if (tail) {
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
}

template <typename T>
T* DoublyLinkedList<T>::find(const std::string& key, bool (*compare)(const T*, const std::string&)) const {
    Node<T>* current = head;
    while (current) {
        if (compare(current->data, key)) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    Node<T>* current = head;
    while (current) {
        Node<T>* temp = current;
        current = current->next;
        delete temp->data;
        delete temp;
    }
    head = tail = nullptr;
}
