//
// Created by rizhi-kote on 22.9.16.
//

#include <cstring>
#include "segment.h"

segment::segment(size_t segment_size, size_t segment_begin, size_t segment_end) {
    this->segment_size = segment_size;
    this->segment_begin = segment_begin;
    this->segment_end = segment_end;
}
