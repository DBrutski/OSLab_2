#define BOOST_TEST_MODULE unit_test

#include <boost/test/included/unit_test.hpp>
#include "mmemory.cpp"

void init_manager(int n, size_t pageSize) {
    _init(n, pageSize);
}

BOOST_AUTO_TEST_CASE(unit_test_unit) {
    _init(5, 8);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);
}

BOOST_AUTO_TEST_CASE(unit_test_malloc_block) {
    _init(5, 8);
    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);

    VA *block;
    _malloc(block, 8);
    size_t pages_amount = dispatcher.reserved_pages_amount;
    BOOST_CHECK_EQUAL(1, pages_amount);

    segment *expected_segment = dispatcher.first_segment;
    BOOST_CHECK(expected_segment != NULL);
    BOOST_CHECK_EQUAL(0, expected_segment->offsetBlock);
    BOOST_CHECK_EQUAL(8, expected_segment->segment_size);
    virtual_page *pages = expected_segment->pages;
    BOOST_CHECK(pages != NULL);
    BOOST_CHECK(pages->buffer_pointer >= dispatcher.allocated_buffer);
    BOOST_CHECK(pages->buffer_pointer <
                dispatcher.allocated_buffer + dispatcher.allocated_pages_amount * dispatcher.page_size);
}