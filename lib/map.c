//
// Created by rizhi-kote on 23.9.16.
//

#include "map.h"

map *create_map() {
    map *new_map = malloc(sizeof(map));
    new_map->first_node = NULL;
    new_map->last_node = NULL;
    new_map->length = 0;
    return new_map;
}

map_node *create_list_node() {
    map_node *node = malloc(sizeof(map_node));;
    node->next_p = NULL;
    node->previos_p = NULL;
    return node;
}


void map_insert(map *self, size_type key, segment *data) {
    map_node *node = self->first_node;
    map_node *new_node = create_list_node();
    if (node == NULL) {
        new_node->key = key;
        new_node->data = data;
        self->first_node = new_node;
        self->last_node = new_node;
        self->length++;
        return;
    }
    while (node->next_p != NULL && node->key < key) {
        node = node->next_p;
    }
    new_node->key = key;
    new_node->data = data;
    if (node->next_p == NULL) {
        self->last_node = new_node;
    }
    new_node->previos_p = node;
    node->next_p = new_node;
    self->length++;
}

size_type map_size(map *self) {
    return self->length;
}

segment *find_less_or_equal(map *self, size_type key) {
    map_node *node = self->first_node;
    if (node == NULL) {
        return NULL;
    }
    while (node->next_p != NULL && node->key < key) {
        node = node->next_p;
    }
    return node->data;
}

segment *map_last(map *self) {
    return self->last_node->data;
}

void free_map(map *map1) {
    map_node *node = map1->first_node;
    if (node != NULL) {
        while (node->next_p != NULL) {
            map_node *node_for_free = node;
            node = node->next_p;
            free(node_for_free);
        }
    }
    free(map1);
}
