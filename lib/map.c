//
// Created by rizhi-kote on 23.9.16.
//

#include "map.h"

void map_insert(struct map *self, size_t key, struct segment data) {
    struct list_node *node = self->first_node;
    struct list_node new_node = create_list_node();
    if (node == NULL) {
        new_node.key = key;
        new_node.data = data;
        self->first_node = &new_node;
        self->last_node = &new_node;
        self->length++;
        return;
    }
    while (node->next_p != NULL && node->key < key) {
        node = node->next_p;
    }
    if (node->next_p == NULL) {
        self->last_node = &node;
    }
    new_node.key = key;
    new_node.data = data;
    node->next_p = &new_node;
    self->length++;
}

size_t map_size(struct map *self) {
    return self->length;
}

struct segment * find_less_or_equal(struct map *self, size_t key) {
    struct list_node *node = self->first_node;
    if (node == NULL) {
        return NULL;
    }
    while (node->next_p != NULL && node->key < key) {
        node = node->next_p;
    }
    return &node->data;
}

struct segment map_last(struct map *self) {
    return self->last_node->data;
}