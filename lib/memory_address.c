
#include "memory_address.h"

memory_address *create_memory_address() {
    memory_address *address = (memory_address *) malloc(sizeof(memory_address));
    address->page_num = 0;
    address->segment_num = 0;
    address->page_offset = 0;
    return address;
}
