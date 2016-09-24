//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_PAGER_H
#define NEIRONS_NETWORK_PAGER_H


static const int TO_BE_ON_THE_SAFE_SIDE = 1;

#include <jmorecfg.h>
#include "page.h"
#include "segment.h"
#include "mmemory.h"
#include "queue.h"

typedef struct {
    size_type allocated_pages_amount;
    char *allocated_memory;


    size_type page_offset_mask;
    size_type page_offset_bits;

    page **pages_virtual_space;
    queue *free_inmemory_pages;
    queue *free_swap_pages;
    size_type page_size;
} memory_pager;

 memory_pager *create_memory_pager(size_type page_size, size_type in_memory_pages_amount, size_type swap_pages_amount);

 queue *create_inmemory_pages_pull( memory_pager *self, size_type pages_amount,
                                         size_type page_size);

 segment * pager_malloc(memory_pager *self, size_type virtual_offset, size_type requred_size);

int pager_write( memory_pager *self, size_type begin_virtual_address, char *buffer, size_type buffer_size);

int pager_read( memory_pager *self, size_type begin_virtual_address, char *buffer, size_type buffer_size);

int pager_free( memory_pager *self, size_type begin_virtual_address, size_type end_virtual_address);

size_type get_required_pages_amount( memory_pager *self, int required_size);

void init_pages_offset( memory_pager *self, size_type page_size);


void load_required_pages( memory_pager *self, size_type first_page, size_type required_pages_amount);

void write_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer,
                size_type buffer_size);

bool is_memory_enought( memory_pager *self, size_type required_size);

bool is_offset_in_range( memory_pager *self, size_type offset);

void read_page(memory_pager *self, page *current_page, size_type page_offset, char *buffer,
               size_type buffer_size);

#endif //NEIRONS_NETWORK_PAGER_H
