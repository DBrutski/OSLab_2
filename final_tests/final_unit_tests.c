#define BOOST_TEST_MODULE unit_test

#include <assert.h>
#include <mmemory.h>
#include <memory_pager.h>
#include <stdio.h>

char *generateBuffer(int size) {
    char *buffer = (char *) malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++) {
        buffer[i] = rand() % 32;
    }
    return buffer;
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

void unit_test_write_buffer_to_one_page() {
    ___init(5, 8);

    VA block1;

    int err = _malloc(&block1, 8);
    assert(check_equal(0, err));

    char *buffer = "buffer";
    size_t buffer_size = 7;
    err = _write(block1, buffer, buffer_size);
    assert(check_equal(0, err));

    VA allocated_buffer = (char *) malloc(sizeof(char) * buffer_size);
    err = _read(block1, allocated_buffer, buffer_size);
    assert(check_equal(0, err));

    assert(check_equal_collection(buffer, buffer + buffer_size,
                                  allocated_buffer, allocated_buffer + buffer_size));
}

void unit_test_write_buffer_to_segment_not_in_begin() {
    ___init(5, 8);

    VA block1;

    int err = _malloc(&block1, 35);
    assert(check_equal(0, err));

    char *buffer = generateBuffer(20);

    err = _write(block1 + 2, buffer, 20);
    assert(check_equal(0, err));

    char *readen_buffer = (char *) malloc(sizeof(char) * 20);
    err = _read(block1 + 5, readen_buffer, 15);
    assert(check_equal(0, err));

    assert(check_equal_collection(buffer + 3, buffer + 3 + 15,
                           readen_buffer, readen_buffer + 15));
}


void unit_test_internal_write() {
    ___init(10, 8);

    VA src_block;

    const int buffer_size = 35;

    int err = _malloc(&src_block, buffer_size);
    assert(check_equal(0, err));

    char *buffer = generateBuffer(buffer_size);

    err = _write(src_block, buffer, buffer_size);
    assert(check_equal(0, err));

    VA dest_block;
    err = _malloc(&dest_block, buffer_size);
    assert(check_equal(0, err));

    err = _write(dest_block, &src_block, buffer_size);
    assert(check_equal(0, err));


    char *readen_buffer = (char *) malloc(sizeof(char) * buffer_size);
    err = _read(dest_block, readen_buffer, buffer_size);
    assert(check_equal(0, err));

    check_equal_collection(buffer, buffer + buffer_size,
                           readen_buffer, readen_buffer + buffer_size);

}


void unit_test_free_function() {
    ___init(5, 8);

    VA block1;

    VA block2;

    int err = _malloc(&block1, 8);
    assert(check_equal(0, err));

    err = _malloc(&block2, 16);
    assert(check_equal(0, err));

    err = _free(block1);
    assert(check_equal(0, err));

    err = _malloc(&block1, 24);
    assert(check_equal(0, err));

    char *buffer = generateBuffer(24);
    size_t buffer_size = 24;
    err = _write(block1, buffer, buffer_size);
    assert(check_equal(0, err));

    VA allocated_buffer = (char *) malloc(sizeof(char) * buffer_size);
    err = _read(block1, allocated_buffer, buffer_size);
    assert(check_equal(0, err));

    assert(check_equal_collection(buffer, buffer + buffer_size,
                                  allocated_buffer, allocated_buffer + buffer_size));

}

int main() {
    unit_test_write_buffer_to_one_page();
    unit_test_write_buffer_to_segment_not_in_begin();
    unit_test_internal_write();
    unit_test_free_function();
    printf("all correct");
}

