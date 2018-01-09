#include "client_server.h"

int receiveImage(const char *destRep, const char *serverPortString, const char *address) {
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

    const socklen_t socklen = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client_addr;
    if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, (socklen_t *) &socklen)) == -1) {
        fprintf(stderr, "Erreur accept\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char buffer[BUFSIZ];
    recv(client_socket, buffer, BUFSIZ, 0);

    FILE *received_file = NULL;
    const time_t currentTime = time(NULL);
    char currentTimeString[SIZE_OF_TIME_STRING];
    strftime(currentTimeString, SIZE_OF_TIME_STRING, "%d-%m-%Y_%H-%M-%S", localtime(&currentTime));
    char *imageName = malloc((strlen(destRep) + strlen(currentTimeString) + 8) * sizeof(char));
    strcpy(imageName, destRep);
    strcat(imageName, "/");
    strcat(imageName, currentTimeString);
    if ((received_file = fopen(strcat(imageName, ".jpg"), "w+")) == NULL) {
        fprintf(stderr, "Erreur ouverture image %s\n%s\n", imageName, strerror(errno));
        return EXIT_FAILURE;
    }
    free(imageName);

    ssize_t len;
    long remain_data = strtol(buffer, NULL, 10);
    while (((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)) {
        fwrite(buffer, sizeof(char), (size_t) len, received_file);
        remain_data -= len;
    }

    fclose(received_file);
    close(client_socket);
    close(server_socket);

    return 0;
}
