/* STRING POOL IMPLEMENTATION
 * Implements string interning to reduce memory usage
 * and speed up string comparisons
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringpool.h"

StringPool string_pool = {0};

void init_string_pool() {
    string_pool.pool = malloc(STRING_POOL_SIZE);
    string_pool.pool_used = 0;
    string_pool.unique_strings = 0;
    string_pool.total_requests = 0;
    string_pool.duplicates_avoided = 0;
    memset(string_pool.buckets, 0, sizeof(string_pool.buckets));
}

char* intern_string(const char* str) {
    if (!str) return NULL;

    string_pool.total_requests++;

    /* Hash the string */
    unsigned int hash = 0;
    const char* p = str;
    while (*p) {
        hash = hash * 31 + *p++;
    }
    hash %= STRING_HASH_SIZE;

    /* Check if already interned */
    StringNode* node = string_pool.buckets[hash];
    while (node) {
        if (strcmp(node->str, str) == 0) {
            string_pool.duplicates_avoided++;
            return node->str;  /* Return existing interned string */
        }
        node = node->next;
    }

    /* Add new string to pool */
    size_t len = strlen(str) + 1;

    /* Check if pool has space */
    if (string_pool.pool_used + len > STRING_POOL_SIZE) {
        /* Pool full, fallback to strdup */
        fprintf(stderr, "WARNING: String pool full, using malloc fallback\n");
        return strdup(str);
    }

    /* Copy string to pool */
    char* interned = string_pool.pool + string_pool.pool_used;
    strcpy(interned, str);
    string_pool.pool_used += len;

    /* Add to hash table */
    StringNode* new_node = malloc(sizeof(StringNode));
    new_node->str = interned;
    new_node->next = string_pool.buckets[hash];
    string_pool.buckets[hash] = new_node;

    string_pool.unique_strings++;
    return interned;
}

void print_string_stats() {
    printf("\n=== String Pool Statistics ===\n");
    printf("Total requests: %d\n", string_pool.total_requests);
    printf("Unique strings: %d\n", string_pool.unique_strings);
    printf("Duplicates avoided: %d\n", string_pool.duplicates_avoided);
    printf("Memory used: %.2f KB / %.2f KB\n",
           string_pool.pool_used / 1024.0,
           STRING_POOL_SIZE / 1024.0);
    printf("Memory saved: ~%.2f KB\n",
           (string_pool.duplicates_avoided * 10) / 1024.0); /* Estimate */
}

void free_string_pool() {
    /* Free hash table nodes */
    for (int i = 0; i < STRING_HASH_SIZE; i++) {
        StringNode* node = string_pool.buckets[i];
        while (node) {
            StringNode* next = node->next;
            free(node);
            node = next;
        }
    }
    /* Free pool */
    if (string_pool.pool) {
        free(string_pool.pool);
    }
}