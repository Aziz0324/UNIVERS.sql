#include "my_ls.h"
#include <stdlib.h>

static int compare_timespec_desc_then_name(struct timespec a, struct timespec b, const char *na, const char *nb)
{
	if (a.tv_sec != b.tv_sec) return (a.tv_sec > b.tv_sec) ? -1 : 1;
	if (a.tv_nsec != b.tv_nsec) return (a.tv_nsec > b.tv_nsec) ? -1 : 1;
	return str_cmp(na, nb);
}

static int compare_entries_by_name(const Entry *e1, const Entry *e2)
{
	return str_cmp(e1->name, e2->name);
}

static int compare_entries_by_time_then_name(const Entry *e1, const Entry *e2)
{
	return compare_timespec_desc_then_name(e1->mtime, e2->mtime, e1->name, e2->name);
}

void sort_entries(Entry *entries, size_t count, int use_time)
{
	if (!entries || count < 2) return;
	/* insertion sort for simplicity and stability */
	for (size_t i = 1; i < count; i++) {
		Entry key = entries[i];
		size_t j = i;
		while (j > 0) {
			int cmp = use_time ? compare_entries_by_time_then_name(&entries[j - 1], &key)
							 : compare_entries_by_name(&entries[j - 1], &key);
			if (cmp <= 0) break;
			entries[j] = entries[j - 1];
			j--;
		}
		entries[j] = key;
	}
}

static int compare_operands_by_name(const Operand *o1, const Operand *o2)
{
	return str_cmp(o1->path, o2->path);
}

static int compare_operands_by_time_then_name(const Operand *o1, const Operand *o2)
{
	return compare_timespec_desc_then_name(o1->mtime, o2->mtime, o1->path, o2->path);
}

void sort_operands(Operand *operands, size_t count, int use_time)
{
	if (!operands || count < 2) return;
	for (size_t i = 1; i < count; i++) {
		Operand key = operands[i];
		size_t j = i;
		while (j > 0) {
			int cmp = use_time ? compare_operands_by_time_then_name(&operands[j - 1], &key)
							 : compare_operands_by_name(&operands[j - 1], &key);
			if (cmp <= 0) break;
			operands[j] = operands[j - 1];
			j--;
		}
		operands[j] = key;
	}
}