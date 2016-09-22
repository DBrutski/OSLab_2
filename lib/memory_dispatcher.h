

#ifndef NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
#define NEIRONS_NETWORK_MEMMORY_DISPATCHER_H


#include <cstddef>
#include "mmemory.h"
#include "segment.h"

class memory_dispatcher {
public:
    segment *first_segment = NULL;
    int page_size = 0;
    VA allocated_buffer = NULL;

    size_t allocated_pages_amount = 0;
    size_t reserved_pages_amount = 0;

    int page_offset_bits;
    int page_offset_mask;
    memory_dispatcher(size_t page_amount, size_t page_size);
    ~memory_dispatcher() {
        delete[] allocated_buffer;
    }

    void init_pages_offset(size_t page_size);

    int malloc(VA *ptr, size_t segment_size);

    bool check_enough_memory(size_t required_size);

    int allocate_memory(VA *ptr, size_t segment_size);

    size_t get_required_pages_amount(int required_size);

    segment get_first_free_segment(size_t segment_size);

    VA calculate_segment_ptr(segment current_segment);

    VA calculate_virtual_offset(segment segment_ptr);

    page *allocate_pages(int pages_amount, size_t offset);

    segment *getSegment(size_t segment_size);

    int write(VA ptr, void *buffer_ptr, size_t buffer_size);

    int get_segment(segment **segment_ptr, size_t *segment_offset, VA memory_offset);

    int write_buffer_to_segment(segment segment, size_t first_segment_offset, void *buffer_ptr, size_t buffer_size);
};


#endif //NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
