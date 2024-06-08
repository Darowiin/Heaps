#include <algorithm>
#include <iostream>
#include <numeric>
#include <chrono>
#include <vector>
#include <random>
#include "Heaps/BinomialHeap.h"
#include "Heaps/FibonacciHeap.h"

template <typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    const int NUM_EXPERIMENTS = 10;
    for (int num = 1000; num <= 50000; num += 1000) {
        std::vector<int> data(num);
        std::iota(data.begin(), data.end(), 1);
        std::reverse(data.begin(), data.end());
        //std::random_device rd;
        //std::mt19937 g(rd());
        //std::shuffle(data.begin(), data.end(), g);
        std::cout << "SIZE = " << num << std::endl;

        std::cout << "Insert operations:" << std::endl;
        double binomialInsertTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            BinomialHeap<int> binomialHeap;
            binomialInsertTime += measureTime([&]() {
                for (int x : data) {
                    binomialHeap.insert(x);
                }
                });
        }
        std::cout << "BinomialHeap: " << binomialInsertTime / NUM_EXPERIMENTS << " ms" << std::endl;

        double fibonacciInsertTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            FibonacciHeap<int> fibonacciHeap;
            fibonacciInsertTime += measureTime([&]() {
                for (int x : data) {
                    fibonacciHeap.insert(x);
                }
                });
        }
        std::cout << "FibonacciHeap: " << fibonacciInsertTime / NUM_EXPERIMENTS << " ms" << std::endl;

        std::cout << "Find minimum operations:" << std::endl;
        BinomialHeap<int> binomialHeapForMin;
        FibonacciHeap<int> fibonacciHeapForMin;

        for (int x : data) {
            binomialHeapForMin.insert(x);
            fibonacciHeapForMin.insert(x);
        }

        double binomialFindMinTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            binomialFindMinTime += measureTime([&]() {
                binomialHeapForMin.getMin();
                });
        }
        std::cout << "BinomialHeap: " << binomialFindMinTime / NUM_EXPERIMENTS << " ms" << std::endl;

        double fibonacciFindMinTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            fibonacciFindMinTime += measureTime([&]() {
                fibonacciHeapForMin.getMin();
                });
        }
        std::cout << "FibonacciHeap: " << fibonacciFindMinTime / NUM_EXPERIMENTS << " ms" << std::endl;

        std::cout << "Extract minimum operations:" << std::endl;
        double binomialExtractMinTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            BinomialHeap<int> binomialHeapForExtract;
            for (int x : data) {
                binomialHeapForExtract.insert(x);
            }
            binomialExtractMinTime += measureTime([&]() {
                while (!binomialHeapForExtract.isEmpty()) {
                    binomialHeapForExtract.extractMin();
                }
                });
        }
        std::cout << "BinomialHeap: " << binomialExtractMinTime / NUM_EXPERIMENTS << " ms" << std::endl;

        double fibonacciExtractMinTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            FibonacciHeap<int> fibonacciHeapForExtract;
            for (int x : data) {
                fibonacciHeapForExtract.insert(x);
            }
            fibonacciExtractMinTime += measureTime([&]() {
                while (!fibonacciHeapForExtract.isEmpty()) {
                    fibonacciHeapForExtract.extractMin();
                }
                });
        }
        std::cout << "FibonacciHeap: " << fibonacciExtractMinTime / NUM_EXPERIMENTS / num << " ms" << std::endl;

        std::cout << "Decrease key operations:" << std::endl;
        double binomialDecreaseKeyTime = 0, fibonacciDecreaseKeyTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            BinomialHeap<int> binomialHeapForDecrease;
            FibonacciHeap<int> fibonacciHeapForDecrease;
            for (int x : data) {
                binomialHeapForDecrease.insert(x);
                fibonacciHeapForDecrease.insert(x);
            }

            binomialDecreaseKeyTime += measureTime([&]() {
                for (int i = 0; i < num / 2; ++i) {
                    binomialHeapForDecrease.decreaseKey(i + 1, i);
                }
                });
            fibonacciDecreaseKeyTime += measureTime([&]() {
                for (int i = 0; i < num / 2; ++i) {
                    fibonacciHeapForDecrease.decreaseKey(i + 1, i);
                }
                });
        }
        std::cout << "BinomialHeap: " << binomialDecreaseKeyTime / NUM_EXPERIMENTS << " ms" << std::endl;
        std::cout << "FibonacciHeap: " << fibonacciDecreaseKeyTime / NUM_EXPERIMENTS / num << " ms" << std::endl;

        std::cout << "Merge operations:" << std::endl;
        double binomialMergeTime = 0, fibonacciMergeTime = 0;
        for (int i = 0; i < NUM_EXPERIMENTS; ++i) {
            BinomialHeap<int> binomialHeap1, binomialHeap2;
            FibonacciHeap<int> fibonacciHeap1, fibonacciHeap2;
            for (int x : data) {
                binomialHeap1.insert(x);
                fibonacciHeap1.insert(x);
            }
            for (int x : data) {
                binomialHeap2.insert(x);
                fibonacciHeap2.insert(x);
            }

            binomialMergeTime += measureTime([&]() {
                binomialHeap1.merge(binomialHeap2);
                });
            fibonacciMergeTime += measureTime([&]() {
                fibonacciHeap1.merge(fibonacciHeap2);
                });
        }
        std::cout << "BinomialHeap: " << binomialMergeTime / NUM_EXPERIMENTS << " ms" << std::endl;
        std::cout << "FibonacciHeap: " << fibonacciMergeTime / NUM_EXPERIMENTS << " ms" << std::endl;
    }
    return 0;
}
