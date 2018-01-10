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

#define SIZE_OF_TIME_STRING 20

/**
 * @param serverPortString port du serveur
 * @param address adresse du serveur
 * @return socket du client créé
 */
int createClient(const char *serverPortString, const char *address);

/**
 * Envoi une image à un serveur
 * @param size taille du tableau
 * @param imageArray image en tableau de char
 * @param client_socket socket du client
 * @return EXIT_SUCCESS si l'envoi a réussi ; EXIT_FAILURE s'il a échoué
 */
int sendImage(long size, const char *imageArray, int client_socket);

/**
 * @param serverPortString port du serveur
 * @param address addresse du serveur en IPv4
 * @return socket du serveur créé
 */
int createServer(const char *serverPortString, const char *address);

/**
 * Reçoit une image
 * @param destRep Répertoire de destination
 * @param server_socket socket du serveur
 * @return EXIT_SUCCESS si la réception a réussi ; EXIT_FAILURE si elle a échoué
 */
int receiveImage(const char *destRep, int server_socket);

#endif //ENVOIDIMAGECLIENTSERVEUR_CLIENT_SERVER_H
