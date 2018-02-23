/*
 * functions.hpp
 *
 *  Created on: 11 févr. 2018
 *      Author: pi
 */

#ifndef SRC_FUNCTIONS_HPP_
#define SRC_FUNCTIONS_HPP_

#include <stdio.h>
#include <iostream>
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
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <vector>
#include <sys/time.h>

#define SIZE_OF_TIME_STRING 20

struct captureConf
{
	int sockfd;
	int newsockfd;
	int portno;
	unsigned int width = 0;
	unsigned int height = 0;
	struct sockaddr_in cli_addr;
	struct sockaddr_in serv_addr;
	socklen_t clilen;


	//Gestion de l'image
	time_t temps;

	bool first_rec = true;
	int image_id = 0;
	char lastDir[SIZE_OF_TIME_STRING+4];

};

using namespace cv;
using namespace std;


void create_socket(int argc, char *argv[], captureConf*);

int sock_accept(int sock);

void requests(captureConf*);

void getImageParameters(captureConf*);

void receiveImage(captureConf*);

void error(const char *msg);


#endif /* SRC_FUNCTIONS_HPP_ */
