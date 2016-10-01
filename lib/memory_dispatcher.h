

#ifndef NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
#define NEIRONS_NETWORK_MEMMORY_DISPATCHER_H

#include <stdbool.h>
#include "mmemory.h"
#include "segment.h"
#include "memory_pager.h"
#include "map.h"

#define SUCCESSFUL_CODE  0;
#define INCORRECT_PARAMETERS_ERROR -1;
#define NOT_ENOUGH_MEMORY_ERROR -2;
#define OUT_OF_RANGE_ERROR -2;
#define UNKNOWN_ERROR 1;

typedef struct {

    map *segments;
    memory_pager *pager;

    int page_size;
} memory_dispatcher;

void free_dispatcher(memory_dispatcher *dispatcher);

memory_dispatcher *create_memory_dispatcher(size_type page_amount, size_type page_size);

int dispatcher_malloc(memory_dispatcher *self, VA *ptr, size_type segment_size);

bool check_enough_memory(memory_dispatcher *self, size_type required_size);

int allocate_memory(memory_dispatcher *self, VA *ptr, size_type segment_size);

segment * create_new_segment(memory_dispatcher *self, size_type segment_size);

int dispatcher_write(memory_dispatcher *self, VA block, void *buffer_ptr, size_type buffer_size);

int get_segment(memory_dispatcher *self, segment **segment_ptr, size_type *in_segment_offset, VA memory_offset);

int dispatcher_read(memory_dispatcher *self, VA ptr, void *buffer, size_type buffer_size);

int dispatcher_free(memory_dispatcher *self, VA segment_ptr);


#endif //NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
