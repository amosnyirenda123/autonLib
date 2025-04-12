#include "../include/auton_memory.h"
#include <stdlib.h>


void* auton_malloc(size_t size){
    void* ptr = malloc(size);
    if(!ptr){
        fprintf(stderr, "[auton_malloc] Failed to allocate %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void* auton_calloc(size_t size, int count){
    void* ptr = calloc(count, size);

    if(!ptr){
        fprintf(stderr, "[auton_calloc] Failed to allocate %zu bytes\n", size * count);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void* auton_realloc(size_t new_size, void* ptr){
    void* new_ptr = realloc(ptr, new_size);
    if(!new_ptr){
        fprintf(stderr, "[auton_realloc] Failed to allocate %zu bytes\n", new_size);
        exit(EXIT_FAILURE);
    }

    return new_ptr;
}

void auton_free(void* ptr){
    free(ptr);
}