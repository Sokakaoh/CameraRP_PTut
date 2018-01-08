#include "client_server.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage : %s fileName serverPort serverAddress", argv[0]);
        exit(EXIT_FAILURE);
    }

    return sendImage(argv[1], argv[2], argv[3]);
}
