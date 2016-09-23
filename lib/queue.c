//
// Created by rizhi-kote on 23.9.16.
//

#include "queue.h"

struct queue create_queue() {
    struct queue out_queue;
    out_queue.back = NULL;
    out_queue.head = NULL;
    out_queue.size = 0;
    return out_queue;
}

void queue_push(struct queue *self, struct page new_page) {
    struct queue_node node = create_queue_node();
    node.data = new_page;
    if (self->back == NULL) {
        self->back = &node;
        self->head = &node;
    } else {
        node.next_p = self->back;
        self->back = &node;
    }
    self->size++;
}

struct page * queue_pop(struct queue *self) {
    if (self->head == NULL) {
        return NULL;
    }
    self->head = self->head->previos_p;
    return &self->head->next_p->data;
}

size_t queue_size(struct queue *self) {
    return self->size;
}