#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <x86intrin.h> // For rdtscp and clflush instructions

#define ARRAY_SIZE 1024       // Larger array to span multiple cache lines
#define CACHE_LINE_SIZE 64    // Cache line size (typical for x86)

// Static memory aligned for cache line size
uint8_t array[ARRAY_SIZE] __attribute__((aligned(CACHE_LINE_SIZE)));

// Function to measure memory access time
uint64_t measure_access_time(volatile uint8_t *addr) {
    uint64_t start, end;
    unsigned int aux;
    start = __rdtscp(&aux);  // Start timing
    (void)*addr;             // Access memory
    end = __rdtscp(&aux);    // Stop timing
    return end - start;
}

// Function to measure cache hits and save to a file
void measure_hits(const char *filename, int iterations) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing hits data");
        exit(EXIT_FAILURE);
    }

    printf("Measuring cache hits...\n");
    for (int i = 0; i < iterations; i++) {
        volatile uint8_t *addr = &array[i % ARRAY_SIZE];  // Access different addresses
        (void)*addr;  // Preload into cache
        uint64_t hit_time = measure_access_time(addr);
        fprintf(file, "%lu\n", hit_time);  // Save hit timing to file
    }

    fclose(file);
}

// Function to measure cache misses and save to a file
void measure_misses(const char *filename, int iterations) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing misses data");
        exit(EXIT_FAILURE);
    }

    printf("Measuring cache misses...\n");
    for (int i = 0; i < iterations; i++) {
        volatile uint8_t *addr = &array[i % ARRAY_SIZE];  // Access different addresses
        _mm_clflush((const void *)addr);  // Flush the cache line
        uint64_t miss_time = measure_access_time(addr);
        fprintf(file, "%lu\n", miss_time);  // Save miss timing to file
    }

    fclose(file);
}

// Function to calculate and display threshold
void calculate_threshold(const char *hit_file, const char *miss_file) {
    FILE *h_fp = fopen(hit_file, "r");
    FILE *m_fp = fopen(miss_file, "r");
    uint64_t hit_sum = 0, miss_sum = 0, hit_count = 0, miss_count = 0, val;

    while (fscanf(h_fp, "%lu", &val) != EOF) {
        hit_sum += val;
        hit_count++;
    }
    while (fscanf(m_fp, "%lu", &val) != EOF) {
        miss_sum += val;
        miss_count++;
    }
    fclose(h_fp);
    fclose(m_fp);

    uint64_t hit_avg = hit_sum / hit_count;
    uint64_t miss_avg = miss_sum / miss_count;
    uint64_t threshold = (hit_avg + miss_avg) / 2;

    printf("Hit Avg: %lu cycles, Miss Avg: %lu cycles, Threshold: %lu cycles\n",
           hit_avg, miss_avg, threshold);
}

int main() {
    const int iterations = 10000;

    // Verify alignment
    if ((uintptr_t)&array % CACHE_LINE_SIZE != 0) {
        fprintf(stderr, "Error: Memory is not aligned to cache line size.\n");
        return EXIT_FAILURE;
    }
    printf("Memory aligned at: %p\n", (void *)&array[0]);

    // Measure cache hits and misses
    measure_hits("hits_data.txt", iterations);
    measure_misses("misses_data.txt", iterations);

    // Calculate and display threshold
    calculate_threshold("hits_data.txt", "misses_data.txt");

    printf("Data collection complete. Results saved to 'hits_data.txt' and 'misses_data.txt'.\n");
    return 0;
}
