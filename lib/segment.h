//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_SEGMENT_H
#define NEIRONS_NETWORK_SEGMENT_H

#include "page.h"

#include "mmemory.h"

class segment {
public:
    struct segment *pNext = NULL;
    size_t segment_size = 0;
    size_t segment_offset = 0;
    page *pages = NULL;
    size_t page_size;

    segment(int segment_offset, size_t segment_size, size_t page_size, size_t pages_amount);

    int write_buffer_to_segment(size_t segment_offset, void *buffer_ptr, size_t buffer_size);

    void init_pages_offset(size_t page_size);

    size_t page_offset_mask;
    size_t page_offset_bits;

    int read_buffer_from_segment(size_t segment_offset, void *buffer_ptr, size_t buffer_size);

    size_t pages_amount;
    ~segment(){
        delete pNext;
    }
};


#endif //NEIRONS_NETWORK_SEGMENT_H
