#include "client_server.h"

int sendImage(const char *fileName, const char *serverPortString, const char *address) {
    const int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        close(sock);
        fprintf(stderr, "Erreur d'ouverture de la socket.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in sock_s;
    sock_s.sin_family = AF_INET;
    sock_s.sin_addr.s_addr = inet_addr(address);
    sock_s.sin_port = htons((const uint16_t) strtol(serverPortString, NULL, 10));
    if (sock_s.sin_port < MIN_PORT || sock_s.sin_port > MAX_PORT) {
        close(sock);
        fprintf(stderr, "Le port doit être compris entre %d et %d.\n", MIN_PORT, MAX_PORT);
        return EXIT_FAILURE;
    }

    FILE *image = fopen(fileName, "r");
    if (fseek(image, 0, SEEK_END) != 0) {
        close(sock);
        fclose(image);
        fprintf(stderr, "Erreur d'ouverture de l'image.\n");
        return EXIT_FAILURE;
    }

    size_t imageLength = (size_t) ftell(image);
    rewind(image);
    char *buffer = malloc(imageLength * sizeof(char));
    if (!fread(buffer, imageLength, 1, image)) {
        free(buffer);
        close(sock);
        fclose(image);
        fprintf(stderr, "Erreur création du buffer.\n");
        return EXIT_FAILURE;
    }
    fclose(image);

    if (sendto(sock, &imageLength, sizeof(size_t), 0, (const struct sockaddr *) &sock_s, sizeof(sock_s)) == -1) {
        free(buffer);
        close(sock);
        fprintf(stderr, "Erreur d'envoi de la taille.\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i <= imageLength; i += MAX_BUFFER) {
        char *subbuff = malloc(MAX_BUFFER * sizeof(char));
        memcpy(subbuff, buffer, i);
        if (sendto(sock, subbuff, MAX_BUFFER, 0, (const struct sockaddr *) &sock_s, sizeof(sock_s)) == -1) {
            free(subbuff);
            free(buffer);
            close(sock);
            fprintf(stderr, "Erreur d'envoi.\n");
            return EXIT_FAILURE;
        }
        /*
         * Problème :
         *     – « double free or corruption » si « free(subbuff) » non commenté ;
         *     – « Process finished with exit code 139 (interrupted by signal 11: SIGSEGV) » si « free(subbuff) » commenté
         */
        free(subbuff);
    }

    free(buffer);
    close(sock);
    return EXIT_SUCCESS;
}
