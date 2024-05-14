#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring> // for strlen
using namespace std;

int main(int argc, char** argv) {
    const char* myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    int fd = open(myfifo, O_WRONLY);
    if (fd == -1) {
        cerr << "Error opening FIFO" << endl;
        return 1;
    }

    const char* message = argv[1];
    ssize_t bytes_written = write(fd, message, strlen(message) + 1);
    if (bytes_written == -1) {
        cerr << "Error writing to FIFO" << endl;
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}



