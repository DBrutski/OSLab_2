#define BOOST_TEST_MODULE unit_test

#include <assert.h>
#include <stdbool.h>
#include <mmemory.h>
#include <queue.h>

bool check_equal(size_type l, size_type r) {
    return l == r;
}

bool check_equal_page(page *page_l, page *page_r) {
    bool result = true;
    result &= page_l->offset == page_r->offset;
    result &= page_l->is_in_memmory == page_r->is_in_memmory;
    return result;
}

void unit_test_queue_pages() {
    page *page1 = create_page(0, true);
    page *page2 = create_page(8, true);
    queue *tested_queue = create_queue(2);
    queue_push(tested_queue, page1);
    queue_push(tested_queue, page2);
    assert(check_equal_page(page1, queue_pop(tested_queue)));
    assert(check_equal_page(page2, queue_pop(tested_queue)));
    free_queue(tested_queue);
}

void unit_test_queue_with_bigger_pages() {
    page *page1 = create_page(0, true);
    page *page2 = create_page(16, true);
    queue *tested_queue = create_queue(2);
    queue_push(tested_queue, page1);
    queue_push(tested_queue, page2);
    assert(check_equal_page(page1, queue_pop(tested_queue)));
    assert(check_equal_page(page2, queue_pop(tested_queue)));
    free_queue(tested_queue);
}

void unit_test_queue_remove() {
    page *page1 = create_page(0, true);
    page *page2 = create_page(16, true);
    page *page3 = create_page(32, true);
    queue *tested_queue = create_queue(3);
    queue_push(tested_queue, page1);
    queue_push(tested_queue, page2);
    queue_push(tested_queue, page3);
    queue_remove(tested_queue, page2);
    assert(check_equal_page(page1, queue_pop(tested_queue)));
    assert(check_equal_page(page3, queue_pop(tested_queue)));
    free_queue(tested_queue);
}

void unit_test_queue_remove_2() {
    page *page1 = create_page(0, true);
    page *page2 = create_page(16, true);
    page *page3 = create_page(32, true);
    queue *tested_queue = create_queue(3);
    queue_push(tested_queue, page1);
    queue_push(tested_queue, page2);
    queue_push(tested_queue, page3);
    queue_remove(tested_queue, page2);
    queue_push(tested_queue, page2);
    assert(check_equal_page(page1, queue_pop(tested_queue)));
    assert(check_equal_page(page3, queue_pop(tested_queue)));
    assert(check_equal_page(page2, queue_pop(tested_queue)));
    free_queue(tested_queue);
}


void unit_test_queue_pop() {
    page *page1 = create_page(0, true);
    page *page2 = create_page(16, true);
    page *page3 = create_page(32, true);
    queue *tested_queue = create_queue(3);
    queue_push(tested_queue, page1);
    queue_push(tested_queue, page2);
    queue_push(tested_queue, page3);
    assert(check_equal_page(page1, queue_pop(tested_queue)));
    assert(check_equal_page(page2, queue_pop(tested_queue)));
    assert(check_equal_page(page3, queue_pop(tested_queue)));
    free_queue(tested_queue);
}

int main() {
    unit_test_queue_pages();
    unit_test_queue_with_bigger_pages();
    unit_test_queue_remove();
    unit_test_queue_pop();
}