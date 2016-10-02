//
// Created by rizhi-kote on 2.10.16.
//

#ifndef NEIRONS_NETWORK_EXTERNAL_PAGER_H
#define NEIRONS_NETWORK_EXTERNAL_PAGER_H


#include "mmemory.h"
#include "queue.h"
#include "memory_pager.h"

memory_pager * create_external_pager(size_type pages_amount, size_type page_size);

page *malloc_page(memory_pager *self);
void write_external_page(memory_pager *self, page *dest_page, char *buffer);
void read_external_page(memory_pager *self, page *src_page, char *buffer);
void free_external_page(memory_pager *self, page *freed_page);

#endif //NEIRONS_NETWORK_EXTERNAL_PAGER_H
