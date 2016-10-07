//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_QUEUE_H
#define NEIRONS_NETWORK_QUEUE_H

#include "mmemory.h"
#include "page.h"

typedef struct {
    page **pages;
    size_type first, last;
    size_type size;
    size_type max_size;
} queue;

queue * create_queue(size_type max_size);

void free_queue(queue *);

void queue_push(queue *self, page *new_page);

page *queue_pop(queue *self);

void queue_remove(queue *self, page *removing_page);

size_type queue_size(queue *self);

#endif //NEIRONS_NETWORK_QUEUE_H
