#include "client_server.h"

int receiveImage(const char *serverPortString, const char *address) {
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Erreur création socket\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(server_addr.sin_addr));
    server_addr.sin_port = htons((uint16_t) strtol(serverPortString, NULL, 10));

    if ((bind(server_socket, (struct sockaddr *) &server_addr, sizeof(struct sockaddr))) == -1) {
        fprintf(stderr, "Erreur bind\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if ((listen(server_socket, 5)) == -1) {
        fprintf(stderr, "Erreur listen\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char buffer[BUFSIZ];
    while (recv(server_socket, buffer, BUFSIZ, 0) == -1);

    FILE *received_file = NULL;
    time_t currentTime = time(NULL);
    char *currentTimeString = ctime(&currentTime);
    char *imageName = malloc((strlen(currentTimeString) + 4) * sizeof(char));
    strcat(imageName, currentTimeString);
    if ((received_file = fopen(strcat(imageName, ".jpg"), "w")) == NULL) {
        fprintf(stderr, "Erreur ouverture image\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    ssize_t len;
    long remain_data = strtol(buffer, NULL, 10);
    while (((len = recv(server_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)) {
        fwrite(buffer, sizeof(char), (size_t) len, received_file);
        remain_data -= len;
    }

    fclose(received_file);
    close(server_socket);

    return 0;
}
