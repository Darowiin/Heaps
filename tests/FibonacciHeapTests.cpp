#include "Heaps/FibonacciHeap.h"
#include "gtest/gtest.h"

TEST(FibonacciHeapTest, InsertAndGetMin) {
    FibonacciHeap<int> heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);

    EXPECT_EQ(heap.getMin(), 5);
}

TEST(FibonacciHeapTest, ExtractMin) {
    FibonacciHeap<int> heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);

    EXPECT_EQ(heap.extractMin(), 5);
    EXPECT_EQ(heap.getMin(), 10);
}

TEST(FibonacciHeapTest, IsEmpty) {
    FibonacciHeap<int> heap;
    EXPECT_TRUE(heap.isEmpty());

    heap.insert(10);
    EXPECT_FALSE(heap.isEmpty());

    heap.extractMin();
    EXPECT_TRUE(heap.isEmpty());
}

TEST(FibonacciHeapTest, CustomComparator) {
    struct MaxComparator {
        bool operator()(const int& a, const int& b) const {
            return a > b;
        }
    };

    FibonacciHeap<int, MaxComparator> maxHeap;
    maxHeap.insert(10);
    maxHeap.insert(20);
    maxHeap.insert(5);

    EXPECT_EQ(maxHeap.getMin(), 20);
    EXPECT_EQ(maxHeap.extractMin(), 20);
    EXPECT_EQ(maxHeap.getMin(), 10);
}

TEST(FibonacciHeapTest, StringTest) {
    FibonacciHeap<std::string> stringHeap;
    stringHeap.insert("apple");
    stringHeap.insert("orange");
    stringHeap.insert("banana");

    EXPECT_EQ(stringHeap.getMin(), "apple");
    EXPECT_EQ(stringHeap.extractMin(), "apple");
    EXPECT_EQ(stringHeap.getMin(), "banana");
}

TEST(FibonacciHeapTest, CustomTypeTest) {
    struct CustomType {
        int id;
        std::string name;

        bool operator<(const CustomType& other) const {
            return id < other.id;
        }
    };

    FibonacciHeap<CustomType> customHeap;
    customHeap.insert({ 1, "one" });
    customHeap.insert({ 3, "three" });
    customHeap.insert({ 2, "two" });

    EXPECT_EQ(customHeap.getMin().name, "one");
    EXPECT_EQ(customHeap.extractMin().name, "one");
    EXPECT_EQ(customHeap.getMin().name, "two");
}

TEST(FibonacciHeapTest, EmptyHeapOperations) {
    FibonacciHeap<int> emptyHeap;
    EXPECT_TRUE(emptyHeap.isEmpty());
    EXPECT_THROW(emptyHeap.getMin(), std::runtime_error);
    EXPECT_THROW(emptyHeap.extractMin(), std::runtime_error);
}

TEST(FibonacciHeapTest, LargeDataTest) {
    const int NUM_ELEMENTS = 100000;
    FibonacciHeap<int> bigHeap;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        bigHeap.insert(i);
    }
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        EXPECT_EQ(bigHeap.extractMin(), i);
    }
    EXPECT_TRUE(bigHeap.isEmpty());
}