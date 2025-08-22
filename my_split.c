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

string_array* my_split(char* param_1, char* param_2)
{
	string_array *result;
	char sep;
	int total_len;
	int tokens;
	int i;
	int start;
	int idx;

	if (!param_1) {
		result = (string_array *)malloc(sizeof(string_array));
		if (!result) {
			return NULL;
		}
		result->size = 0;
		result->array = NULL;
		return result;
	}

	total_len = my_strlen(param_1);
	if (total_len == 0) {
		result = (string_array *)malloc(sizeof(string_array));
		if (!result) {
			return NULL;
		}
		result->size = 0;
		result->array = NULL;
		return result;
	}

	sep = (param_2 && param_2[0] != '\0') ? param_2[0] : '\0';

	if (sep == '\0') {
		/* No valid separator provided: return the whole string as a single element */
		result = (string_array *)malloc(sizeof(string_array));
		if (!result) {
			return NULL;
		}
		result->size = 1;
		result->array = (char **)malloc(sizeof(char *));
		if (!result->array) {
			return result; /* Allocation failed; best effort within constraints */
		}
		result->array[0] = (char *)malloc((total_len + 1) * sizeof(char));
		if (result->array[0]) {
			for (i = 0; i < total_len; i++) {
				result->array[0][i] = param_1[i];
			}
			result->array[0][total_len] = '\0';
		}
		return result;
	}

	tokens = count_tokens(param_1, sep);
	result = (string_array *)malloc(sizeof(string_array));
	if (!result) {
		return NULL;
	}
	result->size = tokens;
	if (tokens == 0) {
		result->array = NULL;
		return result;
	}

	result->array = (char **)malloc(sizeof(char *) * tokens);
	if (!result->array) {
		return result; /* Allocation failed; best effort within constraints */
	}

	i = 0;
	start = -1;
	idx = 0;
	while (param_1[i] != '\0') {
		if (param_1[i] == sep) {
			if (start != -1) {
				result->array[idx] = copy_segment(param_1, start, i);
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
		result->array[idx] = copy_segment(param_1, start, i);
	}

	return result;
}