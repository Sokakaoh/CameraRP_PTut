/*
 * main_cpp.cpp
 *
 *  Created on: 7 févr. 2018
 *      Author: root
 */




#include "func.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>

bool continuer = true;

void sigHandler(int signal)
{
	continuer = false;
}

int main(int argc, char *argv[])
{
	Conf_bgs setBGS;
	setBGS.MOG2 = createBackgroundSubtractorMOG2();


	//Menu de paramétrage
	Conf setRP;
	menu(&setRP);

	//Préparation envoi images
	createSocket(argc, argv, &setRP);
	sendParameters(&setRP);

	//sendInit(&setRP);

	signal(SIGINT, sigHandler);

    Mat capture[2];
    int bytes;
    int img_req = 2;

	Mat gray, gray2;//images en niveau de gris
	Mat diff; //Retour des différentes méthodes de comparaison
	bool tauxDiff = false;
    VideoCapture video(0);

    video.set(CV_CAP_PROP_FRAME_HEIGHT, setRP.height);
    video.set(CV_CAP_PROP_FRAME_WIDTH, setRP.width);

    if( video.isOpened() )
    {

    	//int flags = fcntl(0, F_GETFL, 0);
    	//fcntl(0, F_SETFL, flags | O_NONBLOCK);

    	//Capture de deux images pour initialiser comparaison
		video.read(capture[0]);
		video.read(capture[1]);

		cvtColor(capture[0], gray, COLOR_BGR2GRAY);
		cvtColor(capture[1], gray2, COLOR_BGR2GRAY);

		//switch pour configurer la pré-capture
		//retour avec comp et diff

    	while(continuer) {


    		switch(setRP.comp_method)
    		{
    			case 1 :
    				compare(gray, gray2, diff, CMP_EQ);
    				tauxDiff = 100 - (countNonZero(diff)*100 /  diff.total()) > 25;
    				break;
    			case 2 :
    				diff = compare_contours(gray, gray2);
    				tauxDiff = 100 - (countNonZero(diff)*100 /  diff.total()) > 4;
    				break;
    			case 3 :
    				setBGS.MOG2.get()->apply(capture[0], diff);
    				tauxDiff = 100 - ( (diff.total()*diff.elemSize() - countNonZero(diff))*100 /  diff.total()*diff.elemSize()) > 4;
    				break;

    		}

    		cout << 100 - ( (diff.total()*diff.elemSize() - countNonZero(diff))*100 /  diff.total()*diff.elemSize()) <<endl;
			if(tauxDiff)
			{
				if( send(setRP.sock, &img_req, sizeof(int), 0) < 0)
					error("Erreur envoi numéro requête image.");
				else
				{
					printf("Envoi image.\n");
					bytes = send(setRP.sock, capture[0].data, capture[0].total()*capture[0].elemSize(), 0);

					if (bytes < 0)
	    	         error("ERREUR d'écriture sur la socket");
				}
			}

			capture[0].release();
			gray.release();

			capture[0] = capture[1].clone();
			gray = gray2.clone();

			video.read(capture[1]);

			cvtColor(capture[1], gray2, COLOR_BGR2GRAY);

    	}
    }

    int end = -1;
	if( send(setRP.sock, &end, sizeof(int), 0) < 0)
		error("Erreur envoi signal d'arrêt.");

    video.release();
    close(setRP.sock);
    cout << "socket Fermée" << endl;

    return 0;

}

