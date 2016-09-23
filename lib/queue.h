//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_QUEUE_H
#define NEIRONS_NETWORK_QUEUE_H

#include "mmemory.h"
#include "map.h"
#include "page.h"

struct queue_node {
    struct queue_node *next_p;
    struct queue_node *previos_p;
    struct page data;
};

struct queue_node create_queue_node(){
    struct queue_node node;
    node.next_p = NULL;
    node.previos_p = NULL;
    return node;
}

struct queue {
    struct queue_node *head, *back;
    size_t size;
};

struct queue create_queue();

void queue_push(struct queue *self, struct page new_page);

struct page * queue_pop(struct queue *self);

size_t queue_size(struct queue *self);

#endif //NEIRONS_NETWORK_QUEUE_H
