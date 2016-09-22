#define BOOST_TEST_MODULE unit_test

#include <boost/test/included/unit_test.hpp>
#include "memory_dispatcher.h"

memory_dispatcher init_manager(int n, size_t pageSize) {
    return memory_dispatcher(n, pageSize);
}

struct test_block {
    VA block;
    char *buffer;
    size_t offset;
};

test_block *init_test_blocks(size_t amount, size_t min_size, size_t max_size, size_t min_offset, size_t max_offset) {
    test_block *test_blocks = new test_block[amount];
    for (int i = 0; i < amount; i++) {
        size_t interval = max_size - min_size;
        size_t block_size = min_size + (rand() % interval);
        test_blocks[i].buffer = new char[block_size];
        size_t offset_interval = max_size - min_size;
        size_t block_offset = min_size + (rand() % interval);
        test_blocks[i].offset = block_offset;
    }
}

BOOST_AUTO_TEST_CASE(allocate_page_size_segments) {
    memory_dispatcher dispatcher = init_manager(10, 8);
    test_block *test_blocks = init_test_blocks(20, 8, 8, 0, 0);

    BOOST_CHECK_EQUAL(5, dispatcher.allocated_pages_amount);
}

BOOST_AUTO_TEST_CASE(write_more_then_page_segments) {
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(write_int_buffers) {
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(use_more_memory_than_allocated) {
    BOOST_CHECK(false);
}