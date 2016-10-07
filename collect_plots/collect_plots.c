#include <time.h>
#include <malloc.h>
#include <string.h>
#include <mmemory.c>

typedef struct {
    double x;
    double y;
} point;

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
    for (int page_size = 1; page_size <= 512; page_size <<= 1, point++) {
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
        points[point].y = (double) counter() / (1000 * block_size / page_size);
    }


    char filename[200];
    sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/pump_page.txt");
    write_plot_to_file(filename, points, point);
}

void page_fault_overhead() {
    int amount = 200;
    size_type block_size = 512;
    VA *blocks = (VA *) malloc(sizeof(VA) * amount);
    for (int page_size = 1; page_size <= 512; page_size <<= 1) {
        point *points = (point *) malloc(sizeof(point) * 200);
        for (int block_amount = amount; block_amount < 400; block_amount += 10) {
            ___init(block_size / page_size * block_amount, page_size);

            for (int i = 0; i < amount; i++) {
                char *test_buffer = (char *) malloc(sizeof(char) * block_size);
                _malloc(&blocks[i], block_size);
                _write(blocks[i], test_buffer, block_size);
                free(test_buffer);
            }

            char *test_buffer = (char *) malloc(sizeof(char) * block_size);
            clock_t start = clock();
            for (int i = 0; i < 1000; i++) {
                _read(blocks[rand() % amount], test_buffer, block_size);
            }
            clock_t time = clock() - start;
            points[(block_amount - amount) / 10].x = (double) counter() / (1000 * block_size / page_size);
            points[(block_amount - amount) / 10].y = time;

            char filename[200];
            sprintf(filename, "/home/rizhi-kote/Student/rodia/OSLab_2/page_fault_overhead_%i.txt", page_size);
            write_plot_to_file(filename, points, amount / 10);
        }
    }
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
    return 0;
}