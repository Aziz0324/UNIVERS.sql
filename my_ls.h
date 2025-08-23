#ifndef MY_LS_H
#define MY_LS_H

#include <stddef.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct s_options {
	int flag_all;
	int flag_time;
} Options;

typedef struct s_entry {
	char *name;
	struct timespec mtime;
} Entry;

typedef struct s_operand {
	char *path;
	int is_directory;
	struct timespec mtime;
} Operand;

/* option parsing */
int parse_options(int argc, char **argv, Options *options, int *first_operand_index);

/* path and stat helpers */
int is_directory_path(const char *path, struct stat *out_statbuf);
int get_stat_mtime_lstat(const char *path, struct timespec *out_mtime);

/* basic string and memory helpers (no forbidden libc calls) */
size_t str_len(const char *s);
int str_cmp(const char *a, const char *b);
char *str_dup(const char *s);
char *path_join(const char *dir, const char *name);

/* sorting */
void sort_entries(Entry *entries, size_t count, int use_time);
void sort_operands(Operand *operands, size_t count, int use_time);

/* directory listing */
int collect_directory_entries(const char *path, const Options *opts, Entry **out_entries, size_t *out_count);
void free_entries(Entry *entries, size_t count);
int list_directory(const char *path, const Options *opts);

#endif /* MY_LS_H */