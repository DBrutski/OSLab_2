//
// Created by rizhi-kote on 23.9.16.
//

#include "queue.h"

queue *create_queue() {
    queue *out_queue = (queue *) malloc(sizeof(queue));
    out_queue->last = NULL;
    out_queue->first = NULL;
    out_queue->size = 0;
    return out_queue;
}

void queue_push(queue *self, page *new_page) {
    queue_node *node = create_queue_node();
    node->data = new_page;
    if (self->last == NULL) {
        self->last = node;
        self->first = node;
    } else {
        node->next_p = self->last;
        self->last->previos_p = node;
        self->last = node;
    }
    self->size++;
}

page *queue_pop(queue *self) {
    if (self->first == NULL) {
        return NULL;
    }
    page *out;
    if (self->size == 1) {
        out = self->first->data;
    } else {
        queue_node *first_node = self->first;
        self->first = first_node->previos_p;
        self->first->next_p = NULL;
        self->size--;
        out = first_node->data;
        free(first_node);
    }
    return out;
}

size_type queue_size(queue *self) {
    return self->size;
}

queue_node *create_queue_node() {
    queue_node *node = (queue_node *) malloc(sizeof(queue_node));
    node->next_p = NULL;
    node->previos_p = NULL;
    return node;
}

void free_queue(queue *queue1) {
    queue_node *node = queue1->first;
    if (node != NULL) {
        while (node->next_p != NULL) {
            queue_node *node_to_free = node;
            node = node->next_p;
            free(node_to_free);
        }
    }
    free(queue1);
}
