/**
 * Derek Hessinger and Sumira Naroola
 * Prof. Ying Li
 * CS 333
 * 11/20/24
 * 
 * This program demonstrates different memory management systems in C++
 * 
 * g++ task2 task2.cpp
 * ./task2
 */

// Example 1: Explicit allocation/deallocation
void explicitMemory() {
    // Single object allocation
    int* ptr = new int(42);
    // Single object deallocation
    delete ptr;

    // Array allocation
    int* arr = new int[100];
    // Array deallocation
    delete[] arr;
}

// Example 2: Smart Pointers (Modern C++ Memory Management)
#include <memory>
void smartPointers() {
    // Unique pointer - automatically deletes when out of scope
    std::unique_ptr<int> unique(new int(42));
    
    // Shared pointer - reference counted
    std::shared_ptr<int> shared = std::make_shared<int>(42);
    
    // Weak pointer - doesn't affect reference count
    std::weak_ptr<int> weak = shared;
}

// Example 3: Memory Leaks
void memoryLeaks() {
    // Memory leak - no deletion
    int* leak = new int[1000];
    
    // Resource leak in exception
    try {
        int* data = new int[1000];
        throw std::runtime_error("Error");
        delete[] data;  // Never reached
    } catch (...) {}
}

// Example 4: Side-by-side comparison with C
// C version
void cStyle() {
    int* arr = (int*)malloc(100 * sizeof(int));
    free(arr);
}

// C++ version
void cppStyle() {
    int* arr = new int[100];
    delete[] arr;
    
    // Better C++ version using RAII
    std::vector<int> vec(100);  // Automatic cleanup
}

// Example 5: Container Memory Management
#include <vector>
#include <list>
void containerMemory() {
    std::vector<int> vec;
    vec.reserve(1000);  // Pre-allocate memory
    
    vec.push_back(42);  // Automatic reallocation if needed
    vec.shrink_to_fit();  // Release unused memory
    
    // List nodes allocated as needed
    std::list<int> list;
    list.push_back(42);  // Allocates new node
}