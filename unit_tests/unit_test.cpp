#define BOOST_TEST_MODULE unit_test

#include <boost/test/included/unit_test.hpp>
#include "memory_dispatcher.h"

memory_dispatcher init_manager(int n, size_t pageSize) {
    return memory_dispatcher(n, pageSize);
}

BOOST_AUTO_TEST_CASE(unit_test_unit) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_block) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA *block;
    int err = dispatcher.malloc(block, 8);
    BOOST_CHECK_EQUAL(0, err);

    size_t pages_amount = dispatcher.reserved_pages_amount;
    BOOST_CHECK_EQUAL(1, pages_amount);

    segment expected_segment = (*dispatcher.segments.begin()).second;
    BOOST_CHECK_EQUAL(1, dispatcher.segments.size());
    BOOST_CHECK_EQUAL(0, expected_segment.segment_offset);
    BOOST_CHECK_EQUAL(8, expected_segment.segment_size);
    page *pages = expected_segment.pages;
    BOOST_CHECK(pages != NULL);
    BOOST_CHECK(pages->buffer_pointer >= dispatcher.allocated_buffer);
    BOOST_CHECK(pages->buffer_pointer <
                dispatcher.allocated_buffer + dispatcher.allocated_pages_amount * dispatcher.page_size);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_block_two_blocks) {
    memory_dispatcher dispatcher = init_manager(5, 4);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA block1;
    VA block2;

    int err = dispatcher.malloc(&block1, 6);
    BOOST_CHECK_EQUAL(0, err);

    err = dispatcher.malloc(&block2, 8);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL(4, dispatcher.reserved_pages_amount);

    BOOST_CHECK_EQUAL(0, (size_t) block1);
    BOOST_CHECK_EQUAL(8, (size_t) block2);
}

BOOST_AUTO_TEST_CASE(unit_test_write_buffer_to_one_page) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA block1;

    int err = dispatcher.malloc(&block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher.write(block1, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    VA allocated_buffer = dispatcher.allocated_buffer;
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_buffer, allocated_buffer + buffer_size);

    page allocated_page = (*dispatcher.segments.begin()).second.pages[0];
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_page.buffer_pointer, allocated_page.buffer_pointer + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_write_buffer_to_page_not_in_begin) {
    memory_dispatcher dispatcher = init_manager(5, 16);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA block1;

    int err = dispatcher.malloc(&block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher.write(block1 + 5, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    VA allocated_buffer = dispatcher.allocated_buffer;
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_buffer + 5, allocated_buffer + 5 + buffer_size);

    page allocated_page = (*dispatcher.segments.begin()).second.pages[0];
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_page.buffer_pointer + 5, allocated_page.buffer_pointer + 5 + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_write_buffer_to_two_page_not_in_begin) {
    memory_dispatcher dispatcher = init_manager(5, 4);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA block1;

    int err = dispatcher.malloc(&block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher.write(block1 + 2, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    VA allocated_buffer = dispatcher.allocated_buffer;
    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size,
                                  allocated_buffer + 2, allocated_buffer + 2 + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_read_buffer_from_one_page) {
    memory_dispatcher dispatcher = init_manager(5, 8);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA block1;

    int err = dispatcher.malloc(&block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher.write(block1, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    char *readen_buffer = new char[buffer_size];
    err = dispatcher.read(block1, readen_buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size, readen_buffer, readen_buffer + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_read_buffer_from_two_page_not_in_begin) {
    memory_dispatcher dispatcher = init_manager(5, 4);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA block1;

    int err = dispatcher.malloc(&block1, 16);
    BOOST_CHECK_EQUAL(0, err);

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = dispatcher.write(block1 + 2, buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    char *readen_buffer = new char[buffer_size];
    err = dispatcher.read(block1 + 2, readen_buffer, buffer_size);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + buffer_size, readen_buffer, readen_buffer + buffer_size);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_many_pages) {
    memory_dispatcher dispatcher = init_manager(5, 8);

    VA block;
    int err = dispatcher.malloc(&block, 30);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL(30, (*dispatcher.segments.begin()).second.segment_size);
}

BOOST_AUTO_TEST_CASE(unit_test_create_segments_and_remove_some) {
    memory_dispatcher dispatcher = init_manager(5, 8);

    VA block;
    int err = dispatcher.malloc(&block, 30);
    BOOST_CHECK_EQUAL(0, err);

    BOOST_CHECK_EQUAL(30, (*dispatcher.segments.begin()).second.segment_size);
}
