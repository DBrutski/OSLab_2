//
// Created by rizhi-kote on 23.9.16.
//

#include "queue.h"

 queue * create_queue() {
     queue out_queue;
    out_queue.back = NULL;
    out_queue.head = NULL;
    out_queue.size = 0;
    return &out_queue;
}

void queue_push( queue *self,  page new_page) {
     queue_node node = create_queue_node();
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

 page * queue_pop( queue *self) {
    if (self->head == NULL) {
        return NULL;
    }
    self->head = self->head->previos_p;
    return &self->head->next_p->data;
}

size_t queue_size( queue *self) {
    return self->size;
}

queue_node create_queue_node() {
    queue_node node;
    node.next_p = NULL;
    node.previos_p = NULL;
    return node;
}
