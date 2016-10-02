//
// Created by rizhi-kote on 23.9.16.
//

#include <string.h>
#include <stdbool.h>
#include "memory_pager.h"
#include "segment.h"
#include "page.h"

memory_pager *create_memory_pager(size_type page_size, size_type in_memory_pages_amount, size_type swap_pages_amount) {
    memory_pager *pager = (memory_pager *) malloc(sizeof(memory_pager));
    size_type buffer_size = in_memory_pages_amount * page_size;
    pager->allocated_memory = malloc(sizeof(char) * buffer_size);
    pager->page_size = page_size;
    pager->allocated_pages_amount = in_memory_pages_amount + swap_pages_amount;
    pager->free_inmemory_pages = create_inmemory_pages_pull(pager, in_memory_pages_amount, page_size);
    pager->free_swap_pages = create_queue();
    init_pages_offset(pager, page_size);
    return pager;
}

queue *create_inmemory_pages_pull(memory_pager *self, size_type pages_amount,
                                  size_type page_size) {
    queue *pull = create_queue();
    int i;
    for (i = 0; i < pages_amount; i++) {
        page *new_page = create_page(i * page_size, true);
        queue_push(pull, new_page);
    }
    return pull;
}

segment *pager_malloc(memory_pager *self, size_type virtual_offset, size_type requred_size) {
    size_type required_pages_amount = get_required_pages_amount(self, requred_size);

    page **pages = (page **) malloc(sizeof(page *) * required_pages_amount);
    int i;
    for (i = 0; i < required_pages_amount; i++) {
        pages[i] = queue_pop(self->free_inmemory_pages);
    }

    return create_segment(requred_size, virtual_offset, virtual_offset + required_pages_amount * self->page_size,
                          required_pages_amount,
                          pages);
}

size_type get_required_pages_amount(memory_pager *self, int required_size) {
    int required_pages_amount = required_size / self->page_size;
    int rest = required_size % self->page_size == 0 ? 0 : 1;
    required_pages_amount += rest;
    return required_pages_amount;
}

int first_one(int n) {
    int out;
    for (out = 0; n; n >>= 1, out++);
    return out;
}


void init_pages_offset(memory_pager *self, size_type page_size) {
    int casted_page_size = (double) page_size;
    self->page_offset_mask = page_size - 1;
    self->page_offset_bits = first_one(casted_page_size) - 1;
}

int pager_write(memory_pager *self, segment *current_segment, size_type in_segment_oofset, char *buffer,
                size_type buffer_size) {
    size_type first_page = in_segment_oofset >> self->page_offset_bits;
    size_type require_pages_amount = get_required_pages_amount(self, buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(self, first_page, require_pages_amount);

    size_type first_page_offset = in_segment_oofset & self->page_offset_mask;

    size_type page_offset = first_page_offset;
    size_type buffer_offset = 0;


    size_type page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_type readen_page_size = buffer_size - buffer_offset < self->page_size ?
                                     buffer_size - buffer_offset : self->page_size;
        page *page = current_segment->pages[page_number];;
        write_page(self, page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (self->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void load_required_pages(memory_pager *self, size_type first_page, size_type required_pages_amount) {

}

void
write_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer, size_type buffer_size) {
    memcpy(self->allocated_memory + current_page->offset + page_offset, buffer, buffer_size);
}

bool is_memory_enought(memory_pager *self, size_type required_size) {
    size_type required_pages_amount = get_required_pages_amount(self, required_size);
    return required_pages_amount <= queue_size(self->free_inmemory_pages) + queue_size(self->free_swap_pages);
}

int pager_read(memory_pager *self, segment *current_segment, size_type in_segment_offset, char *buffer,
               size_type buffer_size) {
    size_type first_page = in_segment_offset >> self->page_offset_bits;
    size_type require_pages_amount = get_required_pages_amount(self, buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(self, first_page, require_pages_amount);

    size_type first_page_offset = in_segment_offset & self->page_offset_mask;

    size_type page_offset = first_page_offset;
    size_type buffer_offset = 0;


    size_type page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_type readen_page_size = buffer_size - buffer_offset < self->page_size ?
                                     buffer_size - buffer_offset : self->page_size;

        page *current_page = current_segment->pages[page_number];
        read_page(self, current_page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (self->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void
read_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer, size_type buffer_size) {
    memcpy(buffer, self->allocated_memory + current_page->offset + page_offset, buffer_size);
}

void free_pager(memory_pager *pager) {
    free(pager->allocated_memory);
    free_queue(pager->free_inmemory_pages);
    free_queue(pager->free_swap_pages);
    free(pager);
}

int pager_free(memory_pager *self, segment *freed_segment) {
    int pages_it;
    for (pages_it = 0; pages_it < freed_segment->pages_amount; pages_it++) {
        queue_push(self->free_inmemory_pages, freed_segment->pages[pages_it]);
    }
    return 0;
}


