//
// Created by rizhi-kote on 22.9.16.
//

#include <cstring>
#include "page.h"

void page::write(size_t offset, char *buffer, size_t buffer_size) {
    memcpy(this->buffer_pointer + offset, buffer, buffer_size);
}

void page::read(size_t offset, char *buffer_ptr, size_t buffer_size) {
    memcpy(buffer_ptr, this->buffer_pointer + offset, buffer_size);
}
