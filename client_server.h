#ifndef ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H
#define ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MIN_PORT 1
#define MAX_PORT 65535
#define MAX_BUFFER 1024

/**
 * Envoi une image à un serveur
 * @param fileName chemin de l'image
 * @param serverPortString port du serveur
 * @param address addresse du serveur
 * @return EXIT_SUCCESS si l'envoi a réussi ; EXIT_FAILURE s'il a échoué
 */
int sendImage(const char *fileName, const char *serverPortString, const char *address);

/**
 * Reçoit une image
 * @param serverPortString port du serveur
 * @return EXIT_SUCCESS si la réception a réussi ; EXIT_FAILURE si elle a échoué
 */
int receiveImage(const char *serverPortString);

#endif //ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H
