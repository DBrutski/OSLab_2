//
// Created by rizhi-kote on 23.9.16.
//

#include "map.h"

void map::insert(size_t key, struct segment data) {
    struct list_node *node = first_node;
    struct list_node new_node = create_list_node();
    if (node == NULL) {
        new_node.key = key;
        new_node.data = data;
        first_node = &new_node;
        last_node = &new_node;
        length++;
        return;
    }
    while (node->next_p != NULL && node->key < key) {
        node = node->next_p;
    }
    if (node->next_p == NULL) {
        last_node = &node;
    }
    new_node.key = key;
    new_node.data = data;
    node->next_p = &new_node;
    length++;
}

size_t map::size() {
    return length;
}

struct segment map::find_less_or_equal(size_t key) {
    struct list_node *node = first_node;
    if (node == NULL) {
        return NULL;
    }
    while (node->next_p != NULL && node->key < key) {
        node = node->next_p;
    }
    return node->data;
}

struct segment map::last() {
    return last_node->data;
}