
#include "mmemory.h"
#include "memory_dispatcher.h"

memory_dispatcher *dispatcher = 0;

int _malloc(VA *ptr, size_type szBlock) {
    if (!check_enough_memory(dispatcher, szBlock)) { return NOT_ENOUGH_MEMORY_ERROR; }
    int err = allocate_memory(dispatcher, ptr, szBlock);
    return err;
}

int _free(VA ptr) {
    segment *freed_segment;
    size_type offset;
    int err = get_segment(dispatcher, &freed_segment, &offset, ptr);
    if (err) {
        return err;
    }
    err = pager_free(dispatcher->pager, freed_segment);
    free_segment(freed_segment);
    return err;
}

int _read(VA ptr, void *buffer_ptr, size_type buffer_size) {
    segment *segment;
    size_type segment_offset;
    int err = get_segment(dispatcher, &segment, &segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (segment_offset + buffer_size > segment->segment_size) {
        return OUT_OF_RANGE_ERROR;
    }

    err = pager_read(dispatcher->pager, segment, segment_offset, (char *) buffer_ptr, buffer_size);
    return err;

}


int _write(VA ptr, void *buffer_ptr, size_type buffer_size) {
    segment *segment_ptr = (segment *) malloc(sizeof(segment_ptr));
    size_type in_segment_offset;
    int err = get_segment(dispatcher, &segment_ptr, &in_segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (in_segment_offset + buffer_size > segment_ptr->segment_size) {
        return OUT_OF_RANGE_ERROR;
    }
    if (is_ptr_dispatchers_addres_aria(dispatcher, buffer_ptr)) {
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
        pager_write(dispatcher->pager, segment_ptr, in_segment_offset, (char *) buffer_ptr, buffer_size);
    }
    return SUCCESSFUL_CODE;
}

boolean isPowOfTwo(size_type number) {
    return !(number & (number - 1));
}

/*
 * half of pages to external*/
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
    dispatcher->segments = create_map();
    dispatcher->pager = create_memory_pager(szPage, in_memory, external);
    if (dispatcher == NULL) { return UNKNOWN_ERROR; }
    return 0;
}