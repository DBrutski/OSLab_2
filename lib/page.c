//
// Created by rizhi-kote on 22.9.16.
//

#include <string.h>
#include <jmorecfg.h>
#include <malloc.h>
#include "page.h"

page *create_page(size_type offset, bool in_memory) {
    page *new_page = (page *) malloc(sizeof(page));
    new_page->offset = offset;
    new_page->is_in_memmory = in_memory;
    return new_page;
}