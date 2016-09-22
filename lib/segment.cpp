//
// Created by rizhi-kote on 22.9.16.
//

#include "segment.h"

segment::segment(int segment_offset, size_t segment_size) {
    this->offsetBlock = segment_offset;
    this->pNext = NULL;
    this->segment_size = segment_size;
}
