//
// Created by rizhi-kote on 22.9.16.
//

#include <malloc.h>
#include "segment.h"


segment *create_segment(size_type segment_size, size_type segment_begin, size_type segment_end, size_type pages_amount,
                        page **pages) {
    segment *new_segment = (segment *) malloc(sizeof(segment));
    new_segment->segment_size = segment_size;
    new_segment->segment_begin = segment_begin;
    new_segment->segment_end = segment_end;
    new_segment->pages_amount = pages_amount;
    new_segment->pages = pages;
    return new_segment;
}
