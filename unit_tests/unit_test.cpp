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
    dispatcher.malloc(block, 8);
    size_t pages_amount = dispatcher.reserved_pages_amount;
    BOOST_CHECK_EQUAL(1, pages_amount);

    segment *expected_segment = dispatcher.first_segment;
    BOOST_CHECK(expected_segment != NULL);
    BOOST_CHECK_EQUAL(0, expected_segment->segment_offset);
    BOOST_CHECK_EQUAL(8, expected_segment->segment_size);
    page *pages = expected_segment->pages;
    BOOST_CHECK(pages != NULL);
    BOOST_CHECK(pages->buffer_pointer >= dispatcher.allocated_buffer);
    BOOST_CHECK(pages->buffer_pointer <
                dispatcher.allocated_buffer + dispatcher.allocated_pages_amount * dispatcher.page_size);
}