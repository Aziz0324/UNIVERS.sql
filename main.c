#include "my_ls.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

static int operands_append(Operand **arr, size_t *count, size_t *cap, Operand op)
{
	Operand *new_arr;
	size_t new_cap;
	if (*count + 1 <= *cap) {
		(*arr)[*count] = op;
		(*count)++;
		return 0;
	}
	new_cap = (*cap == 0) ? 8 : (*cap * 2);
	new_arr = (Operand *)malloc(new_cap * sizeof(Operand));
	if (!new_arr) return -1;
	for (size_t i = 0; i < *count; i++) new_arr[i] = (*arr)[i];
	free(*arr);
	*arr = new_arr;
	*cap = new_cap;
	(*arr)[*count] = op;
	(*count)++;
	return 0;
}

static void free_operands(Operand *ops, size_t count)
{
	if (!ops) return;
	for (size_t i = 0; i < count; i++) {
		if (ops[i].path) free(ops[i].path);
	}
	free(ops);
}

int parse_options(int argc, char **argv, Options *options, int *first_operand_index)
{
	int i = 1;
	options->flag_all = 0;
	options->flag_time = 0;
	while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0') {
		const char *opt = argv[i] + 1;
		int j = 0;
		while (opt[j] != '\0') {
			if (opt[j] == 'a') options->flag_all = 1;
			else if (opt[j] == 't') options->flag_time = 1;
			/* ignore unknown flags */
			j++;
		}
		i++;
	}
	*first_operand_index = i;
	return 0;
}

int main(int argc, char **argv)
{
	Options opts;
	int first_operand = 1;
	Operand *files = NULL, *dirs = NULL;
	size_t files_count = 0, files_cap = 0;
	size_t dirs_count = 0, dirs_cap = 0;
	int total_operands = 0;

	parse_options(argc, argv, &opts, &first_operand);
	if (first_operand >= argc) {
		/* default to current directory */
		Operand op;
		struct stat st;
		op.path = str_dup(".");
		if (!op.path) return 0;
		op.is_directory = 1;
		if (lstat(op.path, &st) == 0) op.mtime = st.st_mtim; else { op.mtime.tv_sec = 0; op.mtime.tv_nsec = 0; }
		if (operands_append(&dirs, &dirs_count, &dirs_cap, op) != 0) {
			free(op.path);
			return 0;
		}
		total_operands = 1;
	} else {
		for (int i = first_operand; i < argc; i++) {
			struct stat st;
			Operand op;
			if (lstat(argv[i], &st) != 0) {
				/* skip invalid operand */
				continue;
			}
			op.path = str_dup(argv[i]);
			if (!op.path) continue;
			op.mtime = st.st_mtim;
			op.is_directory = S_ISDIR(st.st_mode) ? 1 : 0;
			if (op.is_directory) {
				if (operands_append(&dirs, &dirs_count, &dirs_cap, op) != 0) { free(op.path); continue; }
			} else {
				if (operands_append(&files, &files_count, &files_cap, op) != 0) { free(op.path); continue; }
			}
			total_operands++;
		}
		if (total_operands == 0) {
			/* nothing valid: behave like current directory */
			Operand op;
			struct stat st;
			op.path = str_dup(".");
			if (!op.path) return 0;
			op.is_directory = 1;
			if (lstat(op.path, &st) == 0) op.mtime = st.st_mtim; else { op.mtime.tv_sec = 0; op.mtime.tv_nsec = 0; }
			if (operands_append(&dirs, &dirs_count, &dirs_cap, op) != 0) { free(op.path); return 0; }
			total_operands = 1;
		}
	}

	/* sort operands */
	sort_operands(files, files_count, opts.flag_time);
	sort_operands(dirs, dirs_count, opts.flag_time);

	/* print non-directories */
	for (size_t i = 0; i < files_count; i++) {
		printf("%s\n", files[i].path);
	}

	/* print directories */
	for (size_t i = 0; i < dirs_count; i++) {
		if ((files_count > 0) || (dirs_count > 1)) {
			if (i > 0 || files_count > 0) printf("\n");
			printf("%s:\n", dirs[i].path);
		}
		list_directory(dirs[i].path, &opts);
	}

	free_operands(files, files_count);
	free_operands(dirs, dirs_count);
	return 0;
}