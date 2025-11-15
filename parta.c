#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];
    
    if (argc != 2) {
        fprintf(stderr, "USAGE: parta <FILE>\n");
        return 1;
    }

    // Open the input file for reading
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Counters
    unsigned long upper = 0;
    unsigned long lower = 0;
    unsigned long number = 0;
    unsigned long space = 0;
    unsigned long other = 0;

    // Read loop
    ssize_t nRead;
    while ((nRead = read(fd, buffer, BUFSIZE)) > 0) {
        for (ssize_t i = 0; i < nRead; ++i) {
            unsigned char c = (unsigned char)buffer[i];
            if (isupper(c)) {
                upper++;
            } else if (islower(c)) {
                lower++;
            } else if (isdigit(c)) {
                number++;
            } else if (isspace(c)) {
                space++;
            } else {
                other++;
            }
        }
    }

    if (nRead < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    close(fd);

    // Output
    printf("Upper,%lu\n", upper);
    printf("Lower,%lu\n", lower);
    printf("Number,%lu\n", number);
    printf("Space,%lu\n", space);
    printf("Others,%lu\n", other);

    return 0;
}
