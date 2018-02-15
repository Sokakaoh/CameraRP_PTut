/*
 * func.hpp
 *
 *  Created on: 15 févr. 2018
 *      Author: pi
 */

#ifndef SRC_FUNC_HPP_
#define SRC_FUNC_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

using namespace cv;
using namespace std;



/**
 *Structure de configuration de la connexion
 *Variables :
 *	width : largeur de l'image
 *	height : hauteur de l'image
 *	sock : soocket client d'envoi
 */

struct Conf
{
	unsigned int width;
	unsigned int height;
	int sock;

};


void error(const char *msg);

Mat compare_contours(Mat image, Mat image2);

Conf* menu(Conf*);

void setSize(Conf*);

void createSocket(int argc, char *argv[], Conf*);


#endif /* SRC_FUNC_HPP_ */
