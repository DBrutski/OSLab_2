#define BOOST_TEST_MODULE unit_test

#include <memory_pager.h>
#include <memory_dispatcher.h>
#include <boost/test/included/unit_test.hpp>

memory_dispatcher init_manager(int n, size_t pageSize) {
    return *create_memory_dispatcher(n, pageSize);
}

BOOST_AUTO_TEST_CASE(unit_test_unit) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_block) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block;
    int err = dispatcher_malloc(&dispatcher, &block, 8);
    BOOST_CHECK_EQUAL(0, err);

    segment expected_segment = dispatcher.segments->first_node->data;
    BOOST_CHECK_EQUAL(1, map_size(dispatcher.segments));
    BOOST_CHECK_EQUAL(0, expected_segment.segment_begin);
    BOOST_CHECK_EQUAL(8, expected_segment.segment_size);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_block_two_blocks) {
    memory_dispatcher dispatcher = init_manager(5, 4);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block1;
    VA block2;

    int err = dispatcher_malloc(&dispatcher, &block1, 6);
    BOOST_CHECK_EQUAL(0, err);

    err = dispatcher_malloc(&dispatcher, &block2, 8);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL(0, (size_t) block1);
    BOOST_CHECK_EQUAL(8, (size_t) block2);
}

BOOST_AUTO_TEST_CASE(unit_test_write_buffer_to_one_page) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block1;

    int err = dispatcher_malloc(&dispatcher, &block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(&dispatcher, block1, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    VA allocated_buffer = pager->allocated_memory;
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_buffer, allocated_buffer + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_write_buffer_to_page_not_in_begin) {
    memory_dispatcher dispatcher = init_manager(5, 16);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block1;

    int err = dispatcher_malloc(&dispatcher, &block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(&dispatcher, block1 + 5, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    VA allocated_buffer = pager->allocated_memory;
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_buffer + 5, allocated_buffer + 5 + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_write_buffer_to_two_page_not_in_begin) {
    memory_dispatcher dispatcher = init_manager(5, 4);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block1;

    int err = dispatcher_malloc(&dispatcher, &block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(&dispatcher, block1 + 2, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    VA allocated_buffer = pager->allocated_memory;
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_buffer + 2, allocated_buffer + 2 + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_read_buffer_from_one_page) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block1;

    int err = dispatcher_malloc(&dispatcher, &block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(&dispatcher, block1, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    char *readen_buffer = new char[buffer_size];
    err = dispatcher_read(&dispatcher, block1, readen_buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size, readen_buffer, readen_buffer + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_read_buffer_from_two_page_not_in_begin) {
    memory_dispatcher dispatcher = init_manager(5, 4);
    memory_pager *pager = dispatcher.pager;
    BOOST_CHECK_EQUAL(5, pager->allocated_pages_amount);

    VA block1;

    int err = dispatcher_malloc(&dispatcher, &block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher_write(&dispatcher, block1 + 2, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    char *readen_buffer = new char[buffer_size];
    err = dispatcher_read(&dispatcher, block1 + 2, readen_buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size, readen_buffer, readen_buffer + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_many_pages) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;
    VA block;
    int err = dispatcher_malloc(&dispatcher, &block, 30);
    BOOST_CHECK_EQUAL(0, err);


    BOOST_CHECK_EQUAL(30, dispatcher.segments->first_node->data.segment_size);
}

BOOST_AUTO_TEST_CASE(unit_test_create_segments_and_remove_some) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    memory_pager *pager = dispatcher.pager;
    VA block1;
    int err = dispatcher_malloc(&dispatcher, &block1, 8);
    BOOST_CHECK_EQUAL(0, err);
    BOOST_CHECK_EQUAL(0, (size_t) block1);

    BOOST_CHECK_EQUAL(0, dispatcher.segments->first_node->data.segment_begin);
    BOOST_CHECK_EQUAL(8, dispatcher.segments->first_node->data.segment_end);

    VA block2;
    err = dispatcher_malloc(&dispatcher, &block2, 30);
    BOOST_CHECK_EQUAL(0, err);
    BOOST_CHECK_EQUAL(8, (size_t) block2);

    segment second = dispatcher.segments->first_node->next_p->data;
    BOOST_CHECK_EQUAL(8, second.segment_begin);
    BOOST_CHECK_EQUAL(40, second.segment_end);

    err = dispatcher_free(&dispatcher, block1);
    BOOST_CHECK_EQUAL(0, err);

    err = dispatcher_malloc(&dispatcher, &block1, 8);
    BOOST_CHECK_EQUAL(0, err);
    BOOST_CHECK_EQUAL(8, (size_t) block1);

    BOOST_CHECK_EQUAL(0, dispatcher.segments->first_node->data.segment_begin);
    BOOST_CHECK_EQUAL(8, dispatcher.segments->first_node->data.segment_end);

}
