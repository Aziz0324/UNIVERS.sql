#include "my_ls.h"
#include <stdlib.h>
#include <sys/stat.h>

size_t str_len(const char *s)
{
	size_t len = 0;
	if (!s) return 0;
	while (s[len] != '\0') len++;
	return len;
}

int str_cmp(const char *a, const char *b)
{
	size_t i = 0;
	unsigned char ca;
	unsigned char cb;
	if (a == b) return 0;
	if (!a) return -1;
	if (!b) return 1;
	while (1) {
		ca = (unsigned char)a[i];
		cb = (unsigned char)b[i];
		if (ca != cb) return (int)ca - (int)cb;
		if (ca == '\0') return 0;
		i++;
	}
}

char *str_dup(const char *s)
{
	size_t len = str_len(s);
	char *copy = (char *)malloc(len + 1);
	size_t i;
	if (!copy) return NULL;
	for (i = 0; i < len; i++) copy[i] = s[i];
	copy[len] = '\0';
	return copy;
}

char *path_join(const char *dir, const char *name)
{
	size_t len_dir = str_len(dir);
	size_t len_name = str_len(name);
	int need_slash = 1;
	char *result;
	size_t i = 0;
	if (len_dir > 0 && dir[len_dir - 1] == '/') need_slash = 0;
	result = (char *)malloc(len_dir + need_slash + len_name + 1);
	if (!result) return NULL;
	for (i = 0; i < len_dir; i++) result[i] = dir[i];
	if (need_slash) {
		result[len_dir] = '/';
		for (i = 0; i < len_name; i++) result[len_dir + 1 + i] = name[i];
		result[len_dir + 1 + len_name] = '\0';
	} else {
		for (i = 0; i < len_name; i++) result[len_dir + i] = name[i];
		result[len_dir + len_name] = '\0';
	}
	return result;
}

int get_stat_mtime_lstat(const char *path, struct timespec *out_mtime)
{
	struct stat st;
	if (lstat(path, &st) != 0) return -1;
	*out_mtime = st.st_mtim;
	return 0;
}

int is_directory_path(const char *path, struct stat *out_statbuf)
{
	struct stat st;
	if (lstat(path, &st) != 0) return 0;
	if (out_statbuf) *out_statbuf = st;
	return S_ISDIR(st.st_mode) ? 1 : 0;
}