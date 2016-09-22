#include "mmemory.h"
#include "memory_dispatcher.h"

memory_dispatcher *dispatcher = NULL;

int _malloc(VA *ptr, size_t szBlock) {
    return dispatcher->malloc(ptr, szBlock);
};

int _free(VA ptr) {
    return dispatcher->free(ptr);
};

int _read(VA ptr, void *buffer_ptr, size_t buffer_size) {
    return dispatcher->read(ptr, buffer_ptr, buffer_size);
};


int _write(VA ptr, void *buffer_ptr, size_t buffer_size) {
    return dispatcher->write(ptr, buffer_ptr, buffer_size);
};

bool isPowOfTwo(size_t number) {
    return !(number & number - 1);
}

int _init(int n, size_t szPage) {
    if (n <= 0 || szPage <= 0) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    if (!isPowOfTwo(szPage)) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    dispatcher = new memory_dispatcher(n, szPage);
    if (dispatcher == NULL) { return UNKNOWN_ERROR; }
    return 0;
};