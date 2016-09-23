

#ifndef NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
#define NEIRONS_NETWORK_MEMMORY_DISPATCHER_H


#include <cstddef>
#include "mmemory.h"
#include "segment.h"
#include "memory_pager.h"
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
    memory_pager *pager;

    int page_size = 0;

    memory_dispatcher(size_t page_amount, size_t page_size);

    int malloc(VA *ptr, size_t segment_size);

    bool check_enough_memory(size_t required_size);

    int allocate_memory(VA *ptr, size_t segment_size);

    size_t get_required_pages_amount(int required_size);

    segment create_new_segment(size_t segment_size);

    int write(VA ptr, void *buffer_ptr, size_t buffer_size);

    int get_segment(segment &segment_ptr, size_t &segment_offset, VA memory_offset);

    int read(VA ptr, void *buffer, size_t buffer_size);

    int free(VA segment_ptr);
};


#endif //NEIRONS_NETWORK_MEMMORY_DISPATCHER_H
