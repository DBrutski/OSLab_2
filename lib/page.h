//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_PAGE_H
#define NEIRONS_NETWORK_PAGE_H

#include "mmemory.h"

struct page {
    size_t offset;

    boolean in_memory;

};

struct page create_page(size_t offset, boolean in_memory);
#endif //NEIRONS_NETWORK_PAGE_H
