#include <gtest/gtest.h>
#include "Heaps/binomialHeap.h"

TEST(BinomialHeapTest, InsertAndGetMin) {
    BinomialHeap<int> heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);

    EXPECT_EQ(heap.getMin(), 5);
}

TEST(BinomialHeapTest, ExtractMin) {
    BinomialHeap<int> heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);

    EXPECT_EQ(heap.extractMin(), 5);
    EXPECT_EQ(heap.getMin(), 10);
}

TEST(BinomialHeapTest, IsEmpty) {
    BinomialHeap<int> heap;
    EXPECT_TRUE(heap.isEmpty());

    heap.insert(10);
    EXPECT_FALSE(heap.isEmpty());

    heap.extractMin();
    EXPECT_TRUE(heap.isEmpty());
}

TEST(BinomialHeapTest, CustomComparator) {
    struct MaxComparator {
        bool operator()(const int& a, const int& b) const {
            return a > b;
        }
    };

    BinomialHeap<int, MaxComparator> maxHeap;
    maxHeap.insert(10);
    maxHeap.insert(20);
    maxHeap.insert(5);

    EXPECT_EQ(maxHeap.getMin(), 20);
    EXPECT_EQ(maxHeap.extractMin(), 20);
    EXPECT_EQ(maxHeap.getMin(), 10);
}

TEST(BinomialHeapTest, StringTest) {
    BinomialHeap<std::string> stringHeap;
    stringHeap.insert("apple");
    stringHeap.insert("orange");
    stringHeap.insert("banana");

    EXPECT_EQ(stringHeap.getMin(), "apple");
    EXPECT_EQ(stringHeap.extractMin(), "apple");
    EXPECT_EQ(stringHeap.getMin(), "banana");
}

TEST(BinomialHeapTest, CustomTypeTest) {
    struct CustomType {
        int id;
        std::string name;

        bool operator<(const CustomType& other) const {
            return id < other.id;
        }
    };

    BinomialHeap<CustomType> customHeap;
    customHeap.insert({ 1, "one" });
    customHeap.insert({ 3, "three" });
    customHeap.insert({ 2, "two" });

    EXPECT_EQ(customHeap.getMin().name, "one");
    EXPECT_EQ(customHeap.extractMin().name, "one");
    EXPECT_EQ(customHeap.getMin().name, "two");
}

TEST(BinomialHeapTest, EmptyHeapOperations) {
    BinomialHeap<int> emptyHeap;
    EXPECT_TRUE(emptyHeap.isEmpty());
    EXPECT_THROW(emptyHeap.getMin(), std::runtime_error);
    EXPECT_THROW(emptyHeap.extractMin(), std::runtime_error);
}

TEST(BinomialHeapTest, LargeDataTest) {
    const int NUM_ELEMENTS = 100000;
    BinomialHeap<int> bigHeap;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        bigHeap.insert(i);
    }
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        EXPECT_EQ(bigHeap.extractMin(), i);
    }
    EXPECT_TRUE(bigHeap.isEmpty());
}