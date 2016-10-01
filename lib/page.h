//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_PAGE_H
#define NEIRONS_NETWORK_PAGE_H

#include "mmemory.h"

typedef struct {
    size_type offset;

    bool is_in_memmory;

} page;

page * create_page(size_type offset, bool in_memory);

#endif //NEIRONS_NETWORK_PAGE_H
