

#ifndef NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
#define NEIRONS_NETWORK_MEMMORY_DISPATCHER_H

#include <stdbool.h>
#include "mmemory.h"
#include "segment.h"
#include "memory_pager.h"
#include "memory_address.h"

#define SUCCESSFUL_CODE  0;
#define INCORRECT_PARAMETERS_ERROR -1;
#define NOT_ENOUGH_MEMORY_ERROR -2;
#define OUT_OF_RANGE_ERROR -2;
#define UNKNOWN_ERROR 1;

typedef struct {

    segment **segments;
    memory_pager *pager;

    int page_size;
} memory_dispatcher;

static const long segment_num_mask = 0xffff00000000;

static const int segment_first_bit = 32;

static const unsigned int segment_offset_mask = 0xffffffff;

static const unsigned long internal_memory_bit = 0x8000000000000000;

void free_dispatcher(memory_dispatcher *dispatcher);

memory_dispatcher *create_memory_dispatcher(size_type page_amount, size_type page_size);

int dispatcher_malloc(memory_dispatcher *self, VA *ptr, size_type segment_size);

bool check_enough_memory(memory_dispatcher *self, size_type required_size);

int dispatcher_write(memory_dispatcher *self, VA block, void *buffer_ptr, size_type buffer_size);

int get_segment(memory_dispatcher *self, memory_address *address, VA memory_offset);

int dispatcher_read(memory_dispatcher *self, VA ptr, void *buffer, size_type buffer_size);

int dispatcher_free(memory_dispatcher *self, VA segment_ptr);

bool is_offset_in_range(memory_dispatcher *self, memory_address *address);

bool is_ptr_dispatchers_address_aria(void *ptr);

memory_address get_memory_address(VA virtual_address);

VA get_virtual_address(memory_address *address);
#endif //NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
