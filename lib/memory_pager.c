//
// Created by rizhi-kote on 23.9.16.
//

#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include "memory_pager.h"

struct memory_pager * create_memory_pager(size_t page_size, size_t in_memory_pages_amount, size_t swap_pages_amount) {
    struct memory_pager pager;
    size_t buffer_size = in_memory_pages_amount * page_size;
    pager.allocated_memory = malloc(sizeof(char)*buffer_size);
    pager.page_size = page_size;
    pager.allocated_pages_amount = in_memory_pages_amount + swap_pages_amount;
    pager.pages_virtual_space = malloc(sizeof(struct page)*pager.allocated_pages_amount);
    pager.free_inmemory_pages = pager.create_inmemory_pages_pull(in_memory_pages_amount, page_size);
    init_pages_offset(page_size);
    return &pager;
}

struct queue * memory_pager::create_inmemory_pages_pull(size_t pages_amount,
                                                                                                  size_t page_size) {
    struct queue pull = create_queue();
    for (int i = 0; i < pages_amount; i++) {
        struct page new_page(i * page_size, true);
        pull.push(new_page);
    }
    return pull;
}

segment memory_pager::malloc(size_t virtual_offset, size_t requred_size) {
    size_t required_pages_amount = get_required_pages_amount(requred_size);

    size_t first_virtual_page_number = virtual_offset >> page_offset_bits;
    for (int i = first_virtual_page_number; i < first_virtual_page_number + required_pages_amount; i++) {
        pages_virtual_space[i] = free_inmemory_pages.front();
        free_inmemory_pages.pop();
    }

    return segment(requred_size, virtual_offset, virtual_offset + required_pages_amount * page_size);
}

size_t memory_pager::get_required_pages_amount(int required_size) {
    int required_pages_amount = required_size / this->page_size;
    int rest = required_size % this->page_size == 0 ? 0 : 1;
    required_pages_amount += rest;
    return required_pages_amount;
}


void memory_pager::init_pages_offset(size_t page_size) {
    this->page_offset_mask = page_size - 1;
    this->page_offset_bits = log2(page_size);
}

int memory_pager::write(size_t begin_virtual_address, char *buffer, size_t buffer_size) {
    size_t first_page = begin_virtual_address >> this->page_offset_bits;
    size_t require_pages_amount = get_required_pages_amount(buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(first_page, require_pages_amount);

    size_t first_page_offset = begin_virtual_address & this->page_offset_mask;

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;


    size_t page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_t readen_page_size = buffer_size - buffer_offset < page_size ?
                                  buffer_size - buffer_offset : page_size;

        page page = this->pages_virtual_space[page_number];
        write_page(page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (this->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void memory_pager::load_required_pages(size_t first_page, size_t required_pages_amount) {

}

void memory_pager::write_page(page current_page, size_t page_offset, char *buffer, size_t buffer_size) {
    memcpy(allocated_memory + current_page.offset + page_offset, buffer, buffer_size);
}

bool memory_pager::is_memory_enought(size_t required_size) {
    size_t required_pages_amount = get_required_pages_amount(required_size);
    return required_pages_amount <= free_inmemory_pages.size() + free_swap_pages.size();
}

bool memory_pager::is_offset_in_range(size_t offset) {
    return offset < 0 || offset >= allocated_pages_amount * page_size ? false : true;
}

int memory_pager::read(size_t begin_virtual_address, char *buffer, size_t buffer_size) {
    size_t first_page = begin_virtual_address >> this->page_offset_bits;
    size_t require_pages_amount = get_required_pages_amount(buffer_size) + TO_BE_ON_THE_SAFE_SIDE;

    load_required_pages(first_page, require_pages_amount);

    size_t first_page_offset = begin_virtual_address & this->page_offset_mask;

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;


    size_t page_number = first_page;
    while (buffer_offset < buffer_size) {
        size_t readen_page_size = buffer_size - buffer_offset < page_size ?
                                  buffer_size - buffer_offset : page_size;

        page current_page = this->pages_virtual_space[page_number];
        read_page(current_page, page_offset, buffer + buffer_offset, readen_page_size);
        buffer_offset += (this->page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

void memory_pager::read_page(page current_page, size_t page_offset, char *buffer, size_t buffer_size) {
    memcpy(buffer, allocated_memory + current_page.offset + page_offset, buffer_size);
}

