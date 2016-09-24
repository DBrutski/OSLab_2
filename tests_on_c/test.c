#include "memory_dispatcher.h"
#include <assert.h>
#include <printf.h>

memory_dispatcher init_manager(size_type n, size_type pageSize) {
    return *create_memory_dispatcher(n, pageSize);
}

void check_equal_i(int left, int right) {
    int i;
    assert(left == right){
        printf(left, right);
    };
};

int main() {

    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;

    check_equal_i(5, pager->allocated_pages_amount);

    VA block;
    int err = dispatcher_malloc(&dispatcher, &block, 8);
    check_equal_i(0, err);

    segment *expected_segment = dispatcher.segments->first_node->data;
    check_equal_i(1, map_size(dispatcher.segments));
    check_equal_i(0, expected_segment->segment_begin);
    check_equal_i(8, expected_segment->segment_size);
    return 0;
}