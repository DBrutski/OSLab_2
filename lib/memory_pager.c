//
// Created by rizhi-kote on 23.9.16.
//

#include <string.h>
#include <stdbool.h>
#include "memory_pager.h"

memory_pager *create_memory_pager(size_type page_size, size_type in_memory_pages_amount, size_type swap_pages_amount) {
    memory_pager *pager = malloc(sizeof(pager));
    pager->pages_virtual_space = malloc(sizeof(page *) * pager->allocated_pages_amount);
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

    size_type first_virtual_page_number = virtual_offset >> self->page_offset_bits;
    int i;
    for (i = first_virtual_page_number; i < first_virtual_page_number + required_pages_amount; i++) {
        page *new_page = queue_pop(self->free_inmemory_pages);
        self->pages_virtual_space[i] = &new_page;
    }

    return create_segment(requred_size, virtual_offset, virtual_offset + required_pages_amount * self->page_size);
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

int pager_write(memory_pager *self, size_type begin_virtual_address, char *buffer, size_type buffer_size) {
    size_type first_page = begin_virtual_address >> self->page_offset_bits;
    size_type require_pages_amount = get_required_pages_amount(self, buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(self, first_page, require_pages_amount);

    size_type first_page_offset = begin_virtual_address & self->page_offset_mask;

    size_type page_offset = first_page_offset;
    size_type buffer_offset = 0;


    size_type page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_type readen_page_size = buffer_size - buffer_offset < self->page_size ?
                                     buffer_size - buffer_offset : self->page_size;

        page *page = self->pages_virtual_space[page_number];
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
    char *dest_ptr = self->allocated_memory + current_page->offset + page_offset;
    char *src_ptr = buffer;
    char *src_end_ptr = buffer + buffer_size;
    for (; src_ptr < src_end_ptr; src_ptr++, dest_ptr++) {
        *dest_ptr = *src_ptr;
    }
}

bool is_memory_enought(memory_pager *self, size_type required_size) {
    size_type required_pages_amount = get_required_pages_amount(self, required_size);
    return required_pages_amount <= queue_size(self->free_inmemory_pages) + queue_size(self->free_swap_pages);
}

bool is_offset_in_range(memory_pager *self, size_type offset) {
    return offset < 0 || offset >= self->allocated_pages_amount * self->page_size ? false : true;
}

int pager_read(memory_pager *self, size_type begin_virtual_address, char *buffer, size_type buffer_size) {
    size_type first_page = begin_virtual_address >> self->page_offset_bits;
    size_type require_pages_amount = get_required_pages_amount(self, buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(self, first_page, require_pages_amount);

    size_type first_page_offset = begin_virtual_address & self->page_offset_mask;

    size_type page_offset = first_page_offset;
    size_type buffer_offset = 0;


    size_type page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_type readen_page_size = buffer_size - buffer_offset < self->page_size ?
                                     buffer_size - buffer_offset : self->page_size;

        page *current_page = self->pages_virtual_space[page_number];
        read_page(self, current_page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (self->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void
read_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer, size_type buffer_size) {
    memcpy(buffer, &(self->allocated_memory) + current_page->offset + page_offset, buffer_size);
}


