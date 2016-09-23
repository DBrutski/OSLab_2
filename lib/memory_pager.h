//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_PAGER_H
#define NEIRONS_NETWORK_PAGER_H


static const int TO_BE_ON_THE_SAFE_SIDE = 1;

#include <cstddef>
#include <fstream>
#include <queue>
#include "page.h"
#include "segment.h"

using namespace std;

class memory_pager {
public:
    size_t allocated_pages_amount;
    char *allocated_memory = NULL;
    fstream swap_file;
    page *pages_virtual_space;
    queue<page> free_inmemory_pages;
    queue<page> free_swap_pages;
    size_t page_size;

    memory_pager(size_t page_size, size_t in_memory_pages_amount, size_t swap_pages_amount);

    queue<page> create_inmemory_pages_pull(size_t pages_amount, size_t page_size);

    segment malloc(size_t virtual_offset, size_t requred_size);

    int write(size_t begin_virtual_address, char *buffer, size_t buffer_size);

    int read(size_t begin_virtual_address, char *buffer, size_t buffer_size);

    int free(size_t begin_virtual_address, size_t end_virtual_address);

    size_t get_required_pages_amount(int required_size);

    void init_pages_offset(size_t page_size);

    size_t page_offset_mask = 0;
    size_t page_offset_bits = 0;

    void load_required_pages(size_t first_page, size_t required_pages_amount);

    void write_page(page current_page, size_t page_offset, char *buffer, size_t buffer_size);

    bool is_memory_enought(size_t required_size);

    bool is_offset_in_range(size_t offset);

    void read_page(page current_page, size_t page_offset, char *buffer, size_t buffer_size);
};


#endif //NEIRONS_NETWORK_PAGER_H
