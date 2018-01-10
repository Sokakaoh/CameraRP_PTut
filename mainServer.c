#include "client_server.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage : %s destRep serverPort serverAddress", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server = createServer(argv[2], argv[3]);
    while (1)
        receiveImage(argv[1], server);
}
