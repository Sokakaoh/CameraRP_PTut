/*
 * serveur_cpp.cpp
 *
 *  Created on: 8 févr. 2018
 *      Author: pi
 */



#include "functions.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>


bool co_accept = true;
/*
void sigHandler(int signal)
{
	co_accept = false;
}*/


int main(int argc, char *argv[])
{

	 captureConf setCap;

	 //signal(SIGINT, sigHandler);

     if(! opendir("dest") )
     {
    	 umask(0); //définir 000 à umask avant de créer dossier
    	 const int dest = mkdir("dest", S_IRWXU | S_IRWXG | S_IRWXO);

    	 if(dest == -1)
    		 error("Erreur création dir ");

     }

     create_socket(argc, argv, &setCap);

     //int flags = fcntl(setCap.newsockfd, F_GETFL, 0);

     while(co_accept)
     {
    	 //fcntl(setCap.newsockfd, F_SETFL, flags | O_NONBLOCK);
         setCap.newsockfd = accept(setCap.sockfd,
                     (struct sockaddr *) &setCap.cli_addr,
                     &setCap.clilen);

         if (setCap.newsockfd < 0)
              error("Erreur sur accept()");
         else
        	 requests(&setCap);

     }


     close(setCap.sockfd);
     close(setCap.newsockfd);

     return 0;
}

