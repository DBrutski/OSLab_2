#define BOOST_TEST_MODULE unit_test

#include <memory_pager.h>
#include <assert.h>
#include <map.h>
#include <memory_dispatcher.h>

memory_dispatcher *init_manager(int n, size_t pageSize) {
    return create_memory_dispatcher(n, pageSize);
}

bool check_equal(size_type l, size_type r) {
    return l == r;
}

bool check_equal_collection(char *begin_l, char *end_l, char *begin_r, char *end_r) {
    if (end_l - begin_l != end_r - begin_r) {
        return false;
    }
    bool is_equal = true;
    char *current_l, *current_r;
    for (current_l = begin_l, current_r = begin_r;
         current_l < end_l && current_r < end_r && is_equal; current_l++, current_r++) {
        is_equal &= *current_l == *current_r;
    }
    return is_equal;
}

void unit_test_init_dispatcher() {
    memory_dispatcher *dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));
    assert(check_equal(0, dispatcher->segments->length));
    free_dispatcher(dispatcher);
}

void unit_test_malloc_block() {
    memory_dispatcher *dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));
    assert(check_equal(0, dispatcher->segments->length));

    VA block;
    int err = dispatcher_malloc(dispatcher, &block, 8);
    assert(check_equal(0, err));

    segment *expected_segment = dispatcher->segments->first_node->data;
    assert(check_equal(1, map_size(dispatcher->segments)));
    assert(check_equal(0, expected_segment->segment_begin));
    assert(check_equal(8, expected_segment->segment_size));
    free_dispatcher(dispatcher);
}

void unit_test_malloc_block_two_blocks() {
    memory_dispatcher *dispatcher = init_manager(5, 4);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));

    VA block1;
    VA block2;

    int err = dispatcher_malloc(dispatcher, &block1, 6);
    assert(check_equal(0, err));

    err = dispatcher_malloc(dispatcher, &block2, 8);
    assert(check_equal(0, err));

    assert(check_equal(0, (size_t) block1));
    assert(check_equal(8, (size_t) block2));
    free_dispatcher(dispatcher);
}

void unit_test_write_buffer_to_one_page() {
    memory_dispatcher *dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));
    assert(check_equal(0, dispatcher->segments->length));

    VA block1;

    int err = dispatcher_malloc(dispatcher, &block1, 16);
    assert(check_equal(0, err));

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(dispatcher, block1, buffer, buffer_size);
    assert(check_equal(0, err));

    VA allocated_buffer = pager->allocated_memory;
    err = dispatcher_read(dispatcher, block1, allocated_buffer, buffer_size);
    check_equal_collection(buffer, buffer + buffer_size,
                           allocated_buffer, allocated_buffer + buffer_size);
    free_dispatcher(dispatcher);
}

void unit_test_write_buffer_to_page_not_in_begin() {
    memory_dispatcher *dispatcher = init_manager(5, 16);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));

    VA block1;

    int err = dispatcher_malloc(dispatcher, &block1, 16);
    assert(check_equal(0, err));

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(dispatcher, block1 + 5, buffer, buffer_size);
    assert(check_equal(0, err));

    VA allocated_buffer = pager->allocated_memory;
    check_equal_collection(buffer, buffer + buffer_size,
                           allocated_buffer + 5, allocated_buffer + 5 + buffer_size);
    free_dispatcher(dispatcher);
}

void unit_test_write_buffer_to_two_page_not_in_begin() {
    memory_dispatcher *dispatcher = init_manager(5, 4);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));

    VA block1;

    int err = dispatcher_malloc(dispatcher, &block1, 16);
    assert(check_equal(0, err));

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(dispatcher, block1 + 2, buffer, buffer_size);
    assert(check_equal(0, err));

    VA allocated_buffer = pager->allocated_memory;
    check_equal_collection(buffer, buffer + buffer_size,
                           allocated_buffer + 2, allocated_buffer + 2 + buffer_size);
    free_dispatcher(dispatcher);
}

void unit_test_read_buffer_from_one_page() {
    memory_dispatcher *dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));

    VA block1;

    int err = dispatcher_malloc(dispatcher, &block1, 16);
    assert(check_equal(0, err));

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(dispatcher, block1, buffer, buffer_size);
    assert(check_equal(0, err));

    char *readen_buffer = malloc(sizeof(char) * buffer_size);
    err = dispatcher_read(dispatcher, block1, readen_buffer, buffer_size);
    assert(check_equal(0, err));

    assert(check_equal_collection(buffer, buffer + buffer_size, readen_buffer, readen_buffer + buffer_size));
    free_dispatcher(dispatcher);
}

void unit_test_read_buffer_from_two_page_not_in_begin() {
    memory_dispatcher *dispatcher = init_manager(5, 4);
    memory_pager *pager = dispatcher->pager;
    assert(check_equal(5, pager->allocated_pages_amount));

    VA block1;

    int err = dispatcher_malloc(dispatcher, &block1, 16);
    assert(check_equal(0, err));

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(dispatcher, block1 + 2, buffer, buffer_size);
    assert(check_equal(0, err));

    char *readen_buffer = malloc(sizeof(char) * buffer_size);
    err = dispatcher_read(dispatcher, block1 + 2, readen_buffer, buffer_size);
    assert(check_equal(0, err));

    assert(check_equal_collection(buffer, buffer + buffer_size, readen_buffer, readen_buffer + buffer_size));
    free_dispatcher(dispatcher);
}

void unit_test_malloc_many_pages() {
    memory_dispatcher *dispatcher = init_manager(5, 8);
    VA block;
    int err = dispatcher_malloc(dispatcher, &block, 30);
    assert(check_equal(0, err));


    assert(check_equal(30, dispatcher->segments->first_node->data->segment_size));
    free_dispatcher(dispatcher);
}

void unit_test_create_segments_and_remove_some() {
    memory_dispatcher *dispatcher = init_manager(5, 8);
    VA block1;
    int err = dispatcher_malloc(dispatcher, &block1, 8);
    assert(check_equal(0, err));
    assert(check_equal(0, (size_t) block1));

    assert(check_equal(0, dispatcher->segments->first_node->data->segment_begin));
    assert(check_equal(8, dispatcher->segments->first_node->data->segment_end));

    VA block2;
    err = dispatcher_malloc(dispatcher, &block2, 30);
    assert(check_equal(0, err));
    assert(check_equal(8, (size_t) block2));

    segment *second = dispatcher->segments->first_node->next_p->data;
    assert(check_equal(8, second->segment_begin));
    assert(check_equal(40, second->segment_end));

    err = dispatcher_free(dispatcher, block1);
    assert(check_equal(0, err));

    err = dispatcher_malloc(dispatcher, &block1, 8);
    assert(check_equal(0, err));
    assert(check_equal(8, (size_t) block1));

    assert(check_equal(0, dispatcher->segments->first_node->data->segment_begin));
    assert(check_equal(8, dispatcher->segments->first_node->data->segment_end));
    free_dispatcher(dispatcher);
}

int main() {
//    unit_test_init_dispatcher();
//    unit_test_malloc_block();
//    unit_test_malloc_block_two_blocks();
    unit_test_write_buffer_to_one_page();
    unit_test_write_buffer_to_page_not_in_begin();
//    unit_test_read_buffer_from_two_page_not_in_begin();
//    unit_test_write_buffer_to_two_page_not_in_begin();
//    unit_test_read_buffer_from_one_page();
//    unit_test_malloc_many_pages();
//    unit_test_create_segments_and_remove_some();
}