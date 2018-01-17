/*
 * serveur.c
 *
 *  Created on: 10 janv. 2018
 *      Author: pi
 */
#include "client_server.h"

int createServer(const char *serverPortString, const char *address) {
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Erreur création socket\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(server_addr.sin_addr));
    server_addr.sin_port = htons((uint16_t) strtol(serverPortString, NULL, 10));

    if ((bind(server_socket, (struct sockaddr *) &server_addr, sizeof(struct sockaddr))) == -1) {
        close(server_socket);
        fprintf(stderr, "Erreur bind\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((listen(server_socket, 5)) == -1) {
        close(server_socket);
        fprintf(stderr, "Erreur listen\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

int acceptClient(const int server_socket) {
    const socklen_t socklen = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client_addr;
    if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, (socklen_t *) &socklen)) == -1) {
        fprintf(stderr, "Erreur accept\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return client_socket;
}

int receiveImage(const char *destRep, const int client_socket) {
    char end[4]
    while (recv(client_socket, end, 4, 0) > 0 && end == "end");
    char imageLength[BUFSIZ];
    recv(client_socket, imageLength, BUFSIZ, 0;

    const time_t currentTime = time(NULL);
    char currentTimeString[SIZE_OF_TIME_STRING];
    strftime(currentTimeString, SIZE_OF_TIME_STRING, "%d-%m-%Y_%H-%M-%S", localtime(&currentTime));
    char *imageName = malloc((strlen(destRep) + strlen(currentTimeString) + 8) * sizeof(char));
    strcpy(imageName, destRep);
    strcat(imageName, "/");
    strcat(imageName, currentTimeString);
    strcat(imageName, ".jpg");

    ssize_t len;
    long remain_data = strtol(imageLength, NULL, 10);
    unsigned char buffer[BUFSIZ];

    //Ecriture image
    const int WIDTH = 640, HEIGHT = 480;
    IplImage *retour = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    char *p = retour->imageData;

    int i = 0;
    while (((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)) {
        if (i < WIDTH * HEIGHT * 3) {
            ssize_t longueur = remain_data > len ? len : remain_data;
            for (int j = 0; j < longueur; j++) {
                *p = buffer[j];
                ++p;
                ++i;
            }
        }
        remain_data -= len;
    }

    cvSaveImage(imageName, retour, NULL);
    free(imageName);
    cvDestroyAllWindows();
    cvReleaseImage(&retour);
    return EXIT_SUCCESS;
}
