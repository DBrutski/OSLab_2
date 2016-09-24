//
// Created by rizhi-kote on 22.9.16.
//

#include <malloc.h>
#include "segment.h"



segment * create_segment(size_type segment_size, size_type segment_begin, size_type segment_end) {
    segment *new_segment = malloc(sizeof(segment));
    new_segment->segment_size = segment_size;
    new_segment->segment_begin = segment_begin;
    new_segment->segment_end = segment_end;
    return new_segment;
}
