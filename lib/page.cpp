//
// Created by rizhi-kote on 22.9.16.
//

#include <cstring>
#include "page.h"

page::page(size_t offset, bool in_memory) {
    this->offset = offset;
    this->in_memory = in_memory;
}
