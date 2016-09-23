#include "memory_dispatcher.h"
#include <assert.h>

 memory_dispatcher init_manager(int n, size_t pageSize) {
    return *create_memory_dispatcher(n, pageSize);
}

void check_equal(void *left, void *right, size_t size) {
    int i;
    for (i = 0; i < size; i++) {
        assert(*(char *)left + i == *(char *)right + i);
    }
};

int main() {
     memory_dispatcher dispatcher = init_manager(5, 8);
     memory_pager *pager = dispatcher.pager;

    check_equal(5, pager->allocated_pages_amount, sizeof(size_t));

    VA block;
    int err = dispatcher_malloc(&dispatcher, &block, 8);
    check_equal(0, err, sizeof(size_t));

     segment expected_segment = dispatcher.segments->first_node->data;
    check_equal(1, map_size(dispatcher.segments), 4);
    check_equal(0, expected_segment.segment_begin, 4);
    check_equal(8, expected_segment.segment_size, 4);
    return 0;
}