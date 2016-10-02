#include <assert.h>
#include <memory_pager.h>
#include "memory_dispatcher.h"

memory_dispatcher *init_manager(size_type n, size_type pageSize) {
    return create_memory_dispatcher(n, pageSize);
}

typedef struct {
    VA block;
    size_type block_size;
    size_type buffer_size;
    char *buffer;
    size_type offset;
} test_block;

void write_buffer_from_dispatcher_memmory();

memory_dispatcher *init_manager_with_paging(size_type pages_amount, size_type page_size) {
    memory_dispatcher *dispatcher = create_memory_dispatcher_with_paging(pages_amount, page_size, pages_amount);
    return dispatcher;
}

test_block *
init_test_blocks(size_type amount, size_type min_block_size, size_type max_block_size, size_type min_offset,
                 size_type max_offset, size_type buffer_size_min, size_type buffer_size_max) {
    test_block *test_blocks = malloc(sizeof(test_block) * amount);
    for (int i = 0; i < amount; i++) {

        size_type interval = max_block_size - min_block_size + 1;
        size_type block_size = min_block_size + (rand() % interval);
        test_blocks[i].buffer = malloc(sizeof(char) * block_size);
        int buffer_num;
        for (buffer_num = 0; buffer_num < block_size; buffer_num++) {
            test_blocks[i].buffer[buffer_num] = rand() % 32 + 1;
        }

        size_type buffer_size_interval = buffer_size_min - buffer_size_max + 1;
        size_type buffer_size = buffer_size_min + (rand() % buffer_size_interval);
        size_type offset_interval = max_offset - min_offset + 1;
        size_type block_offset = min_offset + (rand() % offset_interval);
        test_blocks[i].offset = block_offset;
        test_blocks[i].buffer_size = buffer_size;
        test_blocks[i].block_size = block_size;
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

void test_free_function_on_page_size_blocks() {
    size_type block_size = 8;

    memory_dispatcher *dispatcher = init_manager(10, 8);
    test_block *test_blocks = init_test_blocks(20, block_size, block_size, 0, 0, 0, 0);

    int err;
    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_malloc(dispatcher, &(current_block->block), 8);
        assert(check_equal(0, err));

    }

    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_write(dispatcher, current_block->block, current_block->buffer, block_size);
        assert(check_equal(0, err));
    }

    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        char *readen_buffer = malloc(sizeof(char) * block_size);
        err = dispatcher_read(dispatcher, current_block->block, readen_buffer, block_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(current_block->buffer, current_block->buffer + block_size,
                                      readen_buffer, readen_buffer + block_size));
        free(readen_buffer);
    }

    for (int i = 0; i < 10; i += 2) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_free(dispatcher, current_block->block);
        assert(check_equal(0, err));
    }


    for (int i = 10; i < 15; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_malloc(dispatcher, &(current_block->block), 8);
        assert(check_equal(0, err));

        err = dispatcher_write(dispatcher, current_block->block, current_block->buffer, block_size);
        assert(check_equal(0, err));
    }

    for (int i = 10; i < 15; i++) {
        test_block *current_block = &test_blocks[i];
        char *readen_buffer = malloc(sizeof(char) * block_size);
        err = dispatcher_read(dispatcher, current_block->block, readen_buffer, block_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(current_block->buffer, current_block->buffer + block_size,
                                      readen_buffer, readen_buffer + block_size));
    }

    for (int i = 1; i < 10; i += 2) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_free(dispatcher, current_block->block);
        assert(check_equal(0, err));
    }
    free_dispatcher(dispatcher);
}

void test_work_with_buffer_more_then_page_size_and_write_to_random_place() {
    size_type block_size = 16;
    size_type buffer_size = 9;

    memory_dispatcher *dispatcher = init_manager(20, 8);
    test_block *test_blocks = init_test_blocks(20, block_size, block_size, 1, 3, buffer_size, buffer_size);

    int err;
    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_malloc(dispatcher, &(current_block->block), current_block->block_size);
        assert(check_equal(0, err));

        err = dispatcher_write(dispatcher, current_block->block + current_block->offset, current_block->buffer,
                               current_block->buffer_size);
        assert(check_equal(0, err));
    }

    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        char *readen_buffer = malloc(sizeof(char) * current_block->buffer_size);
        err = dispatcher_read(dispatcher, current_block->block + current_block->offset,
                              readen_buffer, current_block->buffer_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(current_block->buffer, current_block->buffer + current_block->buffer_size,
                                      readen_buffer, readen_buffer + current_block->buffer_size));
        free(readen_buffer);
    }

    for (int i = 0; i < 10; i += 2) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_free(dispatcher, current_block->block);
        assert(check_equal(0, err));
    }


    for (int i = 10; i < 15; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_malloc(dispatcher, &(current_block->block), current_block->block_size);
        assert(check_equal(0, err));

        err = dispatcher_write(dispatcher, current_block->block + current_block->offset, current_block->buffer,
                               current_block->buffer_size);
        assert(check_equal(0, err));
    }

    for (int i = 10; i < 15; i++) {
        test_block *current_block = &test_blocks[i];
        char *readen_buffer = malloc(sizeof(char) * current_block->buffer_size);
        err = dispatcher_read(dispatcher, current_block->block + current_block->offset, readen_buffer,
                              current_block->buffer_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(current_block->buffer, current_block->buffer + current_block->buffer_size,
                                      readen_buffer, readen_buffer + current_block->buffer_size));
    }

    for (int i = 1; i < 10; i += 2) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_free(dispatcher, current_block->block);
        assert(check_equal(0, err));
    }
    free_dispatcher(dispatcher);
}

void write_buffer_from_dispatcher_memmory() {
    size_type block_size = 16;
    size_type buffer_size = 9;

    memory_dispatcher *dispatcher = init_manager(20, 8);
    test_block *test_blocks = init_test_blocks(10, block_size, block_size, 1, 3, buffer_size, buffer_size);

    int err;
    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_malloc(dispatcher, &(current_block->block), current_block->block_size);
        assert(check_equal(0, err));

        err = dispatcher_write(dispatcher, current_block->block + current_block->offset, current_block->buffer,
                               current_block->buffer_size);
        assert(check_equal(0, err));
    }

    for (int i = 0; i < 10; i++) {
        test_block *current_block = &test_blocks[i];
        char *readen_buffer = malloc(sizeof(char) * current_block->buffer_size);
        err = dispatcher_read(dispatcher, current_block->block + current_block->offset,
                              readen_buffer, current_block->buffer_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(current_block->buffer, current_block->buffer + current_block->buffer_size,
                                      readen_buffer, readen_buffer + current_block->buffer_size));
        free(readen_buffer);
    }

    for (int i = 0, j = 5; i < 5; i++, j++) {
        test_block *dest_block = &test_blocks[i];
        test_block *src_block = &test_blocks[j];
        err = dispatcher_write(dispatcher, dest_block->block + dest_block->offset, src_block->block + src_block->offset,
                               src_block->buffer_size);
        assert(check_equal(0, err));

        char *readen_buffer = (char *) malloc(sizeof(char) * src_block->buffer_size);
        err = dispatcher_read(dispatcher, dest_block->block + dest_block->offset, readen_buffer,
                              src_block->buffer_size);
        assert(check_equal(0, err));
        assert(check_equal_collection(src_block->buffer, src_block->buffer + src_block->buffer_size,
                                      readen_buffer, readen_buffer + src_block->buffer_size));

    }
}

void use_more_memory_than_allocated() {
    const size_type block_size = 16;
    const size_type buffer_size = 9;
    const size_type segments_amount = 20;

    memory_dispatcher *dispatcher = init_manager_with_paging(20, 8);
    test_block *test_blocks = init_test_blocks(segments_amount, block_size, block_size, 1, 3, buffer_size, buffer_size);

    int err;
    for (int i = 0; i < segments_amount; i++) {
        test_block *current_block = &test_blocks[i];
        err = dispatcher_malloc(dispatcher, &(current_block->block), current_block->block_size);
        assert(check_equal(0, err));

        err = dispatcher_write(dispatcher, current_block->block + current_block->offset, current_block->buffer,
                               current_block->buffer_size);
        assert(check_equal(0, err));
    }

    for (int i = 0; i < segments_amount; i++) {
        test_block *current_block = &test_blocks[i];
        char *readen_buffer = malloc(sizeof(char) * current_block->buffer_size);
        err = dispatcher_read(dispatcher, current_block->block + current_block->offset,
                              readen_buffer, current_block->buffer_size);
        assert(check_equal(0, err));

        assert(check_equal_collection(current_block->buffer, current_block->buffer + current_block->buffer_size,
                                      readen_buffer, readen_buffer + current_block->buffer_size));
        free(readen_buffer);
    }
    free_dispatcher(dispatcher);
}


int main() {
    test_free_function_on_page_size_blocks();
    test_work_with_buffer_more_then_page_size_and_write_to_random_place();
    write_buffer_from_dispatcher_memmory();
    use_more_memory_than_allocated();
    return 0;
}
