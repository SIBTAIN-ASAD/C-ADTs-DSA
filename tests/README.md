# Heap output-sort regression tests

From the repository root, compile and run with Clang or GCC:

```sh
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined \
  tests/heap_sort_test.c -o /tmp/heap-sort-test
/tmp/heap-sort-test
```

The test exercises the output-sorting function without running the input-file
simulation. It covers empty and singleton ranges, equal completion times, integer
limits, partial-array sorting, and 1,000 records with repeated completion times.
It verifies both chronological order and preservation of every customer record.
