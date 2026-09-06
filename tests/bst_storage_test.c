#define main bst_program_main
#include "../BST/bst_main.c"
#undef main
#include <assert.h>

int main(void)
{
    add("same");
    struct tree_Node *first = root;
    for (int i = 0; i < 1000; ++i) add("same");
    assert(root == first && countNodes(root) == 1);
    assert(find("same")->NodeValue.fre_of_value == 1001);
    FreeHeap(root);
    root = NULL;

    add("1234567890123456789");
    assert(find("1234567890123456789") != NULL);
    add("12345678901234567890");
    assert(countNodes(root) == 1);
    FreeHeap(root);
    root = NULL;

    for (int i = 0; i < 150; ++i) {
        char word[20];
        snprintf(word, sizeof(word), "word%03d", i);
        add(word);
    }
    assert(countNodes(root) == 150);
    FILE *output = tmpfile();
    assert(output != NULL);
    ArrayFunctionality(&output);
    rewind(output);
    for (int i = 0; i < 150; ++i) {
        char word[20], expected[20];
        int frequency = 0;
        snprintf(expected, sizeof(expected), "word%03d", i);
        assert(fscanf(output, "%19s %d", word, &frequency) == 2);
        assert(strcmp(word, expected) == 0 && frequency == 1);
    }
    fclose(output);
    FreeHeap(root);
    root = NULL;
    output = tmpfile();
    assert(output != NULL);
    ArrayFunctionality(&output);
    assert(ftell(output) == 0);
    fclose(output);
    puts("BST storage tests passed");
    return 0;
}
