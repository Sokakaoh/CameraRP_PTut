/*
 * main_cpp.cpp
 *
 *  Created on: 7 févr. 2018
 *      Author: root
 */




#include "func.hpp"


int main(int argc, char *argv[])
{
	Conf setRP;
	setRP.width = DEFAULT_WIDTH;
	setRP.height = DEFAULT_HEIGHT;

	menu(&setRP);


	createSocket(argc, argv, &setRP);

    Mat image[2];
    int bytes;

    Mat comp;
	Mat gray, gray2;//images en niveau de gris
	Mat diff, diff2;
    VideoCapture video(0);

    video.set(CV_CAP_PROP_FRAME_HEIGHT, setRP.height);
    video.set(CV_CAP_PROP_FRAME_WIDTH, setRP.width);

    if( video.isOpened() )
    {

    	while(true) {

    		video.read(image[0]);
    		video.read(image[1]);

			cvtColor(image[0], gray, COLOR_BGR2GRAY);
			cvtColor(image[1], gray2, COLOR_BGR2GRAY);

			diff = compare_contours(gray, gray2);

			if( (100-(countNonZero(diff)*100 /  diff.total())) > 4)
			{
	    		printf("Envoi image.\n");
	    	    bytes = send(setRP.sock, image[0].data, image[0].total()*image[0].elemSize(), 0);
	    	    bytes = send(setRP.sock, image[1].data, image[1].total()*image[1].elemSize(), 0);
	    	    if (bytes < 0)
	    	         error("ERREUR d'écriture sur la socket");
			}

    	}
    }

    video.release();
    close(setRP.sock);

    waitKey(0);
    return 0;

}

