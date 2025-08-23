#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int READLINE_READ_SIZE = 64;
char *my_readline(int fd);
void init_my_readline(void);

int main(void) {
    int fd = open("/etc/hosts", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    char *s;
    while ((s = my_readline(fd)) != NULL) {
        printf("%s\n", s);
        free(s);
    }
    close(fd);
    init_my_readline();
    return 0;
}
