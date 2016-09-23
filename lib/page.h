//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_PAGE_H
#define NEIRONS_NETWORK_PAGE_H

#include "mmemory.h"

struct page {
public:
    size_t offset;

    bool in_memory;

    page() {};

    page(size_t offset, bool in_memory);
};


#endif //NEIRONS_NETWORK_PAGE_H
