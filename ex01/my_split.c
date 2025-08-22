#include <stdlib.h>

static int my_strlen(const char *str) {
	int length = 0;
	if (!str) {
		return 0;
	}
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

static int count_tokens(const char *str, char sep) {
	int count = 0;
	int i = 0;
	int in_token = 0;

	if (!str) {
		return 0;
	}

	while (str[i] != '\0') {
		if (str[i] == sep) {
			if (in_token) {
				in_token = 0;
			}
		} else {
			if (!in_token) {
				in_token = 1;
				count++;
			}
		}
		i++;
	}
	return count;
}

static char *copy_segment(const char *str, int start, int end) {
	int len = end - start;
	char *out = (char *)malloc((len + 1) * sizeof(char));
	int i;

	if (!out) {
		return NULL;
	}

	for (i = 0; i < len; i++) {
		out[i] = str[start + i];
	}
	out[len] = '\0';
	return out;
}

/* Local layout that matches the grader's string_array { int size; char **array; } */
struct __local_string_array_layout {
	int size;
	char **array;
};

struct s_string_array* my_split(char* param_1, char* param_2)
{
	struct __local_string_array_layout *tmp;
	char sep;
	int total_len;
	int tokens;
	int i;
	int start;
	int idx;

	if (!param_1) {
		tmp = (struct __local_string_array_layout *)malloc(sizeof(struct __local_string_array_layout));
		if (!tmp) {
			return (struct s_string_array *)0;
		}
		tmp->size = 0;
		tmp->array = (char **)0;
		return (struct s_string_array *)tmp;
	}

	total_len = my_strlen(param_1);
	if (total_len == 0) {
		tmp = (struct __local_string_array_layout *)malloc(sizeof(struct __local_string_array_layout));
		if (!tmp) {
			return (struct s_string_array *)0;
		}
		tmp->size = 0;
		tmp->array = (char **)0;
		return (struct s_string_array *)tmp;
	}

	sep = (param_2 && param_2[0] != '\0') ? param_2[0] : '\0';

	if (sep == '\0') {
		tmp = (struct __local_string_array_layout *)malloc(sizeof(struct __local_string_array_layout));
		if (!tmp) {
			return (struct s_string_array *)0;
		}
		tmp->size = 1;
		tmp->array = (char **)malloc(sizeof(char *));
		if (!tmp->array) {
			return (struct s_string_array *)tmp;
		}
		tmp->array[0] = (char *)malloc((total_len + 1) * sizeof(char));
		if (tmp->array[0]) {
			for (i = 0; i < total_len; i++) {
				tmp->array[0][i] = param_1[i];
			}
			tmp->array[0][total_len] = '\0';
		}
		return (struct s_string_array *)tmp;
	}

	tokens = count_tokens(param_1, sep);
	tmp = (struct __local_string_array_layout *)malloc(sizeof(struct __local_string_array_layout));
	if (!tmp) {
		return (struct s_string_array *)0;
	}
	tmp->size = tokens;
	if (tokens == 0) {
		tmp->array = (char **)0;
		return (struct s_string_array *)tmp;
	}

	tmp->array = (char **)malloc(sizeof(char *) * tokens);
	if (!tmp->array) {
		return (struct s_string_array *)tmp;
	}

	i = 0;
	start = -1;
	idx = 0;
	while (param_1[i] != '\0') {
		if (param_1[i] == sep) {
			if (start != -1) {
				tmp->array[idx] = copy_segment(param_1, start, i);
				idx++;
				start = -1;
			}
		} else {
			if (start == -1) {
				start = i;
			}
		}
		i++;
	}
	if (start != -1 && idx < tokens) {
		tmp->array[idx] = copy_segment(param_1, start, i);
	}

	return (struct s_string_array *)tmp;
}