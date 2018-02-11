/*
 * serveur_cpp.cpp
 *
 *  Created on: 8 févr. 2018
 *      Author: pi
 */



#include "functions.hpp"

#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[])
{

	//Déclarations variables

     int image_id = 0; // compteur pour différencier plusieurs images dans la même seconde
     time_t currentTime;
     char currentTimeString[SIZE_OF_TIME_STRING];
     char subDir_name[SIZE_OF_TIME_STRING+4];
     char ext_nom[4]; //char pour le compteur
     int sock[2];
     time_t temps = time(NULL);
     bool first_rec = true;

     if(! opendir("dest") )
     {
    	 umask(0); //définir 000 à umask avant de créer dossier
    	 const int dest = mkdir("dest", S_IRWXU | S_IRWXG | S_IRWXO);

    	 if(dest == -1)
    		 error("Erreur création dir ");

     }

     create_socket(argc, argv, sock);


     Mat  img = Mat::zeros( 480, 640 , CV_8UC3);
        int  imgSize = img.total()*img.elemSize();
        uchar sockData[imgSize];
        int bytes;

     while(true) {

      //Réception des images

        for (int i = 0; i < imgSize; i += bytes) {
        if ((bytes = recv(sock[1], sockData +i, imgSize  - i, 0)) == -1) {
          //quit("recv failed", 1);
        	exit(0);
         }
        }


        currentTime = time(NULL);
        strftime(currentTimeString, SIZE_OF_TIME_STRING, "%d-%m-%Y_%H-%M-%S", localtime(&currentTime));

        //Création du répertoire
        if( time(NULL) - temps > 60 || first_rec)
        {
    		currentTime = time(NULL);
    		temps = currentTime;
    		//strftime(subDir_name, SIZE_OF_TIME_STRING, "dest/%d-%m-%Y_%H-%M", localtime(&currentTime));
    		strcpy(subDir_name, "dest/");
    		strcat(subDir_name, currentTimeString);

    		umask(0);
    		const int dir = mkdir(subDir_name, S_IRWXU | S_IRWXG | S_IRWXO);
    		first_rec = false;

        }


        char *imageName = (char *) malloc((strlen("dest/") + strlen(subDir_name) + strlen(currentTimeString) + strlen("/") + strlen(ext_nom) + strlen(".jpg")) * sizeof(char));

        sprintf(ext_nom, "-%d", image_id);
        strcpy(imageName, subDir_name);
        strcat(imageName, "/");
        strcat(imageName, currentTimeString);
        strcat(imageName, ext_nom);
        strcat(imageName, ".jpg");

        if(image_id < 30)
        	image_id++;
        else
        	image_id = 0;


       Mat img2(Size(640, 480), CV_8UC3, sockData);

       printf("Ecriture image : %s\n", imageName);

       imwrite(imageName, img2);
       free(imageName);

       //namedWindow( "Server", CV_WINDOW_AUTOSIZE );// Create a window for display.
       //imshow( "Server", img2 );
       //waitKey(500);



      img2.release();


     }

     close(sock[1]);
     close(sock[0]);

     return 0;
}

