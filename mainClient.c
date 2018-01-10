#include "client_server.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s serverPort serverAddress", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_socket = createClient(argv[1], argv[2]);

    char *filename = "assets/src/imageTest.jpg";
    FILE *image = fopen(filename, "r");
    struct stat imageSize;
    stat(filename, &imageSize);
    char *buffer = malloc(imageSize.st_size * sizeof(char));
    fread(buffer, (size_t) imageSize.st_size, sizeof(char), image);
    fclose(image);

    int retour = sendImage(imageSize.st_size, buffer, client_socket);

    free(buffer);
    close(client_socket);
    return retour;
}
