//
// Created by rizhi-kote on 22.9.16.
//

#include <string.h>
#include <jmorecfg.h>
#include <malloc.h>
#include "page.h"

page * create_page(size_type offset, bool in_memory) {
    page *new_page = malloc(sizeof(page));
    new_page->offset = offset;
    new_page->in_memory = in_memory;
    return new_page;
}