#include <stdio.h>
#include <stdlib.h>

static void print_repeated_char(char character, int count) {
    for (int i = 0; i < count; ++i) {
        putchar(character);
    }
}

static int compute_tree_max_width(int size) {
    if (size <= 0) {
        return 0;
    }

    int start_width_current_block = 1;
    int end_width_current_block = 1;

    for (int block_index_one_based = 1; block_index_one_based <= size; ++block_index_one_based) {
        int block_height = block_index_one_based + 3;
        end_width_current_block = start_width_current_block + 2 * (block_height - 1);

        if (block_index_one_based < size) {
            int reduction = 2 * ((block_index_one_based + 1) / 2);
            start_width_current_block = end_width_current_block - reduction;
        }
    }

    return end_width_current_block;
}

static void print_tree(int size) {
    if (size <= 0) {
        return;
    }

    int max_width = compute_tree_max_width(size);
    int start_width_current_block = 1;

    for (int block_index_one_based = 1; block_index_one_based <= size; ++block_index_one_based) {
        int block_height = block_index_one_based + 3;

        for (int line_index = 0; line_index < block_height; ++line_index) {
            int stars_on_this_line = start_width_current_block + 2 * line_index;
            int spaces_on_this_line = (max_width - stars_on_this_line) / 2;
            print_repeated_char(' ', spaces_on_this_line);
            print_repeated_char('*', stars_on_this_line);
            putchar('\n');
        }

        int end_width_current_block = start_width_current_block + 2 * (block_height - 1);
        int reduction = 2 * ((block_index_one_based + 1) / 2);
        start_width_current_block = end_width_current_block - reduction;
    }

    int trunk_height = size;
    int trunk_width = (size % 2 == 0) ? (size + 1) : size;
    int trunk_left_padding = (max_width - trunk_width) / 2;

    for (int i = 0; i < trunk_height; ++i) {
        print_repeated_char(' ', trunk_left_padding);
        print_repeated_char('|', trunk_width);
        putchar('\n');
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 0;
    }

    char *endptr = NULL;
    long parsed = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0') {
        return 0;
    }
    if (parsed < 0 || parsed > 1000000) {
        return 0;
    }

    int size = (int)parsed;
    print_tree(size);
    return 0;
}

