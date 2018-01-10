#include "client_server.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s serverPort serverAddress", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_socket = createClient(argv[1], argv[2]);

    FILE *image = fopen("assets/src/imageTest.jpg", "r");

    fseek(image, 0L, SEEK_END);
    size_t imageSize = (size_t) ftell(image);
    rewind(image);

    char *buffer = calloc(1, imageSize + 1);
    fread(buffer, imageSize, 1, image);
    fclose(image);

    int retour = sendImage((int) imageSize, buffer, client_socket);

    free(buffer);
    close(client_socket);
    return retour;
}
