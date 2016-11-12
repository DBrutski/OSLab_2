//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_PAGER_H
#define NEIRONS_NETWORK_PAGER_H

#include "page.h"
#include "segment.h"
#include "mmemory.h"
#include "queue.h"
#include "memory_address.h"

typedef struct memory_pager{
    size_type allocated_pages_amount;
    char *allocated_memory;

    queue *free_in_memory_pages;
    queue *pages_to_pump_out;

    struct memory_pager *out_pager;


    size_type page_size;
    size_type page_num_first_bit;
    size_type page_offset_mask;
} memory_pager;

memory_pager *create_memory_pager(size_type page_size, size_type in_memory_pages_amount, size_type swap_pages_amount);

void free_pager(memory_pager *pager);

queue *create_inmemory_pages_pull(size_type pages_amount, size_type page_size);

segment *pager_malloc(memory_pager *self, size_type virtual_offset, size_type requred_size);

int pager_write(memory_pager *self, segment *current_segment, memory_address *address, char *buffer,
                size_type buffer_size);

int pager_read(memory_pager *self, segment *current_segment, memory_address *address, char *buffer,
               size_type buffer_size);

int pager_free(memory_pager *self, segment *freed_segment);

size_type get_required_pages_amount(memory_pager *self, int required_size);

void write_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer,
                size_type buffer_size);

bool is_memory_enought(memory_pager *self, size_type required_size);

void read_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer,
               size_type buffer_size);

size_type get_physical_memory_address(memory_pager *self, segment *sgm, size_type offset);

unsigned int counter();

void init_pages_offset(memory_pager *self, size_type page_size);

void page_pump_up(memory_pager *self, page *pumped_in_page);
#endif //NEIRONS_NETWORK_PAGER_H
