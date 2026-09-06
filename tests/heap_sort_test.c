#define main heap_program_main
#include "../Heap/heap_main.c"
#undef main
#include <assert.h>
#include <limits.h>

int main(void)
{
    struct Customer empty = {0};
    quickSort(&empty, 0, -1);
    quickSort(&empty, 0, 0);

    struct Customer equal[4] = {{.cus_work = 7}, {.cus_work = 7},
                                {.cus_work = 7}, {.cus_work = 7}};
    quickSort(equal, 0, 3);
    for (int i = 0; i < 4; ++i) assert(equal[i].cus_work == 7);

    struct Customer limits[3] = {{.cus_work = INT_MAX}, {.cus_work = 0},
                                 {.cus_work = INT_MIN}};
    quickSort(limits, 0, 2);
    assert(limits[0].cus_work == INT_MIN);
    assert(limits[1].cus_work == 0);
    assert(limits[2].cus_work == INT_MAX);

    struct Customer partial[5] = {{.cus_work = 99}, {.cus_work = 3}, {.cus_work = 2},
                                  {.cus_work = 1}, {.cus_work = -99}};
    quickSort(partial, 1, 3);
    assert(partial[0].cus_work == 99 && partial[4].cus_work == -99);
    for (int i = 1; i <= 3; ++i) assert(partial[i].cus_work == i);

    struct Customer customers[1000] = {0};
    int seen[1000] = {0};
    for (int i = 0; i < 1000; ++i) {
        customers[i].cus_work = (i * 37) % 23;
        customers[i].cur_arv = i;
    }
    quickSort(customers, 0, 999);
    for (int i = 0; i < 1000; ++i) {
        if (i) assert(customers[i - 1].cus_work <= customers[i].cus_work);
        int id = customers[i].cur_arv;
        assert(id >= 0 && id < 1000 && !seen[id]);
        assert(customers[i].cus_work == (id * 37) % 23);
        seen[id] = 1;
    }
    puts("Heap output sorting tests passed");
    return 0;
}
