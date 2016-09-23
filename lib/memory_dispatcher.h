

#ifndef NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
#define NEIRONS_NETWORK_MEMMORY_DISPATCHER_H

#include <stdbool.h>
#include "mmemory.h"
#include "segment.h"
#include "memory_pager.h"
#include "map.h"

const int SUCCESSFUL_CODE = 0;
const int INCORRECT_PARAMETERS_ERROR = -1;
const int NOT_ENOUGH_MEMORY_ERROR = -2;
const int OUT_OF_RANGE_ERROR = -2;
const int UNKNOWN_ERROR = 1;

struct memory_dispatcher {

    struct map *segments;
    struct memory_pager *pager;

    int page_size;
};

struct memory_dispatcher *create_memory_dispatcher(size_t page_amount, size_t page_size);

int dispatcher_malloc(struct memory_dispatcher *self, VA *ptr, size_t segment_size);

bool check_enough_memory(struct memory_dispatcher *self, size_t required_size);

int allocate_memory(struct memory_dispatcher *self, VA *ptr, size_t segment_size);

struct segment create_new_segment(struct memory_dispatcher *self, size_t segment_size);

int dispatcher_write(struct memory_dispatcher *self, VA ptr, void *buffer_ptr, size_t buffer_size);

int get_segment(struct memory_dispatcher *self, struct segment *segment_ptr, size_t *segment_offset, VA memory_offset);

int dispatcher_read(struct memory_dispatcher *self, VA ptr, void *buffer, size_t buffer_size);

int dispatcher_free(struct memory_dispatcher *self, VA segment_ptr);


#endif //NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
