#include "client_server.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s serverPort serverAddress", argv[0]);
        exit(EXIT_FAILURE);
    }

    return receiveImage(argv[1], argv[2]);
}
