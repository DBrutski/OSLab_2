//
// Created by rizhi-kote on 22.9.16.
//

#include "memory_dispatcher.h"

memory_dispatcher::memory_dispatcher(size_t page_amount, size_t page_size) {
    this->allocated_pages_amount = page_amount;
    this->page_size = page_size;
    this->allocated_buffer = new char[page_amount * page_size];
    this->reserved_pages_amount = 0;
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
    segment free_segment = create_new_segment(segment_size);
    *ptr = (VA) free_segment.segment_offset;
    return 0;
}

size_t memory_dispatcher::get_required_pages_amount(int required_size) {
    int required_pages_amount = required_size / this->page_size;
    int rest = required_size % this->page_size == 0 ? 0 : 1;
    required_pages_amount += rest;
    return required_pages_amount;
}

segment memory_dispatcher::create_new_segment(size_t segment_size) {
    segment *ptr;
    if (segments.size() == 0) {
        ptr = create_segment(0, segment_size);
    } else {
        segment last_segment = (*segments.rbegin()).second;
        ptr = create_segment(last_segment.segment_offset + last_segment.pages_amount * last_segment.page_size,
                             segment_size);
    }
    segments.insert(pair<size_t, segment>(ptr->segment_offset, *ptr));
    return *ptr;
}

segment *memory_dispatcher::create_segment(size_t segment_offset, size_t segment_size) {
    size_t required_pages = this->get_required_pages_amount(segment_size);
    segment *ptr = new segment(segment_offset, segment_size, this->page_size, required_pages);
    page *segment_pages = allocate_pages(segment_offset, required_pages);
    ptr->pages = segment_pages;
    return ptr;
}

page *memory_dispatcher::allocate_pages(size_t offset, int pages_amount) {
    page *pages = new page[pages_amount];
    this->reserved_pages_amount += pages_amount;
    for (int i = 0; i < pages_amount; i++) {
        pages[i].page_size = this->page_size;
        pages[i].buffer_pointer = this->allocated_buffer + offset + this->page_size * i;
    }
    return pages;
}

int memory_dispatcher::write(VA ptr, void *buffer_ptr, size_t buffer_size) {
    segment segment_ptr;
    size_t segment_offset;
    int err = get_segment(segment_ptr, segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (segment_offset + buffer_size > segment_ptr.segment_size) {
        return OUT_OF_RANGE_ERROR;
    }
    err = segment_ptr.write_buffer_to_segment(segment_offset, buffer_ptr, buffer_size);
    return err;
}

int memory_dispatcher::get_segment(segment &segment_ptr, size_t &segment_offset, VA memory_offset) {
    size_t offset = (size_t) memory_offset;
    if (offset < 0 || offset >= this->allocated_pages_amount * this->page_size) {
        return OUT_OF_RANGE_ERROR;
    }
    pair<size_t, segment> current_segment = (*--segments.upper_bound(offset));
    segment_ptr = current_segment.second;
    segment_offset = offset;
    return SUCCESSFUL_CODE;
}

int memory_dispatcher::write_buffer_to_segment(segment segment, size_t first_segment_offset, void *buffer_ptr,
                                               size_t buffer_size) {
    return 0;
}

int memory_dispatcher::read(VA ptr, void *buffer_ptr, size_t buffer_size) {
    segment segment_ptr;
    size_t segment_offset;
    int err = get_segment(segment_ptr, segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (segment_offset + buffer_size > segment_ptr.segment_size) {
        return OUT_OF_RANGE_ERROR;
    }

    err = segment_ptr.read_buffer_from_segment(segment_offset, buffer_ptr, buffer_size);
    return err;
}

int memory_dispatcher::free(VA segment_ptr) {


    return 0;
}
