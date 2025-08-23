#include <unistd.h>
#include <stdlib.h>

// Global storage for buffered data between calls
char *g_storage = NULL;

// Global read size (mutable; tester may change it)
int READLINE_READ_SIZE = 512;

static size_t my_strlen(const char *s)
{
	size_t length = 0;
	if (s == NULL)
		return 0;
	while (s[length] != '\0')
		length++;
	return length;
}

static ssize_t find_newline_position(const char *s)
{
	ssize_t index = 0;
	if (s == NULL)
		return -1;
	while (s[index] != '\0') {
		if (s[index] == '\n')
			return index;
		index++;
	}
	return -1;
}

static char *copy_range(const char *src, size_t start, size_t len)
{
	char *dst = (char *)malloc(len + 1);
	size_t i = 0;
	if (dst == NULL)
		return NULL;
	while (i < len) {
		dst[i] = src[start + i];
		i++;
	}
	dst[len] = '\0';
	return dst;
}

static int append_to_storage(const char *buf, size_t buf_len)
{
	size_t old_len = my_strlen(g_storage);
	char *joined = (char *)malloc(old_len + buf_len + 1);
	size_t i = 0;
	if (joined == NULL)
		return -1;
	while (i < old_len) {
		joined[i] = g_storage[i];
		i++;
	}
	i = 0;
	while (i < buf_len) {
		joined[old_len + i] = buf[i];
		i++;
	}
	joined[old_len + buf_len] = '\0';
	if (g_storage != NULL)
		free(g_storage);
	g_storage = joined;
	return 0;
}

char *my_readline(int fd)
{
	ssize_t newline_pos;

	if (fd < 0 || READLINE_READ_SIZE <= 0)
		return NULL;

	newline_pos = find_newline_position(g_storage);
	while (newline_pos < 0) {
		char *buffer = (char *)malloc((size_t)READLINE_READ_SIZE);
		ssize_t bytes_read;

		if (buffer == NULL) {
			if (g_storage != NULL) {
				free(g_storage);
				g_storage = NULL;
			}
			return NULL;
		}

		bytes_read = read(fd, buffer, (size_t)READLINE_READ_SIZE);
		if (bytes_read < 0) {
			free(buffer);
			if (g_storage != NULL) {
				free(g_storage);
				g_storage = NULL;
			}
			return NULL;
		}
		if (bytes_read == 0) {
			free(buffer);
			if (g_storage == NULL || g_storage[0] == '\0') {
				if (g_storage != NULL) {
					free(g_storage);
					g_storage = NULL;
				}
				return NULL; // EOF before reading any characters
			}
			// Return leftover as last line (no newline)
			{
				char *line = g_storage;
				g_storage = NULL;
				return line;
			}
		}

		if (append_to_storage(buffer, (size_t)bytes_read) != 0) {
			free(buffer);
			if (g_storage != NULL) {
				free(g_storage);
				g_storage = NULL;
			}
			return NULL;
		}
		free(buffer);
		newline_pos = find_newline_position(g_storage);
	}

	// Extract line up to (but not including) the newline
	{
		char *line = copy_range(g_storage, 0, (size_t)newline_pos);
		if (line == NULL) {
			free(g_storage);
			g_storage = NULL;
			return NULL;
		}

		// Compute remainder after newline
		{
			size_t total_len = my_strlen(g_storage);
			size_t remain_len = total_len - ((size_t)newline_pos + 1);
			if (remain_len == 0) {
				free(g_storage);
				g_storage = NULL;
			} else {
				char *remainder = copy_range(g_storage, (size_t)newline_pos + 1, remain_len);
				free(g_storage);
				if (remainder == NULL) {
					free(line);
					g_storage = NULL;
					return NULL;
				}
				g_storage = remainder;
			}
			return line;
		}
	}
}

void init_my_readline(void)
{
	if (g_storage != NULL) {
		free(g_storage);
		g_storage = NULL;
	}
}