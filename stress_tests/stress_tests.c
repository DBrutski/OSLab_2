#define BOOST_TEST_MODULE unit_test

#include <assert.h>
#include <memory_pager.h>
#include <page.h>
#include "memory_dispatcher.h"

memory_dispatcher *init_manager(int n, size_type pageSize) {
    return create_memory_dispatcher(n, pageSize);
}

typedef struct {
    VA block;
    char *buffer;
    size_type offset;
} test_block;

test_block *
init_test_blocks(size_type amount, size_type min_size, size_type max_size, size_type min_offset, size_type max_offset) {
    test_block *test_blocks = malloc(sizeof(test_block) * amount);
    for (int i = 0; i < amount; i++) {
        size_type interval = max_size - min_size + 1;
        size_type block_size = min_size + (rand() % interval);
        test_blocks[i].buffer = malloc(sizeof(char) * block_size);
        int buffer_num;
        for (buffer_num = 0; buffer_num < block_size; buffer_num++) {
            test_blocks[i].buffer[buffer_num] = rand() % 32 + 1;
        }

        size_type offset_interval = max_offset - min_offset + 1;
        size_type block_offset = min_size + (rand() % offset_interval);
        test_blocks[i].offset = block_offset;

    }
    return test_blocks;
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

void allocate_page_size_segments() {
    size_type block_size = 8;

    memory_dispatcher *dispatcher = init_manager(10, 8);
    memory_pager *pager = dispatcher->pager;
    test_block *test_blocks = init_test_blocks(20, block_size, block_size, 0, 0);

    int err;
    for (int i = 0; i < 10; i++) {
        err = dispatcher_malloc(dispatcher, &(test_blocks[i].block), 8);
        assert(check_equal(0, err));
        assert(pager->pages_virtual_space[i]->offset == i * 8);

    }

    for (int i = 0; i < 10; i++) {

        err = dispatcher_write(dispatcher, test_blocks[i].block, test_blocks[i].buffer, block_size);
        assert(check_equal(0, err));
    }

    for (int i = 0; i < 10; i++) {
        char *readen_buffer = malloc(sizeof(char) * block_size);
        err = dispatcher_read(dispatcher, test_blocks[i].block, readen_buffer, block_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(test_blocks[i].buffer, test_blocks[i].buffer + block_size,
                                      readen_buffer, readen_buffer + block_size));
    }

    assert(check_equal(10, pager->allocated_pages_amount));

    for (int i = 0; i < 10; i += 2) {
        err = dispatcher_free(dispatcher, test_blocks[i].block);
        assert(check_equal(0, err));
    }

    assert(check_equal(5, pager->allocated_pages_amount));

    for (int i = 10; i < 15; i++) {
        VA block = test_blocks[i].block;
        err = dispatcher_malloc(dispatcher, &block, 8);
        assert(check_equal(0, err));

        err = dispatcher_write(dispatcher, test_blocks[i].block, test_blocks[i].buffer, block_size);
        assert(check_equal(0, err));
    }

    assert(check_equal(10, pager->allocated_pages_amount));

    for (int i = 10; i < 15; i++) {
        char *readen_buffer = malloc(sizeof(char) * block_size);
        err = dispatcher_read(dispatcher, test_blocks[i].block, readen_buffer, block_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(test_blocks[i].buffer, test_blocks[i].buffer + block_size,
                                      readen_buffer, readen_buffer + block_size));
    }

    for (int i = 1; i < 10; i += 2) {
        err = dispatcher_free(dispatcher, test_blocks[i].block);
        assert(check_equal(0, err));
    }

    assert(check_equal(5, pager->allocated_pages_amount));
}

void write_more_then_page_segments() {
    assert(false);
}

void write_int_buffers() {
    assert(false);
}

void use_more_memory_than_allocated() {
    assert(false);
}

int main() {
    allocate_page_size_segments();
    write_more_then_page_segments();
    write_int_buffers();
    use_more_memory_than_allocated();
    return 0;
}