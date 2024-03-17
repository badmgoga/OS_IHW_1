#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void function(char a[], char b[], char c[]) {
    int flag = 0;
    int h = 0;
    for (int i = 0; i < strlen(a); i++) {
        for (int j = 0; j < strlen(b); j++) {
            if (a[i] == b[j]) {
                for (int k = 0; k < strlen(c); k++) {
                    if (a[i] == c[k]) {
                        flag = 1;
                        break;
                    }
                }
                if (flag != 1) {
                    c[h++] = a[i];
                }
                flag = 0;
                break;
            }
        }
    }
    c[h++] = '\0';
}

int main(int argc, char* argv[]) {
    char* input1 = argv[1];
    char* input2 = argv[2];
    char* output = argv[3];
    int file1, file2, file3, fd1, fd2, fd3, result, size;
    char buffer1[5000], buffer2[5000], buffer3[5000], buffer4[5000],
        buffer5[5000], buffer6[5000];

    mknod("in1.fifo", S_IFIFO | 0666, 0);
    mknod("in2.fifo", S_IFIFO | 0666, 0);
    mknod("out.fifo", S_IFIFO | 0666, 0);
    result = fork();
    if (result < 0) {
        printf("Can\'t fork child");
        exit(-1);
    }
    else if (result > 0) {
        if ((file1 = open(input1, O_RDONLY)) < 0) {
            printf("Can\'t open file");
            exit(-1);
        }
        size = read(file1, buffer1, 5000);
        if (size < 0) {
            printf("Can\'t read from file");
            exit(-1);
        }
        if (close(file1) < 0) {
            printf("Can\'t close file");
            exit(-1);
        }
        if ((fd1 = open("in1.fifo", O_WRONLY)) < 0) {
            printf("Can\'t open fifo");
            exit(-1);
        }
        size = write(fd1, buffer1, size);
        if (size < 0) {
            printf("Can\'t write to fifo");
            exit(-1);
        }
        if (close(fd1) < 0) {
            printf("Can\'t close fifo");
            exit(-1);
        }

        if ((file2 = open(input2, O_RDONLY)) < 0) {
            printf("Can\'t open file");
            exit(-1);
        }
        size = read(file2, buffer2, 5000);
        if (size < 0) {
            printf("Can\'t read from file");
            exit(-1);
        }
        if (close(file2) < 0) {
            printf("Can\'t close file");
            exit(-1);
        }
        if ((fd2 = open("in2.fifo", O_WRONLY)) < 0) {
            printf("Can\'t open fifo");
            exit(-1);
        }
        size = write(fd2, buffer2, size);
        if (size < 0) {
            printf("Can\'t write to fifo");
            exit(-1);
        }
        if (close(fd2) < 0) {
            printf("Can\'t close fifo");
            exit(-1);
        }

        if ((fd3 = open("out.fifo", O_RDONLY)) < 0) {
            printf("Can\'t open fifo");
            exit(-1);
        }
        size = read(fd3, buffer3, size);
        if (size < 0) {
            printf("Can\'t read from pipe");
            exit(-1);
        }
        if (close(fd3) < 0) {
            printf("Can\'t close pipe");
            exit(-1);
        }
        if ((file3 = open(output, O_WRONLY | O_CREAT, 0666)) < 0) {
            printf("Can\'t open file");
            exit(-1);
        }
        size = write(file3, buffer3, size);
        if (size < 0) {
            printf("Can\'t write to file");
            exit(-1);
        }
        if (close(file3) < 0) {
            printf("Can\'t close file");
            exit(-1);
        }
    }
    else {
        if ((fd1 = open("in1.fifo", O_RDONLY)) < 0) {
            printf("Can\'t open fifo");
            exit(-1);
        }
        size = read(fd1, buffer4, 5000);
        if (size < 0) {
            printf("Can\'t read from fifo");
            exit(-1);
        }
        if (close(fd1) < 0) {
            printf("Can\'t close fifo");
            exit(-1);
        }

        if ((fd2 = open("in2.fifo", O_RDONLY)) < 0) {
            printf("Can\'t open fifo");
            exit(-1);
        }
        size = read(fd2, buffer5, 5000);
        if (size < 0) {
            printf("Can\'t read from fifo");
            exit(-1);
        }
        if (close(fd2) < 0) {
            printf("Can\'t close fifo");
            exit(-1);
        }

        function(buffer4, buffer5, buffer6);

        if ((fd3 = open("out.fifo", O_WRONLY)) < 0) {
            printf("Can\'t open fifo");
            exit(-1);
        }
        size = write(fd3, buffer6, strlen(buffer6));
        if (size < 0) {
            printf("Can\'t write to fifo");
            exit(-1);
        }
        if (close(fd3) < 0) {
            printf("Can\'t close fifo");
            exit(-1);
        }
    }
}
