//
// Created by rizhi-kote on 22.9.16.
//

#include <stdlib.h>
#include "memory_dispatcher.h"



int lowest_bit_number(int n) {
    int out;
    for (out = 0; n; n >>= 1, out++);
    return out;
}

void init_pages_offset(size_type page_size) {
    int casted_page_size = (double) page_size;
    page_offset_mask = page_size - 1;
    page_num_first_bit = lowest_bit_number(casted_page_size) - 1;
}


int dispatcher_malloc(memory_dispatcher *self, VA *ptr, size_type segment_size) {
    if (!check_enough_memory(self, segment_size)) { return NOT_ENOUGH_MEMORY_ERROR; }
    int i = 0;
    while (self->segments[i] != NULL) i++;
    self->segments[i] = pager_malloc(self->pager, 0, segment_size);

    memory_address *address = create_memory_address();
    address->segment_num = i;
    *ptr = get_virtual_address(address);
    free(address);
    return 0;
}

bool check_enough_memory(memory_dispatcher *self, size_type required_size) {
    return is_memory_enought(self->pager, required_size);
}

int dispatcher_write(memory_dispatcher *self, VA block, void *buffer_ptr, size_type buffer_size) {
    memory_address address;
    int err = get_segment(self, &address, block);
    if (err != 0) {
        return err;
    }

    if (is_offset_in_range(self, &address)) {
        return OUT_OF_RANGE_ERROR;
    }
    if (is_ptr_dispatchers_address_aria(buffer_ptr)) {
        char *temp_buffer = (char *) malloc(sizeof(char) * buffer_size);
        err = dispatcher_read(self, buffer_ptr, temp_buffer, buffer_size);
        if (err) {
            return UNKNOWN_ERROR;
        }
        buffer_ptr = temp_buffer;
    }
    pager_write(self->pager, self->segments[address.segment_num], &address, (char *) buffer_ptr, buffer_size);
    return SUCCESSFUL_CODE;
}

bool is_ptr_dispatchers_address_aria(void *ptr) {
    return (unsigned long) ptr & internal_memory_bit;
}

int dispatcher_read(memory_dispatcher *self, VA ptr, void *buffer, size_type buffer_size) {
    memory_address address;
    int err = get_segment(self, &address, ptr);
    if (err != 0) {
        return err;
    }

    if (is_offset_in_range(self, &address)) {
        return OUT_OF_RANGE_ERROR;
    }

    err = pager_read(self->pager, self->segments[address.segment_num], &address, (char *) buffer, buffer_size);
    return err;
}

int dispatcher_free(memory_dispatcher *self, VA segment_ptr) {
    memory_address address;
    int err = get_segment(self, &address, segment_ptr);
    if (err) {
        return err;
    }
    segment *freed_segment = self->segments[address.segment_num];
    self->segments[address.segment_num] = NULL;
    err = pager_free(self->pager, freed_segment);
    free_segment(freed_segment);
    return err;
}

memory_dispatcher *create_memory_dispatcher(size_type page_amount, size_type page_size) {
    int in_memory = page_amount / 2 + page_amount % 2;
    int external = page_amount / 2;

    memory_dispatcher *dispatcher = (memory_dispatcher *) malloc(sizeof(memory_dispatcher));
    dispatcher->page_size = page_size;
    dispatcher->segments = (segment **) malloc(sizeof(segment *) * page_amount);
    for (int i = 0; i < page_amount; i++) dispatcher->segments[i] = NULL;
    dispatcher->pager = create_memory_pager(page_size, in_memory, external);
    init_pages_offset(page_size);
    return dispatcher;
}

void free_dispatcher(memory_dispatcher *dispatcher) {
    free(dispatcher->segments);
    free_pager(dispatcher->pager);
    free(dispatcher);
}

VA get_virtual_address(memory_address *address) {
    unsigned long virtual_address = 0;
    virtual_address |= address->segment_num << segment_first_bit;
    virtual_address |= address->page_num << page_num_first_bit;
    virtual_address |= address->page_offset;
    virtual_address |= internal_memory_bit;
    return (VA) virtual_address;
}

memory_address get_memory_address(VA virtual_address) {
    unsigned long address = (unsigned long) virtual_address;
    memory_address result;
    result.segment_num = (address & segment_num_mask) >> segment_first_bit;
    result.page_num = (address & segment_offset_mask) >> page_num_first_bit;
    result.page_offset = (address & segment_offset_mask) & page_offset_mask;
    return result;
}

bool is_offset_in_range(memory_dispatcher *self, memory_address *address) {
    if (!self->segments[address->segment_num]) return false;
    segment *current_segment = self->segments[address->segment_num];
    return current_segment->segment_size <
           address->page_num * self->page_size + address->page_offset;
}

int get_segment(memory_dispatcher *self, memory_address *address, VA memory_offset) {
    *address = get_memory_address(memory_offset);
    return is_offset_in_range(self, address);
}

