//
// Created by rizhi-kote on 23.9.16.
//

#ifndef NEIRONS_NETWORK_MAP_H
#define NEIRONS_NETWORK_MAP_H


#include <stdlib.h>
#include "segment.h"

typedef struct map_node {
    size_type key;
    segment *data;
    struct map_node *next_p;
    struct map_node *previos_p;
} map_node;

map_node * create_list_node();

typedef struct {
    size_type length;
    map_node *first_node;
    map_node *last_node;
} map;

map * create_map();
void free_map(map *map1);

segment *find_less_or_equal(map *self, size_type key);

void map_insert(map *self, size_type key, segment *data);

segment * map_last(map *self);

size_type map_size(map *self);

#endif //NEIRONS_NETWORK_MAP_H
