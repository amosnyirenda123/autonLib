#ifndef AUTON_MEM_H
#define AUTON_MEM_H

#include <stdio.h>


void* auton_malloc(size_t size);
void* auton_calloc(size_t size, int count);
void* auton_realloc(size_t new_size, void* ptr);
void auton_free(void* ptr);

#endif //AUTON_MEM_H