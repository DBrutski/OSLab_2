//
// Created by rizhi-kote on 2.10.16.
//

#include <string.h>
#include <stdlib.h>
#include "external_pager.h"

queue *create_external_pages_pull(memory_pager *self, size_type pages_amount, size_type page_size);

memory_pager *create_external_pager(size_type pages_amount, size_type page_size) {
    memory_pager *pager = (memory_pager *) malloc(sizeof(memory_pager));
    pager->allocated_memory = (char *) malloc(sizeof(char) * pages_amount * page_size);
    pager->page_size = page_size;
    pager->allocated_pages_amount = pages_amount;
    pager->free_in_memory_pages = create_external_pages_pull(pager, pages_amount, page_size);
    pager->out_pager = NULL;
    return pager;
}

queue *create_external_pages_pull(memory_pager *self, size_type pages_amount, size_type page_size) {
    queue *pull = create_queue(pages_amount);
    int i;
    for (i = 0; i < pages_amount; i++) {
        page *new_page = create_page(i * page_size, true);
        new_page->is_in_memmory = false;
        queue_push(pull, new_page);
    }
    return pull;
}

page *malloc_page(memory_pager *self) {
    return queue_pop(self->free_in_memory_pages);
}

void write_external_page(memory_pager *self, page *dest_page, char *buffer) {
    memcpy(self->allocated_memory + dest_page->offset, buffer, self->page_size);
}

void read_external_page(memory_pager *self, page *src_page, char *buffer) {
    memcpy(buffer, self->allocated_memory + src_page->offset, self->page_size);
}

void free_external_page(memory_pager *self, page *freed_page) {
    queue_push(self->free_in_memory_pages, freed_page);
}


