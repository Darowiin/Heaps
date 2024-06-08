#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <functional>

template <typename T, typename Comparator = std::less<T>>
class FibonacciHeap {
private:
    struct Node {
        T key;
        int degree;
        bool mark;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;

        explicit Node(T k) : key(k), degree(0), mark(false), parent(nullptr), child(nullptr) {
            left = right = this;
        }
    };

    Node* minNode;
    int nodeCount;
    Comparator compare;

    void link(Node* y, Node* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x;
        if (x->child == nullptr) {
            x->child = y;
            y->left = y->right = y;
        }
        else {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right = y;
            y->right->left = y;
        }
        x->degree++;
        y->mark = false;
    }

    void consolidate() {
        int D = std::log2(nodeCount) + 1;
        std::vector<Node*> A(D, nullptr);

        std::vector<Node*> rootList;
        Node* x = minNode;
        if (x != nullptr) {
            do {
                rootList.push_back(x);
                x = x->right;
            } while (x != minNode);

            for (Node* w : rootList) {
                x = w;
                int d = x->degree;
                while (A[d] != nullptr) {
                    Node* y = A[d];
                    if (compare(x->key, y->key) == false) {
                        std::swap(x, y);
                    }
                    link(y, x);
                    A[d] = nullptr;
                    d++;
                }
                A[d] = x;
            }

            minNode = nullptr;
            for (Node* node : A) {
                if (node != nullptr) {
                    if (minNode == nullptr) {
                        minNode = node;
                        node->left = node->right = node;
                    }
                    else {
                        node->left = minNode;
                        node->right = minNode->right;
                        minNode->right = node;
                        node->right->left = node;
                        if (compare(node->key, minNode->key)) {
                            minNode = node;
                        }
                    }
                }
            }
        }
    }

    void cut(Node* x, Node* y) {
        if (x->right == x) {
            y->child = nullptr;
        }
        else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        y->degree--;
        x->left = x->right = x;
        minNode->left->right = x;
        x->right = minNode;
        x->left = minNode->left;
        minNode->left = x;
        x->parent = nullptr;
        x->mark = false;
    }

    void cascadingCut(Node* y) {
        Node* z = y->parent;
        if (z != nullptr) {
            if (!y->mark) {
                y->mark = true;
            }
            else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    Node* find(Node* start, const T& key) const {
        if (!start) return nullptr;
        Node* node = start;
        do {
            if (node->key == key) return node;
            Node* res = find(node->child, key);
            if (res) return res;
            node = node->right;
        } while (node != start);
        return nullptr;
    }

public:
    FibonacciHeap() : minNode(nullptr), nodeCount(0), compare(Comparator()) {}

    ~FibonacciHeap() {
        clear();
    }

    void clear() {
        while (minNode) {
            extractMin();
        }
    }

    bool isEmpty() const {
        return minNode == nullptr;
    }

    void insert(const T key) {
        Node* node = new Node(key);
        if (minNode == nullptr) {
            minNode = node;
        }
        else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right = node;
            node->right->left = node;
            if (compare(node->key, minNode->key)) {
                minNode = node;
            }
        }
        nodeCount++;
    }

    T getMin() const {
        if (!minNode) throw std::runtime_error("Heap is empty");
        return minNode->key;
    }

    T extractMin() {
        if (!minNode) throw std::runtime_error("Heap is empty");

        Node* z = minNode;
        if (z != nullptr) {
            if (z->child != nullptr) {
                Node* child = z->child;
                do {
                    Node* next = child->right;
                    minNode->left->right = child;
                    child->right = minNode;
                    child->left = minNode->left;
                    minNode->left = child;
                    child->parent = nullptr;
                    child = next;
                } while (child != z->child);
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                minNode = nullptr;
            }
            else {
                minNode = z->right;
                consolidate();
            }
            nodeCount--;
            T minKey = z->key;
            delete z;
            return minKey;
        }
        return std::numeric_limits<T>::max();
    }

    void decreaseKey(const T& oldKey, const T& newKey) {
        if (compare(oldKey, newKey)) throw std::invalid_argument("New key is greater than current key");

        Node* node = find(minNode, oldKey);
        node->key = newKey;
        Node* y = node->parent;
        if (y != nullptr && compare(node->key, y->key)) {
            cut(node, y);
            cascadingCut(y);
        }
        if (compare(node->key, minNode->key)) {
            minNode = node;
        }
    }

    void deleteKey(const T key) {
        Node* node = find(minNode, key);
        if (node) {
            decreaseKey(node, std::numeric_limits<T>::min());
            extractMin();
        }
    }

    void merge(FibonacciHeap& other) {
        if (minNode == nullptr) {
            minNode = other.minNode;
        }
        else if (other.minNode != nullptr) {
            minNode->left->right = other.minNode->right;
            other.minNode->right->left = minNode->left;
            minNode->left = other.minNode;
            other.minNode->right = minNode;
            if (compare(other.minNode->key, minNode->key)) {
                minNode = other.minNode;
            }
        }
        nodeCount += other.nodeCount;
        other.minNode = nullptr;
        other.nodeCount = 0;
    }
};