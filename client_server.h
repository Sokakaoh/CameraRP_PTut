#ifndef ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H
#define ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**
 * Envoi une image à un serveur
 * @param fileName chemin de l'image
 * @param serverPortString port du serveur
 * @param address adresse du serveur
 * @return EXIT_SUCCESS si l'envoi a réussi ; EXIT_FAILURE s'il a échoué
 */
int sendImage(const char *fileName, const char *serverPortString, const char *address);

/**
 * Reçoit une image
 * @param serverPortString port du serveur
 * @param address adresse du serveur
 * @return EXIT_SUCCESS si la réception a réussi ; EXIT_FAILURE si elle a échoué
 */
int receiveImage(const char *serverPortString, const char *address);

#endif //ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H
