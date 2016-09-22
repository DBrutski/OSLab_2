//
// Created by rizhi-kote on 22.9.16.
//

#include <cmath>
#include "memory_dispatcher.h"

memory_dispatcher::memory_dispatcher(size_t page_amount, size_t page_size) {
    this->init_pages_offset(page_size);
    this->allocated_pages_amount = page_amount;
    this->page_size = page_size;
    this->allocated_buffer = new char[page_amount * page_size];
    this->reserved_pages_amount = 0;
}

void memory_dispatcher::init_pages_offset(size_t page_size) {
    this->page_offset_mask = page_size - 1;
    this->page_offset_bits = log2(page_size);
}

int memory_dispatcher::malloc(VA *ptr, size_t segment_size) {
    if (!this->check_enough_memory(segment_size)) { return NOT_ENOUGH_MEMORY_ERROR; }
    int err = this->allocate_memory(ptr, segment_size);
    return err;
}

bool memory_dispatcher::check_enough_memory(size_t required_size) {
    double required_pages_amount = get_required_pages_amount(required_size);
    return (this->allocated_pages_amount - this->reserved_pages_amount) >= required_pages_amount;
}

int memory_dispatcher::allocate_memory(VA *ptr, size_t segment_size) {
    segment free_segment = get_first_free_segment(segment_size);
    VA asd = calculate_segment_ptr(free_segment);
    *ptr = calculate_virtual_offset(free_segment);
    return 0;
}

size_t memory_dispatcher::get_required_pages_amount(int required_size) {
    int required_pages_amount = required_size / this->page_size;
    int rest = required_pages_amount % this->page_size == 0 ? 0 : 1;
    required_pages_amount += rest;
    return required_pages_amount;
}

segment memory_dispatcher::get_first_free_segment(size_t segment_size) {
    segment *existed_segment = this->first_segment;
    if (existed_segment == NULL) {
        segment *ptr = getSegment(segment_size);
        this->first_segment = ptr;
        return *ptr;
    }

    while (existed_segment->pNext != NULL) {
        existed_segment = existed_segment->pNext;
    }
    segment *ptr = getSegment(segment_size);
    existed_segment->pNext = ptr;
    return *ptr;
}

segment *memory_dispatcher::getSegment(size_t segment_size) {
    segment *ptr = new segment(0, segment_size);
    size_t required_pages = this->get_required_pages_amount(segment_size);
    page *segment_pages = allocate_pages(required_pages, 0);
    return ptr;
}

VA memory_dispatcher::calculate_segment_ptr(segment current_segment) {
    return calculate_virtual_offset(current_segment);
}

VA memory_dispatcher::calculate_virtual_offset(segment segment_ptr) {
    size_t offset = 0;
    segment *current_segment = this->first_segment;
    while (current_segment != &segment_ptr) {
        offset += current_segment->segment_size;
        current_segment = current_segment->pNext;
    }
    return (VA) this->allocated_buffer + offset;
}

page *memory_dispatcher::allocate_pages(int pages_amount, size_t offset) {
    page *pages = new page[];
    for (int i = 0; i < pages_amount; i++) {
        pages[i].page_size = this->page_size;
        pages[i].buffer_pointer = this->allocated_buffer + offset + this->page_size * i;
    }
    return pages;
}

int memory_dispatcher::write(VA ptr, void *buffer_ptr, size_t buffer_size) {
    segment *segment_ptr;
    size_t segment_offset;
    int err = get_segment(&segment_ptr, &segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (!segment_offset + buffer_size >= segment_ptr->segment_size) {
        return OUT_OF_RANGE_ERROR;
    }
    err = write_buffer_to_segment(*segment_ptr, segment_offset, buffer_ptr, buffer_size);
    return err;
}

int memory_dispatcher::get_segment(segment **segment_ptr, size_t *segment_offset, VA memory_offset) {
    size_t offset = memory_offset - this->allocated_buffer;
    if (offset < 0 || offset >= this->allocated_pages_amount * this->page_size) {
        return OUT_OF_RANGE_ERROR;
    }
    segment *current_segment = this->first_segment;

    while (offset > 0 + current_segment->segment_size) {
        offset -= current_segment->segment_size;
        if (offset > 0) {
            if (current_segment->pNext == NULL) {
                return OUT_OF_RANGE_ERROR;
            }
            current_segment = current_segment->pNext;
        }
    }
    *segment_offset = offset;
    *segment_ptr = current_segment;
    return SUCCESSFUL_CODE;
}

int memory_dispatcher::write_buffer_to_segment(segment segment, size_t first_segment_offset, void *buffer_ptr,
                                               size_t buffer_size) {
    return 0;
}
