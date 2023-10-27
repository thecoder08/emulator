#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "loadfile.h"

int loadFile(char* filename, char* buffer) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    struct stat st;
    fstat(fd, &st);
    read(fd, buffer, st.st_size);
    close(fd);
    return 0;
}

int saveFile(char* filename, char* buffer, int size) {
    int fd = open(filename, O_CREAT | O_WRONLY, 0664);
    if (fd == -1) {
        return 1;
    }
    write(fd, buffer, size);
    close(fd);
    return 0;
}