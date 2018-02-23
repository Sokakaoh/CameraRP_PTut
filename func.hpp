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
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>

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
	unsigned int width = DEFAULT_WIDTH;
	unsigned int height = DEFAULT_HEIGHT;
	int sock;
	int comp_method = 3;// par défaut comparaison par pixels

};

/**
 * Structure pour la méthode BGS (Background Substraction)
 */

struct Conf_bgs
{
	Mat maskMOG2;
	Ptr<BackgroundSubtractorMOG2> MOG2;
};

void error(const char *msg);

Mat compare_contours(Mat image, Mat image2);

Conf* menu(Conf*);

void setSize(Conf*);

void createSocket(int argc, char *argv[], Conf*);

void sendParameters(Conf* setRP);

void sendInit(Conf* setRP);

void apply_bgs(Conf_bgs*);

void compChoice(Conf*);


#endif /* SRC_FUNC_HPP_ */
