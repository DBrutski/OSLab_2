//
// Created by rizhi-kote on 22.9.16.
//

#include <cstring>
#include <cmath>
#include "segment.h"

segment::segment(int segment_offset, size_t segment_size, size_t page_size, size_t pages_amount) {
    this->segment_offset = segment_offset;
    this->pNext = NULL;
    this->segment_size = segment_size;
    this->page_size = page_size;
    init_pages_offset(page_size);
    this->pages_amount = pages_amount;
}


void segment::init_pages_offset(size_t page_size) {
    this->page_offset_mask = page_size - 1;
    this->page_offset_bits = log2(page_size);
}

int segment::write_buffer_to_segment(size_t segment_offset, void *buffer_ptr, size_t buffer_size) {
    size_t page_number = segment_offset >> this->page_offset_bits;
    size_t first_page_offset = segment_offset & this->page_offset_mask;

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;

    while (buffer_offset < buffer_size) {
        page page = this->pages[page_number];
        page.write(page_offset, (char *) buffer_ptr + buffer_offset, this->page_size - page_offset);
        buffer_offset += (this->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

int segment::read_buffer_from_segment(size_t segment_offset, void *buffer_ptr, size_t buffer_size) {
    size_t page_number = segment_offset >> this->page_offset_bits;
    size_t first_page_offset = segment_offset & this->page_offset_mask;

    page page = this->pages[page_number];

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;
    while (buffer_offset < buffer_size) {

        page.read(page_offset, (char *) buffer_ptr + buffer_offset, this->page_size - page_offset);
        buffer_offset += (this->page_size - page_offset);
        page_offset = 0;
        page = this->pages[++page_number];
    }
    return 0;
}
