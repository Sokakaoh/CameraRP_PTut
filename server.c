#include "client_server.h"

int receiveImage(const char *serverPortString) {
    const int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        close(sock);
        fprintf(stderr, "Erreur d'ouverture de la socket.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in serverSocket;
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons((uint16_t) strtol(serverPortString, NULL, 10));
    if (serverSocket.sin_port < MIN_PORT || serverSocket.sin_port > MAX_PORT) {
        close(sock);
        fprintf(stderr, "Le port doit être compris entre %d et %d.\n", MIN_PORT, MAX_PORT);
        return EXIT_FAILURE;
    }
    serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (const struct sockaddr *) &serverSocket, sizeof(serverSocket)) == -1) {
        close(sock);
        fprintf(stderr, "Erreur bind()\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in clientSocket;
    socklen_t clientSocketLength = sizeof(clientSocket);

    char *imageLengthString = malloc(sizeof(size_t));
    if (recvfrom(sock, imageLengthString, sizeof(size_t), 0, (struct sockaddr *) &clientSocket, &clientSocketLength) !=
        -1) {
        free(imageLengthString);
        close(sock);
        fprintf(stderr, "Erreur réception taille image.\n");
        return EXIT_FAILURE;
    }

    long imageLength = strtol(imageLengthString, NULL, 10);
    char *buffer = malloc(imageLength * sizeof(char));
    for (size_t i = 0; i <= imageLength; i++) {
        char *subbuff = malloc(MAX_BUFFER * sizeof(char));
        if (recvfrom(sock, subbuff, MAX_BUFFER, 0, (struct sockaddr *) &clientSocket, &clientSocketLength)) {
            free(subbuff);
            free(buffer);
            free(imageLengthString);
            close(sock);
            fprintf(stderr, "Erreur de réception.\n");
            return EXIT_FAILURE;
        }
        strcat(buffer, subbuff);
        free(subbuff);
    }

    FILE *image = NULL;
    time_t currentTime = time(NULL);
    char *currentTimeString = ctime(&currentTime);
    char *imageName = malloc((strlen(currentTimeString) + 4) * sizeof(char));
    strcat(imageName, currentTimeString);
    image = fopen(strcat(imageName, ".jpg"), "w");
    free(imageName);
    if (image == NULL) {
        fclose(image);
        free(buffer);
        free(imageLengthString);
        close(sock);
        fprintf(stderr, "Erreur création image.\n");
        return EXIT_FAILURE;
    }
    fprintf(image, buffer);
    fclose(image);

    free(buffer);
    free(imageLengthString);
    close(sock);
    return EXIT_SUCCESS;
}
