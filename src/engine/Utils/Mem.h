//
// Created by vd on 29.12.22.
//

#ifndef ENGINE_MEM_H
#define ENGINE_MEM_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void* aligned_alloc_custom(size_t alignment, size_t size, bool zero);
void aligned_free(void* aligned_ptr);

#endif //ENGINE_MEM_H
