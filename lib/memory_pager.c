//
// Created by rizhi-kote on 23.9.16.
//

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "memory_pager.h"

struct memory_pager *create_memory_pager(size_t page_size, size_t in_memory_pages_amount, size_t swap_pages_amount) {
    struct memory_pager pager;
    size_t buffer_size = in_memory_pages_amount * page_size;
    pager.allocated_memory = malloc(sizeof(char) * buffer_size);
    pager.page_size = page_size;
    pager.allocated_pages_amount = in_memory_pages_amount + swap_pages_amount;
    pager.pages_virtual_space = malloc(sizeof(struct page) * pager.allocated_pages_amount);
    pager.free_inmemory_pages = create_inmemory_pages_pull(&pager, in_memory_pages_amount, page_size);
    init_pages_offset(&pager, page_size);
    return &pager;
}

struct queue *create_inmemory_pages_pull(struct memory_pager *self, size_t pages_amount,
                                         size_t page_size) {
    struct queue pull = create_queue();
    int i;
    for (i = 0; i < pages_amount; i++) {
        struct page new_page = create_page(i * page_size, true);
        queue_push(self->free_inmemory_pages, new_page);
    }
    return &pull;
}

struct segment pager_malloc(struct memory_pager *self, size_t virtual_offset, size_t requred_size) {
    size_t required_pages_amount = get_required_pages_amount(self, requred_size);

    size_t first_virtual_page_number = virtual_offset >> self->page_offset_bits;
    int i;
    for (i = first_virtual_page_number; i < first_virtual_page_number + required_pages_amount; i++) {
        self->pages_virtual_space[i] = *queue_pop(self->free_inmemory_pages);
    }

    return create_segment(requred_size, virtual_offset, virtual_offset + required_pages_amount * self->page_size);
}

size_t get_required_pages_amount(struct memory_pager *self, int required_size) {
    int required_pages_amount = required_size / self->page_size;
    int rest = required_size % self->page_size == 0 ? 0 : 1;
    required_pages_amount += rest;
    return required_pages_amount;
}


void init_pages_offset(struct memory_pager *self, size_t page_size) {
    self->page_offset_mask = page_size - 1;
    self->page_offset_bits = log2(page_size);
}

int pager_write(struct memory_pager *self, size_t begin_virtual_address, char *buffer, size_t buffer_size) {
    size_t first_page = begin_virtual_address >> self->page_offset_bits;
    size_t require_pages_amount = get_required_pages_amount(self, buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(self, first_page, require_pages_amount);

    size_t first_page_offset = begin_virtual_address & self->page_offset_mask;

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;


    size_t page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_t readen_page_size = buffer_size - buffer_offset < self->page_size ?
                                  buffer_size - buffer_offset : self->page_size;

        struct page page = self->pages_virtual_space[page_number];
        write_page(self, page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (self->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void load_required_pages(struct memory_pager *self, size_t first_page, size_t required_pages_amount) {

}

void
write_page(struct memory_pager *self, struct page current_page, size_t page_offset, char *buffer, size_t buffer_size) {
    memcpy(self->allocated_memory + current_page.offset + page_offset, buffer, buffer_size);
}

bool is_memory_enought(struct memory_pager *self, size_t required_size) {
    size_t required_pages_amount = get_required_pages_amount(self, required_size);
    return required_pages_amount <= queue_size(self->free_inmemory_pages) + queue_size(self->free_swap_pages);
}

bool is_offset_in_range(struct memory_pager *self, size_t offset) {
    return offset < 0 || offset >= self->allocated_pages_amount * self->page_size ? false : true;
}

int read(struct memory_pager *self, size_t begin_virtual_address, char *buffer, size_t buffer_size) {
    size_t first_page = begin_virtual_address >> self->page_offset_bits;
    size_t require_pages_amount = get_required_pages_amount(self, buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(self, first_page, require_pages_amount);

    size_t first_page_offset = begin_virtual_address & self->page_offset_mask;

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;


    size_t page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_t readen_page_size = buffer_size - buffer_offset < self->page_size ?
                                  buffer_size - buffer_offset : self->page_size;

        struct page current_page = self->pages_virtual_space[page_number];
        read_page(self, current_page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (self->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void
read_page(struct memory_pager *self, struct page current_page, size_t page_offset, char *buffer, size_t buffer_size) {
    memcpy(buffer, self->allocated_memory + current_page.offset + page_offset, buffer_size);
}


