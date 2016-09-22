//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_SEGMENT_H
#define NEIRONS_NETWORK_SEGMENT_H


#include <cstddef>
#include "page.h"

class segment {
public:
    struct segment *pNext = NULL;
    unsigned int segment_size = 0;
    unsigned int offsetBlock = 0;
    page *pages = NULL;

    segment(int segment_offset, size_t segment_size);
};


#endif //NEIRONS_NETWORK_SEGMENT_H
