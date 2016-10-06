
#include <stddef.h>
#include <stdlib.h>
#include "mmemory.h"
#include "memory_dispatcher.h"

memory_dispatcher *dispatcher = 0;

int _malloc(VA *ptr, size_type szBlock) {
    if (!check_enough_memory(dispatcher, szBlock)) { return NOT_ENOUGH_MEMORY_ERROR; }
    int i = 0;
    while (dispatcher->segments[i] != NULL) i++;
    dispatcher->segments[i] = pager_malloc(dispatcher->pager, 0, szBlock);

    memory_address *address = create_memory_address();
    address->segment_num = i;
    *ptr = get_virtual_address(address);
    free(address);
    return 0;
}

int _free(VA ptr) {
    memory_address address;
    int err = get_segment(dispatcher, &address, ptr);
    if (err) {
        return err;
    }
    segment *freed_segment = dispatcher->segments[address.segment_num];
    dispatcher->segments[address.segment_num] = NULL;
    err = pager_free(dispatcher->pager, freed_segment);
    free_segment(freed_segment);
    return err;
}

int _read(VA ptr, void *buffer_ptr, size_type buffer_size) {
    memory_address address;
    int err = get_segment(dispatcher, &address, ptr);
    if (err != 0) {
        return err;
    }

    if (is_offset_in_range(dispatcher, &address)) {
        return OUT_OF_RANGE_ERROR;
    }

    err = pager_read(dispatcher->pager, dispatcher->segments[address.segment_num], &address, (char *) buffer_ptr,
                     buffer_size);
    return err;
}


int _write(VA ptr, void *buffer_ptr, size_type buffer_size) {
    memory_address address;
    int err = get_segment(dispatcher, &address, ptr);
    if (err != 0) {
        return err;
    }

    if (is_offset_in_range(dispatcher, &address)) {
        return OUT_OF_RANGE_ERROR;
    }
    if (is_ptr_dispatchers_address_aria(buffer_ptr)) {
        char *temp_buffer = (char *) malloc(sizeof(char) * buffer_size);
        err = dispatcher_read(dispatcher, buffer_ptr, temp_buffer, buffer_size);
        if (err) {
            return UNKNOWN_ERROR;
        }
        err = dispatcher_write(dispatcher, ptr, temp_buffer, buffer_size);
        if (err) {
            return UNKNOWN_ERROR;
        }
    } else {
        pager_write(dispatcher->pager, dispatcher->segments[address.segment_num], &address, (char *) buffer_ptr, buffer_size);
    }
    return SUCCESSFUL_CODE;
}

bool isPowOfTwo(size_type number) {
    return !(number & (number - 1));
}

/*
 * half of pages to external
 * */
int ___init(int n, size_type szPage) {
    if (dispatcher != NULL) {
        free_dispatcher(dispatcher);
    }
    if (n <= 0 || szPage <= 0) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    if (!isPowOfTwo(szPage)) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    int in_memory = n / 2 + n % 2;
    int external = n / 2;
    dispatcher = (memory_dispatcher *) malloc(sizeof(memory_dispatcher));
    dispatcher->page_size = szPage;
    dispatcher->segments = (segment **) malloc(sizeof(segment *) * n);
    for (int i = 0; i < n; i++) dispatcher->segments[i] = NULL;
    dispatcher->pager = create_memory_pager(szPage, in_memory, external);
    init_pages_offset(szPage);
    if (dispatcher == NULL) { return UNKNOWN_ERROR; }
    return 0;
}