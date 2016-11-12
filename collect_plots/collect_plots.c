#include <time.h>
#include <malloc.h>
#include <string.h>
#include <mmemory.c>


typedef struct {
    double x;
    double y;
} point;

void calc_average_value();

int getInt(size_type max_block_size, int page_size, int point_amount, point *points, int point_num, VA const *blocks);

void calc_address_simulation(memory_dispatcher *dispatcher, memory_address *ptr, VA block, int size);

int getCalcAddressOverheadToPageSize(point *);

void write_plot_to_file(char *filename, point *plot, int plot_size) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("ошибка открытия пример.txt");
        return;
    }
    char str[80];
    for (int i = 0; i < plot_size; i++) {
        sprintf(str, "%f %f\n", plot[i].x, plot[i].y);
        fwrite(str, strlen(str), 1, f);
    }
    fclose(f);
}

void get_plot_write_plot(int point_amount, int page_amount, int page_size, int block_size) {
    point *points = (point *) malloc(sizeof(point) * point_amount);
    ___init(page_amount / 2, page_size);
    for (int i = 0; i < point_amount; i++) {
        VA block;
        char *test_buffer = (char *) malloc(sizeof(char) * block_size);
        _malloc(&block, block_size);
        unsigned long start = clock();
        for (int j = 0; j < 1000; j++) {
            _write(block, test_buffer, block_size);
        }
        unsigned long end = clock();
        points[i].x = i;
        points[i].y = end - start;
        free(test_buffer);
    }
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/write_plot_%i_%i_%i.txt", page_amount, page_size,
            block_size);
    write_plot_to_file(filename, points, point_amount);
}

void get_plot_malloc(int point_amount, int page_amount, int page_size, int block_size) {
    point *points = (point *) malloc(sizeof(point) * point_amount);
    ___init(page_amount / 2, page_size);
    for (int i = 0; i < point_amount; i++) {
        VA block;
        unsigned long start = clock();
        _malloc(&block, block_size);
        unsigned long end = clock();
        points[i].x = i;
        points[i].y = end - start;
    }
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/malloc_plot_%i_%i_%i.txt", page_amount, page_size,
            block_size);
    write_plot_to_file(filename, points, point_amount);
}

void get_plot_internal_write_plot(int point_amount, int page_amount, int page_size, int block_size) {
    point *points = (point *) malloc(sizeof(point) * point_amount);
    ___init(page_amount / 2, page_size);
    VA *blocks = (VA *) malloc(sizeof(VA) * point_amount);

    for (int i = 0; i < point_amount; i++) {
        VA block;
        char *test_buffer = (char *) malloc(sizeof(char) * block_size);
        _malloc(&block, block_size);
        blocks[i] = block;
        free(test_buffer);
    }
    for (int i = 0; i < point_amount; i++) {
        unsigned long start = clock();
        for (int j = 0; j < 1000; j++) {
            _write(blocks[i], blocks[point_amount - i - 1], block_size);
        }
        unsigned long end = clock();
        points[i].x = i;
        points[i].y = end - start;
    }
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/internal_write_plot_%i_%i_%i.txt", page_amount, page_size,
            block_size);
    write_plot_to_file(filename, points, point_amount);
}

void get_plot_read_plot(int point_amount, int page_amount, int page_size, int block_size) {
    point *points = (point *) malloc(sizeof(point) * point_amount);
    ___init(page_amount / 2, page_size);
    VA *blocks = (VA *) malloc(sizeof(VA) * point_amount);

    for (int i = 0; i < point_amount; i++) {
        VA block;
        char *test_buffer = (char *) malloc(sizeof(char) * block_size);
        _malloc(&block, block_size);
        _write(blocks[i], blocks[point_amount - i - 1], block_size);
        blocks[i] = block;
        free(test_buffer);
    }
    for (int i = 0; i < point_amount; i++) {
        char *test_buffer = (char *) malloc(sizeof(char) * block_size);
        unsigned long start = clock();
        for (int j = 0; j < 1000; j++) {
            _read(blocks[i], test_buffer, block_size);
        }
        unsigned long end = clock();
        points[i].x = i;
        points[i].y = end - start;
        free(test_buffer);
    }
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/read_plot_%i_%i_%i.txt", page_amount, page_size,
            block_size);
    write_plot_to_file(filename, points, point_amount);
}

void time_to_calc_address(size_type point_amount, size_type block_size) {
    point *points = (point *) malloc(sizeof(point) * point_amount);
    VA *blocks = (VA *) malloc(sizeof(VA) * point_amount);
    ___init(point_amount, block_size);

    for (int i = 0; i < point_amount; i++) {
        char *test_buffer = (char *) malloc(sizeof(char) * block_size);
        _malloc(&blocks[i], block_size);
        _write(blocks[i], test_buffer, block_size);
        free(test_buffer);
    }

    memory_address address;
    for (int j = 0; j < point_amount; j++) {
        time_t start = clock();
        for (int i = 0; i < 10000; i++) {
            get_segment(dispatcher, &address, blocks[j]);
        }
        time_t time = clock() - start;
        points[j].x = j;
        points[j].y = time;
    }
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/get_address.txt");
    write_plot_to_file(filename, points, point_amount);
}

void number_of_pump_up_page_size() {
    point *points = (point *) malloc(sizeof(point) * 10);
    int point = 0;
    int block_amount = 200;
    size_type block_size = 512;
    VA *blocks = (VA *) malloc(sizeof(VA) * block_amount);
    for (int page_size = 8; page_size <= 512; page_size <<= 1, point++) {
        ___init(block_size / page_size * block_amount, page_size);

        for (int i = 0; i < block_amount; i++) {
            char *test_buffer = (char *) malloc(sizeof(char) * block_size);
            _malloc(&blocks[i], block_size);
            _write(blocks[i], test_buffer, block_size);
            free(test_buffer);
        }

        char *test_buffer = (char *) malloc(sizeof(char) * block_size);
        for (int i = 0; i < 1000; i++) {
            _read(blocks[rand() % block_amount], test_buffer, block_size);
        }
        points[point].x = page_size;
        points[point].y = counter();
    }


    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/pump_page.txt");
    write_plot_to_file(filename, points, point);
}

void page_fault_overhead() {
    size_type block_size = 4096;
    point *points = (point *) malloc(sizeof(point) * 200);
    int point_num = 0;
    for (int page_size = 128; page_size <= 4096; page_size <<= 1, point_num++) {

        ___init(2, page_size);

        memory_address address;
        VA block;
        page *pages[2];
        _malloc(&block, page_size);
        get_segment(dispatcher, &address, block);

        segment *segment1 = dispatcher->segments[address.segment_num];
        pages[1] = segment1->pages[0];

        _malloc(&block, page_size);
        get_segment(dispatcher, &address, block);
        segment *segment2 = dispatcher->segments[address.segment_num];
        pages[0] = segment2->pages[0];

        int times = (block_size / page_size) * 10000.;
        clock_t start = clock();
        for (int i = 0, block_num = 0; i < times; i++, block_num = (block_num + 1) % 2) {
            page_pump_up(dispatcher->pager, pages[block_num]);
            }
        clock_t time = clock() - start;
        points[point_num].x = page_size;
        points[point_num].y = time;

    }
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/plots/page_fault_overhead/page_fault_overhead.txt");
    write_plot_to_file(filename, points, point_num);
}

size_type get_physical_memory_address_multipli(memory_pager *self, segment *sgm, size_type offset, size_t page_size) {

    page *current_page = sgm->pages[offset / page_size];
    if (current_page->is_in_memmory == false) {
        page_pump_up(self, current_page);
    }
    return current_page->offset + (offset & self->page_offset_mask);
}

void get_segment_overheat_to_page_size() {
    int point_amount = 100000;
    point *points = (point *) malloc(sizeof(point) * point_amount);

    size_t page_size = 64;
    size_t readen_buffer_size = 100000;
    ___init(2 * readen_buffer_size / page_size + 1, page_size);
    VA block;
    _malloc(&block, readen_buffer_size);
    clock_t start = clock();
    segment *current_segment = dispatcher->segments[0];
    for (int i = 0; i < readen_buffer_size; i++) {
        get_physical_memory_address(dispatcher->pager, current_segment, i);
    }
    points[0].x = 0;
    points[0].y = clock() - start;

    start = clock();
    for (int i = 0; i < readen_buffer_size; i++) {
        get_physical_memory_address_multipli(dispatcher->pager, current_segment, i, page_size);
    }
    points[1].x = 1;
    points[1].y = clock() - start;

    point_amount = 2;
    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/plots/get_address/get_address_page_fault.txt");
    write_plot_to_file(filename, points, point_amount);
}

int main() {
//    int sizes[] = {32, 64, 128, 256};
//    int pages_amount_mask = 1 << 4;
//    int block_size_mask = 3 << 2;
//    int page_size_mask = 3;
//    for (int i = 0; i < 32; i++) {
//        int block_size = (i & block_size_mask) >> 2;
//        int page_size = i & page_size_mask;
//        int pages_amount = 1 << ((i & pages_amount_mask) >> 4);
//        if (page_size > block_size) {
//            continue;
//        }
////        get_plot_internal_write_plot(200, 200 * sizes[block_size] / sizes[page_size] * pages_amount, sizes[page_size],
////                                     sizes[block_size]);
//        get_plot_write_plot(200, 200 * sizes[block_size] / sizes[page_size] * pages_amount, sizes[page_size],
//                                     sizes[block_size]);
////        get_plot_read_plot(200, 200 * sizes[block_size] / sizes[page_size] * pages_amount, sizes[page_size],
////                           sizes[block_size]);
//    }

    page_fault_overhead();
//    get_segment_overheat_to_page_size();
    return 0;
}
