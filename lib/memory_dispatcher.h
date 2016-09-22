

#ifndef NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
#define NEIRONS_NETWORK_MEMMORY_DISPATCHER_H


#include <cstddef>
#include "mmemory.h"
#include "segment.h"
#include <map>

using namespace std;

const int SUCCESSFUL_CODE = 0;
const int INCORRECT_PARAMETERS_ERROR = -1;
const int NOT_ENOUGH_MEMORY_ERROR = -2;
const int OUT_OF_RANGE_ERROR = -2;
const int UNKNOWN_ERROR = 1;

class memory_dispatcher {
public:


    map<size_t, segment> segments;
    int page_size = 0;
    char *allocated_buffer = NULL;

    size_t allocated_pages_amount = 0;
    size_t reserved_pages_amount = 0;

    int page_offset_bits;
    int page_offset_mask;

    memory_dispatcher(size_t page_amount, size_t page_size);

    ~memory_dispatcher() {
        delete[] allocated_buffer;
    }


    int malloc(VA *ptr, size_t segment_size);

    bool check_enough_memory(size_t required_size);

    int allocate_memory(VA *ptr, size_t segment_size);

    size_t get_required_pages_amount(int required_size);

    segment create_new_segment(size_t segment_size);

    VA calculate_segment_ptr(segment *current_segment);

    VA calculate_virtual_offset(segment *segment_ptr);

    page *allocate_pages(size_t offset, int pages_amount);

    segment *create_segment(size_t segment_offset, size_t segment_size);

    int write(VA ptr, void *buffer_ptr, size_t buffer_size);

    int get_segment(segment &segment_ptr, size_t &segment_offset, VA memory_offset);

    int read(VA ptr, void *buffer_ptr, size_t buffer_size);

    int write_buffer_to_segment(segment segment, size_t first_segment_offset, void *buffer_ptr, size_t buffer_size);

    int free(VA segment_ptr);
};


#endif //NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
