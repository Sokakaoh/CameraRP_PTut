/*
 * functions.cpp
 *
 *  Created on: 11 févr. 2018
 *      Author: pi
 */

#include "functions.hpp"

void create_socket(int argc, char *argv[], int tab_socket[2] )
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;

    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr,"Aucun port spécifié.\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       error("Erreur ouverture socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("Erreur liaison de la socket");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (newsockfd < 0)
         error("Erreur sur accept()");

    tab_socket[0] = sockfd;
    tab_socket[1] = newsockfd;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
