
#include "mmemory.h"
#include "memory_dispatcher.h"

memory_dispatcher *dispatcher = 0;

int _malloc(VA *ptr, size_type szBlock) {
    return dispatcher_malloc(dispatcher, ptr, szBlock);
}

int _free(VA ptr) {
    return dispatcher_free(dispatcher, ptr);
}

int _read(VA ptr, void *buffer_ptr, size_type buffer_size) {
    return dispatcher_read(dispatcher, ptr, buffer_ptr, buffer_size);
}


int _write(VA ptr, void *buffer_ptr, size_type buffer_size) {
    return dispatcher_write(dispatcher, ptr, buffer_ptr, buffer_size);
}

boolean isPowOfTwo(size_type number) {
    return !(number & (number - 1));
}

int _init(int n, size_type szPage) {
    if (dispatcher != NULL) {
        free_dispatcher(dispatcher);
    }
    if (n <= 0 || szPage <= 0) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    if (!isPowOfTwo(szPage)) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    dispatcher = create_memory_dispatcher(n, szPage);
    if (dispatcher == NULL) { return UNKNOWN_ERROR; }
    return 0;
}