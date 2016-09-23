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

void queue::push(struct page new_page) {
    struct queue_node node = create_queue_node();
    node.data = new_page;
    if (back == NULL) {
        back = &node;
        head = &node;
    } else {
        node.next_p = back;
        back = &node;
    }
    size++;
}

struct page queue::pop() {
    if (head == NULL) {
        return NULL;
    }
    head = head->previos_p;
    return head->next_p->data;
}