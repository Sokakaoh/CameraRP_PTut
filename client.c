#include "client_server.h"

int createClient(const char *serverPortString, const char *address) {
    struct sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(remote_addr.sin_addr));
    remote_addr.sin_port = htons((uint16_t) strtol(serverPortString, NULL, 10));

    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Erreur création socket\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *) &remote_addr, sizeof(struct sockaddr)) == -1) {
        close(client_socket);
        fprintf(stderr, "Erreur connect\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

int sendImage(const long size, const char *imageArray, const int client_socket) {
    char *sizeString = malloc(sizeof(int));
    sprintf(sizeString, "%li", size);
    if (send(client_socket, sizeString, sizeof(size), NULL) < 0) {
        free(sizeString);
        fprintf(stderr, "Erreur envoi taille\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    free(sizeString);

    int index = 0;
    while (index < size)
        if (send(client_socket, imageArray + (index += BUFSIZ), BUFSIZ, NULL) < 0) {
            fprintf(stderr, "Erreur envoi tableau\n%s\n", strerror(errno));
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}
