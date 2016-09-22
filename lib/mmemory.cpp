#include "mmemory.h"
#include <math.h>
#include <string.h>


#define SWAPING_NAME       "swap.dat"

const int SUCCESSFUL_CODE = 0;
const int INCORRECT_PARAMETERS_ERROR = -1;
const int NOT_ENOUGH_MEMORY_ERROR = -2;
const int OUT_OF_RANGE_ERROR = -2;
const int UNKNOWN_ERROR = 1;

size_t get_required_pages_amount(size_t block_size);

struct page_info {
    unsigned long offsetPage;
    char isUse;
};

typedef struct virtual_page {
    bool in_memory = true;
    VA buffer_pointer = NULL;
};

struct segment {
    struct segment *pNext = NULL;
    unsigned int segment_size = 0;
    unsigned int offsetBlock = 0;
    virtual_page *pages = NULL;

};

struct memory_dispatcher {
    struct segment *first_segment = NULL;
    int page_size = 0;
    VA allocated_buffer = NULL;

    size_t allocated_pages_amount = 0;
    size_t reserved_pages_amount = 0;

    int page_offset_bits;
    int page_offset_mask;

    ~memory_dispatcher() {
        delete[] allocated_buffer;
    }
};

memory_dispatcher dispatcher;

bool check_enough_memory(size_t block_size);

int allocate_memory(VA *ptr, size_t segment_size);

segment *get_first_free_segment(size_t size);

segment *construct_segment(size_t offset, size_t size);

VA calculate_segment_ptr(segment *current_segment);

int get_segment(segment **segment_ptr, size_t &segment_offset, VA string);

int write_buffer_to_segment(segment segment_ptr, size_t segment_offset, void *buffer_ptr, size_t buffer_size);

void write_buffer_to_page(char *page_begin, char *page_end, char *buffer_begin, char *buffer_end);

int read_buffer_from_segment(segment segment_ptr, size_t segment_offset, void *buffer_ptr, size_t buffer_size);

void read_buffer_from_page(char *page_start, char *page_end, char *buffer_ptr);

VA calculate_virtual_offset(segment *segment_ptr);

int init_pages_offset(size_t szPage);

bool check_enough_memory(size_t block_size);

int allocate_memory(VA *ptr, size_t segment_size);

segment *get_first_free_segment(size_t size);

size_t get_required_pages_amount(size_t block_size);

segment *construct_segment(size_t offset, size_t size) {
    segment *existed_segment;
    existed_segment->offsetBlock = offset;
    existed_segment->pNext = NULL;
    size_t required_pages_amount = get_required_pages_amount(size);
    existed_segment->segment_size = required_pages_amount * dispatcher.page_size;
    existed_segment->pages = new virtual_page[required_pages_amount];

    for (int i = 0; i < required_pages_amount; i++) {
        existed_segment->pages->buffer_pointer =
                dispatcher.allocated_buffer + existed_segment->offsetBlock +
                dispatcher.page_size * i;
    }
    return existed_segment;
};

VA calculate_segment_ptr(segment *current_segment);

int get_segment(segment **segment_ptr, size_t &segment_offset, VA string);

int write_buffer_to_segment(segment segment_ptr, size_t segment_offset, void *buffer_ptr, size_t buffer_size);

void write_buffer_to_page(char *page_begin, char *page_end, char *buffer_begin, char *buffer_end);

int read_buffer_from_segment(segment segment_ptr, size_t segment_offset, void *buffer_ptr, size_t buffer_size);

void read_buffer_from_page(char *page_start, char *page_end, char *buffer_ptr);

VA calculate_virtual_offset(segment *segment_ptr);

int init_pages_offset(size_t szPage);

int _malloc(VA *ptr, size_t szBlock) {
    if (!check_enough_memory(szBlock)) { return NOT_ENOUGH_MEMORY_ERROR; }
    int err = allocate_memory(ptr, szBlock);
    return err;
}

int allocate_memory(VA *ptr, size_t segment_size) {
    segment *free_segment = get_first_free_segment(segment_size);
    VA asd = calculate_segment_ptr(free_segment);
    *ptr = calculate_virtual_offset(free_segment);
    return 0;
}

VA calculate_virtual_offset(segment *segment_ptr) {
    size_t offset = 0;
    segment *current_segment = dispatcher.first_segment;
    while (current_segment != segment_ptr) {
        offset += current_segment->segment_size;
        current_segment = current_segment->pNext;
    }
    return (VA) dispatcher.allocated_buffer + offset;
}

VA calculate_segment_ptr(segment *current_segment) {
    return calculate_virtual_offset(current_segment);
}

segment *get_first_free_segment(size_t size) {
    segment *existed_segment = dispatcher.first_segment;
    if (existed_segment == NULL) {
        segment *ptr = construct_segment(0, size);
        dispatcher.first_segment = ptr;
        return ptr;
    }
    while (existed_segment->pNext != NULL) {
        existed_segment = existed_segment->pNext;
    }
    return construct_segment(existed_segment->offsetBlock + existed_segment->segment_size, size);
}


bool check_enough_memory(size_t block_size) {
    double required_pages_amount = get_required_pages_amount(block_size);
    return (dispatcher.allocated_pages_amount - dispatcher.reserved_pages_amount) >= required_pages_amount;
}

size_t get_required_pages_amount(size_t block_size) {
    int required_pages_amount = block_size / dispatcher.page_size;
    int rest = block_size % dispatcher.page_size == 0 ? 0 : 1;
    required_pages_amount += rest;
    return required_pages_amount;
}

int _free(VA ptr) {
    return -1;
}

int _read(VA ptr, void *buffer_ptr, size_t buffer_size) {
    segment *segment;
    size_t segment_offset;
    int err = get_segment(&segment, segment_offset, ptr);
    if (err) {
        return err;
    }
    if (!(segment_offset + buffer_size) >= segment->segment_size) {
        return OUT_OF_RANGE_ERROR;
    }

    err = read_buffer_from_segment(*segment, segment_offset, buffer_ptr, buffer_size);
    return err;
}

int read_buffer_from_segment(segment segment_ptr, size_t segment_offset, void *buffer_ptr, size_t buffer_size) {
    size_t page_number = segment_offset >> dispatcher.page_offset_bits;
    size_t first_page_offset = segment_offset & dispatcher.page_offset_mask;

    virtual_page page = segment_ptr.pages[page_number];

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;
    while (buffer_offset < buffer_size) {

        read_buffer_from_page(page.buffer_pointer + page_offset,
                              page.buffer_pointer + dispatcher.page_size,
                              (char *) buffer_ptr + buffer_offset);
        buffer_offset += (dispatcher.page_size - page_offset);
        page_offset = 0;
        page = segment_ptr.pages[++page_number];
    }


    return 0;
}

void read_buffer_from_page(char *page_start, char *page_end, char *buffer_ptr) {
    for (VA current_page_byte = page_start; current_page_byte < page_end; current_page_byte++, buffer_ptr++) {
        *buffer_ptr = *current_page_byte;
    }
}

int _write(VA ptr, void *buffer_ptr, size_t buffer_size) {
    segment *segment_ptr;
    size_t segment_offset;
    int err = get_segment(&segment_ptr, segment_offset, ptr);
    if (err != 0) {
        return err;
    }

    if (!segment_offset + buffer_size >= segment_ptr->segment_size) {
        return OUT_OF_RANGE_ERROR;
    }
    err = write_buffer_to_segment(*segment_ptr, segment_offset, buffer_ptr, buffer_size);
    return err;
}

int write_buffer_to_segment(segment segment_ptr, size_t segment_offset, void *buffer_ptr, size_t buffer_size) {
    size_t page_number = segment_offset >> dispatcher.page_offset_bits;
    size_t first_page_offset = segment_offset & dispatcher.page_offset_mask;

    size_t page_offset = first_page_offset;
    size_t buffer_offset = 0;

    while (buffer_offset < buffer_size) {
        virtual_page *page = &segment_ptr.pages[page_number];
        memcpy((char *) page->buffer_pointer + page_offset, (char *) buffer_ptr + buffer_offset,
               dispatcher.page_size - page_offset);
        buffer_offset += (dispatcher.page_size - page_offset);
        page_offset = 0;
        page_number++;
    }
    return 0;
}

int get_segment(segment **segment_ptr, size_t &segment_offset, VA ptr) {
    size_t offset = ptr - dispatcher.allocated_buffer;
    if (offset < 0 || offset >= dispatcher.allocated_pages_amount * dispatcher.page_size) {
        return OUT_OF_RANGE_ERROR;
    }
    segment *current_segment = dispatcher.first_segment;

    while (offset > 0 + current_segment->segment_size) {
        offset -= current_segment->segment_size;
        if (offset > 0) {
            if (current_segment->pNext == NULL) {
                return OUT_OF_RANGE_ERROR;
            }
            current_segment = current_segment->pNext;
        }
    }
    segment_offset = offset;
    *segment_ptr = current_segment;
    return SUCCESSFUL_CODE;
}

int isPow2(int a) {
    return !(a & (a - 1));
}

int _init(int n, size_t szPage) {
    dispatcher.~memory_dispatcher();
    if (n <= 0 || szPage <= 0) { return INCORRECT_PARAMETERS_ERROR; }

    int err = init_pages_offset(szPage);
    if (err) {
        return err;
    }

    dispatcher = memory_dispatcher();
    dispatcher.allocated_pages_amount = n;
    dispatcher.page_size = szPage;
    dispatcher.allocated_buffer = new char[n * szPage];
    dispatcher.reserved_pages_amount = 0;
    if (dispatcher.allocated_buffer == NULL) { return UNKNOWN_ERROR; }
    return 0;
}

int init_pages_offset(size_t szPage) {
    if (!isPow2(szPage)) {
        return INCORRECT_PARAMETERS_ERROR;
    }
    dispatcher.page_offset_mask = szPage - 1;
    dispatcher.page_offset_bits = log2(szPage);
    return SUCCESSFUL_CODE;
}