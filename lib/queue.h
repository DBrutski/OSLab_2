//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_QUEUE_H
#define NEIRONS_NETWORK_QUEUE_H

#include "mmemory.h"
#include "map.h"
#include "page.h"

typedef struct queue_node{
    struct queue_node *next_p;
    struct queue_node *previos_p;
    page *data;
} queue_node;

queue_node * create_queue_node();

typedef struct {
    queue_node *first, *last;
    size_type size;
} queue;

queue * create_queue();

void queue_push(queue *self, page *new_page);

page *queue_pop(queue *self);

size_type queue_size(queue *self);

#endif //NEIRONS_NETWORK_QUEUE_H
