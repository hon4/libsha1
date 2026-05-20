#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

// Assume sha1 and SHA1_Init, SHA1_Update, SHA1_Final are declared elsewhere

#define NUM_ITERATIONS 1000

// Function to measure time difference in seconds
double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void benchmark_sha1(const uint8_t* data, uint64_t len) {
    struct timespec start, end;
    uint8_t digest[20];

    // Warm-up (optional)
	SHA1(data, len, digest);

    // Start timing
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        SHA1(data, len, digest);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = get_time_diff(start, end);
    printf("Hashing %lu bytes %d times took %.6f seconds\n", len, NUM_ITERATIONS, elapsed);
    printf("Throughput: %.2f MB/sec\n", (len * NUM_ITERATIONS) / (elapsed * 1024 * 1024));
}

int main() {
    // Test data of various sizes
    uint8_t *small_data = malloc(64);   // 64 bytes
    uint8_t *medium_data = malloc(1024); // 1 KB
    uint8_t *large_data = malloc(1024 * 1024); // 1 MB

    // Fill data with random bytes for realism
    for (int i = 0; i < 64; i++) small_data[i] = rand() & 0xFF;
    for (int i = 0; i < 1024; i++) medium_data[i] = rand() & 0xFF;
    for (int i = 0; i < 1024 * 1024; i++) large_data[i] = rand() & 0xFF;

    printf("Benchmarking small data:\n");
    benchmark_sha1(small_data, 64);

    printf("Benchmarking medium data:\n");
    benchmark_sha1(medium_data, 1024);

    printf("Benchmarking large data:\n");
    benchmark_sha1(large_data, 1024 * 1024);

    free(small_data);
    free(medium_data);
    free(large_data);

    return 0;
}
