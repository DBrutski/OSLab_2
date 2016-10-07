//
// Created by rizhi-kote on 23.9.16.
//

#include "queue.h"
#include <stdlib.h>

queue *create_queue(size_type max_size) {
    queue *out_queue = (queue *) malloc(sizeof(queue));
    out_queue->pages = (page **) malloc(sizeof(page *) * max_size);
    out_queue->last = 0;
    out_queue->first = 0;
    out_queue->size = 0;
    out_queue->max_size = max_size;
    return out_queue;
}

void queue_push(queue *self, page *new_page) {
    if (self->size != 0) {
        self->last++;
        self->last = self->last % self->max_size;
    }
    self->pages[self->last] = new_page;
    self->size++;
}


void queue_remove(queue *self, page *removing_page) {
    size_type number = 0;
    while (number < self->size && self->pages[number] != removing_page) number++;
    if (number == self->size - 1) return;
    for (; number < self->size; number++)self->pages[number] = self->pages[number + 1];
    self->size--;
    self->last--;
}

page *queue_pop(queue *self) {
    if (self->size == 0) return NULL;
    page *out_page = self->pages[self->first];
    self->first = ++self->first % self->max_size;
    self->size--;
    return out_page;
}

size_type queue_size(queue *self) {
    return self->size;
}

void free_queue(queue *queue1) {
    free(queue1->pages);
    free(queue1);
}
