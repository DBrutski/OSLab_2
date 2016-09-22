//
// Created by rizhi-kote on 22.9.16.
//

#ifndef NEIRONS_NETWORK_PAGE_H
#define NEIRONS_NETWORK_PAGE_H

#include "mmemory.h"

class page {
public:
    bool in_memory = true;
    char *buffer_pointer = NULL;
    size_t page_size = 0;

    void write(size_t offset, char *buffer, size_t buffer_size);

    void read(size_t offset, char *buffer_ptr, size_t buffer_size);
};


#endif //NEIRONS_NETWORK_PAGE_H
