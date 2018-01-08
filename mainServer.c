#include "client_server.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s serverPort", argv[0]);
        exit(EXIT_FAILURE);
    }

    return receiveImage(argv[1]);
}
