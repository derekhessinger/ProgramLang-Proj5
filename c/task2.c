/**
 * Derek Hessinger
 * Prof. Ying Li
 * CS 333
 * 11/19/24
 * 
 * This program experiments with the time cost of memory management in C
 * 
 * gcc -o task2 task2.c
 * ./task2
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITERATIONS 1000
#define SMALL_SIZE 100
#define MEDIUM_SIZE 10000
#define LARGE_SIZE 1000000
#define NANO_TO_MS 1000000.0  // Convert nanoseconds to milliseconds

typedef struct {
    size_t size_ints;
    double avg_time_ms;
    double total_bytes;
} TimingResult;

// Function to measure allocation time for a specific size with nanosecond precision
TimingResult measure_allocation_time(size_t num_ints, int iterations) {
    struct timespec start, end;
    TimingResult result;
    int *ptr;
    double total_time = 0;
    
    // Warm-up allocation (not included in timing)
    ptr = malloc(sizeof(int) * num_ints);
    free(ptr);
    
    // Perform timed allocations
    for (int i = 0; i < iterations; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        ptr = malloc(sizeof(int) * num_ints);
        if (ptr == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        free(ptr);
        
        // Calculate time difference in nanoseconds, then convert to milliseconds
        double time_ns = (end.tv_sec - start.tv_sec) * 1e9 + 
                        (end.tv_nsec - start.tv_nsec);
        total_time += time_ns / NANO_TO_MS;
    }
    
    result.size_ints = num_ints;
    result.avg_time_ms = total_time / iterations;
    result.total_bytes = (double)num_ints * sizeof(int);
    
    return result;
}

// Function to measure chunked allocation time with nanosecond precision
TimingResult measure_chunked_allocation(size_t total_ints, size_t chunk_size, int iterations) {
    struct timespec start, end;
    TimingResult result;
    double total_time = 0;
    int num_chunks = total_ints / chunk_size;
    int **ptrs = malloc(sizeof(int*) * num_chunks);
    
    // Warm-up allocation
    for (int j = 0; j < num_chunks; j++) {
        ptrs[j] = malloc(sizeof(int) * chunk_size);
        free(ptrs[j]);
    }
    
    // Perform timed allocations
    for (int i = 0; i < iterations; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        for (int j = 0; j < num_chunks; j++) {
            ptrs[j] = malloc(sizeof(int) * chunk_size);
            if (ptrs[j] == NULL) {
                printf("Memory allocation failed!\n");
                exit(1);
            }
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        // Free allocated memory
        for (int j = 0; j < num_chunks; j++) {
            free(ptrs[j]);
        }
        
        // Calculate time difference in nanoseconds, then convert to milliseconds
        double time_ns = (end.tv_sec - start.tv_sec) * 1e9 + 
                        (end.tv_nsec - start.tv_nsec);
        total_time += time_ns / NANO_TO_MS;
    }
    
    free(ptrs);
    
    result.size_ints = total_ints;
    result.avg_time_ms = total_time / iterations;
    result.total_bytes = (double)total_ints * sizeof(int);
    
    return result;
}

int main() {
    TimingResult single_results[3];
    TimingResult chunked_results[3];
    FILE *fp;
    
    // Open CSV file for writing
    fp = fopen("memory_allocation_results.csv", "w");
    if (fp == NULL) {
        printf("Could not open file for writing!\n");
        return 1;
    }
    
    // Write CSV header
    fprintf(fp, "Size (ints),Size (bytes),Single Allocation Time (ms),Chunked Allocation Time (ms)\n");
    
    // Test allocations and write results
    size_t sizes[] = {SMALL_SIZE, MEDIUM_SIZE, LARGE_SIZE};
    
    printf("\nMeasuring allocations...\n");
    
    for (int i = 0; i < 3; i++) {
        // Measure single allocations
        single_results[i] = measure_allocation_time(sizes[i], NUM_ITERATIONS);
        
        // Measure chunked allocations
        size_t chunk_size = sizes[i] / 100;
        chunked_results[i] = measure_chunked_allocation(sizes[i], chunk_size, NUM_ITERATIONS);
        
        // Write results to CSV
        fprintf(fp, "%zu,%.0f,%.9f,%.9f\n", 
                single_results[i].size_ints,
                single_results[i].total_bytes,
                single_results[i].avg_time_ms,
                chunked_results[i].avg_time_ms);
        
        // Print human-readable results to console
        printf("\nResults for size %zu:\n", sizes[i]);
        printf("Single allocation: %.9f ms\n", single_results[i].avg_time_ms);
        printf("Chunked allocation: %.9f ms\n", chunked_results[i].avg_time_ms);
    }
    
    fclose(fp);
    printf("\nResults have been saved to 'memory_allocation_results.csv'\n");
    
    return 0;
}