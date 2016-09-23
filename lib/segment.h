//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_SEGMENT_H
#define NEIRONS_NETWORK_SEGMENT_H

#include "page.h"

#include "mmemory.h"

typedef struct {
    size_t segment_size;
    size_t segment_begin;
    size_t segment_end;
} segment;

segment create_segment(size_t segment_size, size_t segment_begin, size_t segment_end);

#endif //NEIRONS_NETWORK_SEGMENT_H
