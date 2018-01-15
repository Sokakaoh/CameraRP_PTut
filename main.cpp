/*
 * main.cpp
 *
 *  Created on: 10 Jan 2018
 *      Author: pi
 */

/*
 * main.cpp
 *
 *  Created on: 27 déc. 2017
 *      Author: root
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

//#include "client_server.h"

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

//#define SIZE_OF_TIME_STRING 20

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
int sendImage(long size, const unsigned char *imageArray, int client_socket);

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

int createClient(const char *serverPortString, const char *address) {
    struct sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &(remote_addr.sin_addr));
    remote_addr.sin_port = htons((uint16_t) strtol(serverPortString, NULL, 10));

    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Erreur création socket\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *) &remote_addr, sizeof(struct sockaddr)) == -1) {
        close(client_socket);
        fprintf(stderr, "Erreur connect\n%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

int sendImage(const long size, const unsigned char *imageArray, const int client_socket) {
    unsigned char *sizeString = (unsigned char*) malloc(sizeof(int));
    sprintf((char *)sizeString, "%li", size);

    if (send(client_socket, sizeString, strlen((char *)sizeString), NULL) < 0) {
        free(sizeString);
        fprintf(stderr, "Erreur envoi taille\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    free(sizeString);

    for (int i = 0; i < size; i += BUFSIZ)
    {
        if (send(client_socket, &imageArray[i], BUFSIZ, NULL) < 0) {
            fprintf(stderr, "Erreur envoi tableau\n%s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}


using namespace cv;
using namespace std;

Mat src;
Mat src_gray;
Mat src2;
Mat src2_gray;
int thresh = 30;

Mat compare_pix(Mat image, Mat image2)
{
	Mat diff;
	compare(image, image2, diff, cv::CMP_EQ);

	return diff;
}

Mat compare_contours(Mat image, Mat image2)
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( image, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC1 );
  Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC1 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( 255, 255, 255 );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Detect edges using canny
  Canny( image2, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( 255, 255, 255 );
       drawContours( drawing2, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  Mat diff;
  compare(drawing, drawing2, diff, cv::CMP_EQ);

  //printf("Pourcentage différences : %d\n", 100-(countNonZero(diff)*100 / (diff.cols*diff.rows)) );
  //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  //imshow( "Contours", diff );

  return diff;
}

int main(int, char** argv) {

	printf("Lancement programme.\n");

	int socket_envoi = createClient("7000", "127.0.0.1");

	Mat image;
	VideoCapture camera(0);
	camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	//Varialbes pour test
	Mat tab[2];// images stocker par la caméra
	Mat gray, gray2;//images en niveau de gris
	Mat diff, diff2;

	if (camera.isOpened()) {
		while (true) {
			if (camera.read(tab[0])) {

				printf("Lecture caméra.\n");

				//camera.read(tab[0]);
				camera.read(tab[1]);
				cvtColor(tab[0], gray, COLOR_BGR2GRAY);
				cvtColor(tab[1], gray2, COLOR_BGR2GRAY);

				diff = compare_contours(gray, gray2);
				//diff2 = compare_pix(gray, gray2);

				//printf("taille supposé : %d\n", tab[0].cols * tab[0].rows * 3);
				unsigned char* img = tab[0].data;

				if( (100-(countNonZero(diff)*100 /  (diff.cols*diff.rows))) > 4)
				{
					sendImage(tab[0].cols * tab[0].rows * 3, img, socket_envoi);
					printf("Envoi image..\n");
				}



				printf("Pourcentage différences : %d\n", 100-(countNonZero(diff)*100 /  (diff.cols*diff.rows) ));//norm(diff, NORM_L2, noArray()

				//namedWindow("Test", WINDOW_AUTOSIZE);
				//imshow("Test", diff);

			}

			//waitKey(100);
		}

		camera.release();
	}
	else
		printf("Lecture camera impossible.\n");

	return (0);
}




