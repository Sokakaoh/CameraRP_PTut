#include "client_server.h"

int sendImage(const char *fileName, const char *serverPortString, const char *address) {
    struct sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(remote_addr.sin_addr));
    remote_addr.sin_port = htons((uint16_t) strtol(serverPortString, NULL, 10));

    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Erreur création socket\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (connect(client_socket, (struct sockaddr *) &remote_addr, sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Erreur connect\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    int image;
    if ((image = open(fileName, O_RDONLY)) == -1) {
        fprintf(stderr, "Erreur ouverture image\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct stat file_stat;
    if (fstat(image, &file_stat) < 0) {
        fprintf(stderr, "Erreur fstat\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char file_size[256];
    sprintf(file_size, "%li", file_stat.st_size);
    if (send(client_socket, file_size, sizeof(file_size), 0) < 0) {
        fprintf(stderr, "Erreur envoi taille\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    off_t offset = 0;
    off_t remain_data = file_stat.st_size;
    ssize_t sent_bytes;
    while (((sent_bytes = sendfile(client_socket, image, &offset, BUFSIZ)) > 0) && (remain_data > 0))
        remain_data -= sent_bytes;

    close(client_socket);

    return EXIT_SUCCESS;
}
