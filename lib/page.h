//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_PAGE_H
#define NEIRONS_NETWORK_PAGE_H


#include "mmemory.h"

class page {
public:
    bool in_memory = true;
    VA buffer_pointer = NULL;
    size_t page_size = 0;
};


#endif //NEIRONS_NETWORK_PAGE_H
