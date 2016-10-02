#include <memory_pager.h>
#include <assert.h>
#include <map.h>
#include <segment.h>
#include <page.h>

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

memory_pager *init_pager_with_paging(size_type inmemmory_pages, size_type outmemmory_pages, size_type page_size) {
    return create_memory_pager(page_size, inmemmory_pages, outmemmory_pages);
}

void test_init_pager() {
    memory_pager *pager = init_pager_with_paging(1, 1, 8);
    assert(check_equal(1, queue_size(pager->free_in_memory_pages)));
    assert(check_equal(1, queue_size(pager->out_pager->free_in_memory_pages)));
}

void test_malloc_pager_with_paging() {
    memory_pager *pager = init_pager_with_paging(1, 1, 8);

    segment *allocated_segment = pager_malloc(pager, 0, 16);
    assert(check_equal(0, allocated_segment->pages[0]->offset));
    assert(check_equal(true, allocated_segment->pages[0]->is_in_memmory));
    assert(check_equal(0, allocated_segment->pages[1]->offset));
    assert(check_equal(false, allocated_segment->pages[1]->is_in_memmory));
}

void test_write_to_pumped_page() {
    memory_pager *pager = init_pager_with_paging(1, 1, 8);

    char *first_buffer = "buffer1";
    char *second_buffer = "buffer2";
    size_type buffer_size = 8;

    segment *allocated_segment = pager_malloc(pager, 0, 16);
    page *first_page = allocated_segment->pages[0];
    page *second_page = allocated_segment->pages[1];

    write_page(pager, first_page, 0, first_buffer, buffer_size);
    assert(check_equal_collection(first_buffer, first_buffer + buffer_size, pager->allocated_memory,
                                  pager->allocated_memory + buffer_size));

    write_page(pager, second_page, 0, second_buffer, buffer_size);
    assert(check_equal_collection(second_buffer, second_buffer + buffer_size, pager->allocated_memory,
                                  pager->allocated_memory + buffer_size));
    assert(check_equal_collection(first_buffer, first_buffer + buffer_size, pager->out_pager->allocated_memory,
                                  pager->out_pager->allocated_memory + buffer_size));

    assert(check_equal(0, second_page->offset));
    assert(check_equal(true, second_page->is_in_memmory));
    assert(check_equal(0, first_page->offset));
    assert(check_equal(false, first_page->is_in_memmory));
}

int main() {
    test_init_pager();
    test_malloc_pager_with_paging();
    test_write_to_pumped_page();
}