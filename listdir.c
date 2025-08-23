#include "my_ls.h"
#include <stdlib.h>
#include <stdio.h>

static int entries_append(Entry **arr, size_t *count, size_t *cap, Entry ent)
{
	Entry *new_arr;
	size_t new_cap;
	if (*count + 1 <= *cap) {
		(*arr)[*count] = ent;
		(*count)++;
		return 0;
	}
	new_cap = (*cap == 0) ? 8 : (*cap * 2);
	new_arr = (Entry *)malloc(new_cap * sizeof(Entry));
	if (!new_arr) return -1;
	for (size_t i = 0; i < *count; i++) new_arr[i] = (*arr)[i];
	free(*arr);
	*arr = new_arr;
	*cap = new_cap;
	(*arr)[*count] = ent;
	(*count)++;
	return 0;
}

int collect_directory_entries(const char *path, const Options *opts, Entry **out_entries, size_t *out_count)
{
	DIR *dir;
	struct dirent *de;
	Entry *entries = NULL;
	size_t count = 0;
	size_t cap = 0;
	
	dir = opendir(path);
	if (!dir) return -1;
	while ((de = readdir(dir)) != NULL) {
		const char *name = de->d_name;
		if (!opts->flag_all && name[0] == '.') continue;
		char *full = path_join(path, name);
		if (!full) { closedir(dir); free(entries); return -1; }
		struct timespec mt;
		if (get_stat_mtime_lstat(full, &mt) != 0) {
			free(full);
			/* skip entries we cannot stat */
			continue;
		}
		Entry ent;
		ent.name = str_dup(name);
		ent.mtime = mt;
		free(full);
		if (!ent.name) { closedir(dir); free(entries); return -1; }
		if (entries_append(&entries, &count, &cap, ent) != 0) {
			free(ent.name);
			closedir(dir);
			free(entries);
			return -1;
		}
	}
	closedir(dir);
	*out_entries = entries;
	*out_count = count;
	return 0;
}

void free_entries(Entry *entries, size_t count)
{
	if (!entries) return;
	for (size_t i = 0; i < count; i++) {
		if (entries[i].name) free(entries[i].name);
	}
	free(entries);
}

int list_directory(const char *path, const Options *opts)
{
	Entry *entries = NULL;
	size_t count = 0;
	if (collect_directory_entries(path, opts, &entries, &count) != 0) return -1;
	sort_entries(entries, count, opts->flag_time);
	for (size_t i = 0; i < count; i++) {
		printf("%s\n", entries[i].name);
	}
	free_entries(entries, count);
	return 0;
}