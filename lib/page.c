//
// Created by rizhi-kote on 22.9.16.
//

#include <string.h>
#include <jmorecfg.h>
#include "page.h"

struct page create_page(size_t offset, bool in_memory) {
    struct page new_page;
    new_page.offset = offset;
    new_page.in_memory = in_memory;
    return new_page;
}