//
// Created by rizhi-kote on 6.10.16.
//

#ifndef NEIRONS_NETWORK_MEMORY_ADDRESS_H
#define NEIRONS_NETWORK_MEMORY_ADDRESS_H

#include <stdlib.h>
#include "mmemory.h"

typedef struct {
    size_type segment_num;
    size_type segment_offset;
} memory_address;

memory_address *create_memory_address();

#endif //NEIRONS_NETWORK_MEMORY_ADDRESS_H
