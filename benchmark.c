/* BENCHMARKING IMPLEMENTATION
 * Cross-platform performance measurement
 * Measures CPU time, wall time, and memory usage
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "benchmark.h"

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif __APPLE__
    #include <mach/mach.h>
    #include <mach/mach_time.h>
    #include <sys/time.h>
#else
    #include <sys/time.h>
    #include <sys/resource.h>
    #include <unistd.h>
#endif

/* Platform-specific memory measurement */
long get_memory_usage() {
#ifdef _WIN32
    /* Windows implementation */
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return (long)pmc.WorkingSetSize;
    }
    return 0;
#elif __APPLE__
    /* macOS implementation */
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(),
                                    TASK_BASIC_INFO,
                                    (task_info_t)&info, &size);
    if (kerr == KERN_SUCCESS) {
        return info.resident_size;
    }
    return 0;
#else
    /* Linux implementation */
    FILE* file = fopen("/proc/self/status", "r");
    if (file) {
        char line[128];
        while (fgets(line, 128, file)) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                long memory;
                sscanf(line, "VmRSS: %ld", &memory);
                fclose(file);
                return memory * 1024; /* Convert KB to bytes */
            }
        }
        fclose(file);
    }
    return 0;
#endif
}

BenchmarkResult* start_benchmark() {
    BenchmarkResult* result = malloc(sizeof(BenchmarkResult));

    /* Get initial CPU time */
    clock_t start_cpu = clock();

    /* Get initial wall time */
#ifdef _WIN32
    LARGE_INTEGER frequency, start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    result->wall_time = (double)start.QuadPart / frequency.QuadPart;
#else
    struct timeval start_wall;
    gettimeofday(&start_wall, NULL);
    result->wall_time = start_wall.tv_sec + start_wall.tv_usec / 1000000.0;
#endif

    /* Store initial CPU time */
    result->cpu_time = (double)start_cpu;

    /* Get initial memory */
    result->memory_usage = get_memory_usage();
    result->peak_memory = 0;

    return result;
}

void end_benchmark(BenchmarkResult* result, const char* phase) {
    /* Get final CPU time */
    clock_t end_cpu = clock();

    /* Get final wall time */
#ifdef _WIN32
    LARGE_INTEGER frequency, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&end);
    double end_wall_time = (double)end.QuadPart / frequency.QuadPart;
#else
    struct timeval end_wall;
    gettimeofday(&end_wall, NULL);
    double end_wall_time = end_wall.tv_sec + end_wall.tv_usec / 1000000.0;
#endif

    /* Calculate elapsed times */
    double start_cpu = result->cpu_time;
    result->cpu_time = ((double)(end_cpu - start_cpu)) / CLOCKS_PER_SEC;

    double start_wall = result->wall_time;
    result->wall_time = end_wall_time - start_wall;

    /* Get final memory */
    long final_memory = get_memory_usage();
    result->peak_memory = final_memory - result->memory_usage;

    /* Print results */
    printf("\n=== %s Performance ===\n", phase);
    printf("CPU Time:     %.6f seconds\n", result->cpu_time);
    printf("Wall Time:    %.6f seconds\n", result->wall_time);
    printf("Memory Delta: %.2f KB\n", result->peak_memory / 1024.0);
}

void free_benchmark(BenchmarkResult* result) {
    if (result) {
        free(result);
    }
}