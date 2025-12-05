#ifndef BENCHMARK_H
#define BENCHMARK_H

/* PERFORMANCE BENCHMARKING FRAMEWORK
 * Cross-platform performance measurement utilities
 * Supports Windows, macOS, and Linux
 */

typedef struct {
    double cpu_time;
    double wall_time;
    long memory_usage;
    long peak_memory;
} BenchmarkResult;

/* Platform-specific memory measurement */
long get_memory_usage();

/* Start a benchmark measurement */
BenchmarkResult* start_benchmark();

/* End benchmark and print results */
void end_benchmark(BenchmarkResult* result, const char* phase);

/* Free benchmark result */
void free_benchmark(BenchmarkResult* result);

#endif