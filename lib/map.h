//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_MAP_H
#define NEIRONS_NETWORK_MAP_H


#include <stdlib.h>
#include "segment.h"

struct list_node {
    size_t key;
    struct segment data;
    struct list_node *next_p;
    struct list_node *previos_p;
};

struct list_node create_list_node() {
    struct list_node node;
    node.next_p = NULL;
    node.previos_p = NULL;
    return node;
}

struct map {
    size_t length;
    struct list_node *first_node;
    struct list_node *last_node;

    struct segment find_less_or_equal(size_t key);

    void insert(size_t key, struct segment data);

    struct segment last();

    size_t size();
};

struct map create_map() {
    struct map new_map;
    new_map.first_node = NULL;
    new_map.last_node = NULL;
    return new_map;
}

#endif //NEIRONS_NETWORK_MAP_H
