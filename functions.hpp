/*
 * functions.hpp
 *
 *  Created on: 11 févr. 2018
 *      Author: pi
 */

#ifndef SRC_FUNCTIONS_HPP_
#define SRC_FUNCTIONS_HPP_

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
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <vector>

#define SIZE_OF_TIME_STRING 20

using namespace cv;
using namespace std;

void create_socket(int argc, char *argv[], int tab_socket[]);

int sock_accept(int sock);

void error(const char *msg);



#endif /* SRC_FUNCTIONS_HPP_ */
