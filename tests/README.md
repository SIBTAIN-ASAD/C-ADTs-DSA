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

# BST storage and input regression tests

```sh
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined \
  tests/bst_storage_test.c -o /tmp/bst-storage-test
/tmp/bst-storage-test
python3 tests/test_bst_input.py
```

The storage tests cover duplicate insertions, 19-character word boundaries,
empty trees, and exporting 150 unique words. Input tests compile a sanitized
binary and use temporary files to check duplicate frequencies, truncated commands,
empty input, and overlong tokens. The word limit remains 19 characters; longer
words are rejected, not silently truncated.
