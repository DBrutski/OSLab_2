//
// Created by rizhi-kote on 22.9.16.
//

#include "memory_dispatcher.h"

int dispatcher_malloc( memory_dispatcher *self, VA *ptr, size_t segment_size) {
    if (!check_enough_memory(self, segment_size)) { return NOT_ENOUGH_MEMORY_ERROR; }
    int err = allocate_memory(self, ptr, segment_size);
    return err;
}

bool check_enough_memory( memory_dispatcher *self, size_t required_size) {
    return is_memory_enought(self->pager, required_size);
}

int allocate_memory( memory_dispatcher *self, VA *ptr, size_t segment_size) {
     segment free_segment = create_new_segment(self, segment_size);
    *ptr = (VA) free_segment.segment_begin;
    return 0;
}

 segment create_new_segment( memory_dispatcher *self, size_t segment_size) {
     segment ptr;
    if (map_size(self->segments) == 0) {
        ptr = pager_malloc(self->pager, 0, segment_size);
    } else {
         segment last_segment = map_last(self->segments);
        ptr = pager_malloc(self->pager, last_segment.segment_end, segment_size);
    }
    map_insert(self->segments, ptr.segment_begin, ptr);
    return ptr;
}

int dispatcher_write( memory_dispatcher *self, VA ptr, void *buffer_ptr, size_t buffer_size) {
     segment segment_ptr;
    size_t segment_offset;
    int err = get_segment(self, &segment_ptr, segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (segment_offset + buffer_size > segment_ptr.segment_size) {
        return OUT_OF_RANGE_ERROR;
    }
    pager_write(self->pager, segment_ptr.segment_begin + segment_offset, (char *) buffer_ptr, buffer_size);
    return SUCCESSFUL_CODE;
}

int get_segment( memory_dispatcher *self,  segment *segment_ptr, size_t *segment_offset, VA memory_offset) {
    size_t offset = (size_t) memory_offset;
    is_offset_in_range(self->pager, offset);
     segment *current_segment = find_less_or_equal(self->segments, offset);
    *segment_ptr = *current_segment;
    *segment_offset = offset;
    return SUCCESSFUL_CODE;
}

int dispatcher_read( memory_dispatcher *self, VA ptr, void *buffer, size_t buffer_size) {
     segment segment;
    size_t segment_offset;
    int err = get_segment(self, &segment, segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (segment_offset + buffer_size > segment.segment_size) {
        return OUT_OF_RANGE_ERROR;
    }

    err = pager_read(self->pager, segment.segment_begin + segment_offset, (char *) buffer, buffer_size);
    return err;
}

int dispatcher_free( memory_dispatcher *self, VA segment_ptr) {
    return 0;
}

memory_dispatcher *create_memory_dispatcher(size_t page_amount, size_t page_size) {
    memory_dispatcher dispatcher;
    dispatcher.page_size = page_size;
    dispatcher.pager = create_memory_pager(page_size, page_amount, 0);
    return &dispatcher;
}
