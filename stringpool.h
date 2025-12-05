#ifndef STRINGPOOL_H
#define STRINGPOOL_H

/* STRING POOL / INTERNING
 * Eliminates duplicate string allocations for identifiers
 * Strings with same content share one memory location
 * Reduces memory usage and allows fast pointer comparison
 */

#define STRING_POOL_SIZE 16384  /* 16KB pool */
#define STRING_HASH_SIZE 127    /* Prime number for hashing */

typedef struct StringNode {
    char* str;
    struct StringNode* next;
} StringNode;

typedef struct {
    StringNode* buckets[STRING_HASH_SIZE];
    char* pool;
    size_t pool_used;
    int unique_strings;
    int total_requests;
    int duplicates_avoided;
} StringPool;

extern StringPool string_pool;

void init_string_pool();
char* intern_string(const char* str);
void print_string_stats();
void free_string_pool();

#endif