#define BOOST_TEST_MODULE mmemory_test

#include <boost/test/included/unit_test.hpp>
#include "mmemory.h"

void init_manager(int n, size_t pageSize) {
    _init(n, pageSize);
}

BOOST_AUTO_TEST_CASE(init_manager_test) {
    int err = _init(5, 32);
    BOOST_CHECK_EQUAL(0, err);
}

BOOST_AUTO_TEST_CASE(test_mallok) {
    init_manager(5, 32);

    VA block;
    int err = _malloc(&block, 32);
    BOOST_CHECK_EQUAL(0, err);

}

BOOST_AUTO_TEST_CASE(test_mallok_more_than_page) {
    init_manager(5, 32);

    VA block;
    int err = _malloc(&block, 70);
    BOOST_CHECK_EQUAL(0, err);
}

BOOST_AUTO_TEST_CASE(test_mallok_get_error_when_block_to_large) {
    init_manager(5, 8);

    VA block;
    int err = _malloc(&block, 70);
    BOOST_CHECK_EQUAL(-2, err);

}

BOOST_AUTO_TEST_CASE(write_char_block) {
    init_manager(5, 32);

    VA block;
    int err = _malloc(&block, 18);
    BOOST_CHECK_EQUAL(0, err);

    char *writen_block = "segment size = 15";
    err = _write(block, writen_block, 15);
    BOOST_CHECK_EQUAL(0, err);


    char *readen_block;
    err = _read(block, readen_block, 15);
    BOOST_CHECK_EQUAL(0, err);


    BOOST_CHECK_EQUAL_COLLECTIONS(writen_block, writen_block + 15, readen_block, readen_block + 15);
}

BOOST_AUTO_TEST_CASE(write_char_block_in_two_pages) {
    init_manager(5, 8);

    VA block;
    int err = _malloc(&block, 18);
    BOOST_CHECK_EQUAL(0, err);

    char *writen_block = "segment size = 15";
    err = _write(block, writen_block, 15);
    BOOST_CHECK_EQUAL(0, err);


    char *readen_block;
    err = _read(block, readen_block, 15);
    BOOST_CHECK_EQUAL(0, err);


    BOOST_CHECK_EQUAL_COLLECTIONS(writen_block, writen_block + 15, readen_block, readen_block + 15);
}

BOOST_AUTO_TEST_CASE(write_char_block_in_to_small_block) {
    init_manager(5, 8);

    VA block;
    int err = _malloc(&block, 7);
    BOOST_CHECK_EQUAL(0, err);


    char *writen_block = "segment size = 15";
    err = _write(block, writen_block, 15);
    BOOST_CHECK_EQUAL(-2, err);
}

BOOST_AUTO_TEST_CASE(write_char_block_not_in_block_beginning) {
    init_manager(5, 8);

    VA block;
    int err = _malloc(&block, 20);
    BOOST_CHECK_EQUAL(0, err);


    block += 5;
    char *writen_block = "segment size = 15";
    err = _write(block, writen_block, 15);
    BOOST_CHECK_EQUAL(0, err);

    char *readen_block;
    err = _read(block, readen_block, 15);
    BOOST_CHECK_EQUAL(0, err);


    BOOST_CHECK_EQUAL_COLLECTIONS(writen_block, writen_block + 15, readen_block, readen_block + 15);
}

BOOST_AUTO_TEST_CASE(write_int_block) {
    init_manager(5, 8);

    VA block;
    int err = _malloc(&block, 20);
    BOOST_CHECK_EQUAL(0, err);


    block += 5;
    int *writen_block = new int[5]{1, 2, 3, 4, 5};
    err = _write(block, writen_block, 15);
    BOOST_CHECK_EQUAL(0, err);

    int *readen_block;
    err = _read(block, readen_block, 20);
    BOOST_CHECK_EQUAL(0, err);


    BOOST_CHECK_EQUAL_COLLECTIONS(writen_block, writen_block + 5, readen_block, readen_block + 5);
}
