#pragma once

#include <iostream>
#include <functional>
#include <stdexcept>
#include <limits>

template <typename T, typename Comparator = std::less<T>>
class BinomialHeap {
private:
    template <typename T>
    struct Node {
        T key;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;

        explicit Node(T k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };
    Node<T>* head;
    Comparator compare;

    Node<T>* mergeTrees(Node<T>* b1, Node<T>* b2) const {
        if (compare(b2->key, b1->key)) {
            std::swap(b1, b2);
        }
        b2->parent = b1;
        b2->sibling = b1->child;
        b1->child = b2;
        b1->degree++;
        return b1;
    }

    Node<T>* unionHeaps(Node<T>* h1, Node<T>* h2) const {
        if (!h1) return h2;
        if (!h2) return h1;

        Node<T>* newHead = nullptr;
        Node<T>** pos = &newHead;

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *pos = h1;
                h1 = h1->sibling;
            }
            else {
                *pos = h2;
                h2 = h2->sibling;
            }
            pos = &((*pos)->sibling);
        }

        *pos = h1 ? h1 : h2;
        return newHead;
    }

    void adjustHeap() {
        if (!head) return;

        Node<T>* prev = nullptr;
        Node<T>* curr = head;
        Node<T>* next = curr->sibling;

        while (next) {
            if ((curr->degree != next->degree) ||
                (next->sibling && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            }
            else {
                if (compare(curr->key, next->key)) {
                    curr->sibling = next->sibling;
                    mergeTrees(curr, next);
                }
                else {
                    if (prev) {
                        prev->sibling = next;
                    }
                    else {
                        head = next;
                    }
                    mergeTrees(next, curr);
                    curr = next;
                }
            }
            next = curr->sibling;
        }
    }

    Node<T>* extractMinTree(Node<T>*& prevMin) {
        if (!head) return nullptr;

        Node<T>* minNode = head;
        Node<T>* minPrev = nullptr;
        Node<T>* next = head->sibling;
        Node<T>* prev = head;

        while (next) {
            if (compare(next->key, minNode->key)) {
                minNode = next;
                minPrev = prev;
            }
            prev = next;
            next = next->sibling;
        }

        if (minPrev) {
            minPrev->sibling = minNode->sibling;
        }
        else {
            head = minNode->sibling;
        }

        prevMin = minPrev;
        return minNode;
    }

    Node<T>* reverseChildList(Node<T>* node) {
        Node<T>* prev = nullptr;
        Node<T>* curr = node;
        Node<T>* next;

        while (curr) {
            next = curr->sibling;
            curr->sibling = prev;
            prev = curr;
            curr = next;
        }

        return prev;
    }

    Node<T>* findNode(Node<T>* node, const T key) {
        if (!node) return nullptr;
        if (node->key == key) return node;

        Node<T>* res = findNode(node->child, key);
        if (res) return res;

        return findNode(node->sibling, key);
    }

public:
    BinomialHeap() : head(nullptr), compare(Comparator()) {}

    ~BinomialHeap() {
        clear();
    }

    void clear() {
        while (head) {
            Node<T>* minNode = extractMinTree(head);
            delete minNode;
        }
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void insert(const T key) {
        Node<T>* node = new Node<T>(key);
        head = unionHeaps(head, node);
        adjustHeap();
    }

    T getMin() const {
        if (!head) throw std::runtime_error("Heap is empty");

        Node<T>* minNode = head;
        Node<T>* next = head->sibling;

        while (next) {
            if (compare(next->key, minNode->key)) {
                minNode = next;
            }
            next = next->sibling;
        }

        return minNode->key;
    }

    T extractMin() {
        if (!head) throw std::runtime_error("Heap is empty");

        Node<T>* prevMin = nullptr;
        Node<T>* minNode = extractMinTree(prevMin);

        T min = minNode->key;

        if (minNode->child) {
            Node<T>* newHead = reverseChildList(minNode->child);
            head = unionHeaps(head, newHead);
        }

        delete minNode;
        adjustHeap();
        return min;
    }

    void deleteKey(const T key) {
        Node<T>* node = findNode(head, key);
        if (node) {
            decreaseKey(node, std::numeric_limits<T>::min());
            extractMin();
        }
    }

    void decreaseKey(const T& oldKey, const T& newKey) {
        if (compare(oldKey, newKey)) throw std::invalid_argument("New key is greater than old key");

        Node<T>* node = findNode(head, oldKey);
        node->key = newKey;
        Node<T>* y = node;
        Node<T>* z = y->parent;

        while (z && compare(y->key, z->key)) {
            std::swap(y->key, z->key);
            y = z;
            z = y->parent;
        }
    }

    void merge(BinomialHeap& other) {
        head = mergeTrees(head, other.head);
        other.head = nullptr;
    }
};