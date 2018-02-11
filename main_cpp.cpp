/*
 * main_cpp.cpp
 *
 *  Created on: 7 févr. 2018
 *      Author: root
 */


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

using namespace cv;
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

Mat compare_contours(Mat image, Mat image2)
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  Mat src;
  Mat src_gray;
  Mat src2;
  Mat src2_gray;
  int thresh = 30;

  /// Detect edges using canny
  Canny( image, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC1 );
  Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC1 );
  for( unsigned int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( 255, 255, 255 );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Detect edges using canny
  Canny( image2, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  for( unsigned int i = 0; i< contours.size(); i++ )
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

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    //server = atoi(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");


    Mat image[2];
    int bytes;

    Mat comp;
	Mat gray, gray2;//images en niveau de gris
	Mat diff, diff2;
    VideoCapture video(0);

    video.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    video.set(CV_CAP_PROP_FRAME_WIDTH, 640);

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
	    	    bytes = send(sockfd, image[0].data, image[0].total()*image[0].elemSize(), 0);
	    	    bytes = send(sockfd, image[1].data, image[1].total()*image[1].elemSize(), 0);
	    	    if (bytes < 0)
	    	         error("ERROR writing to socket");
			}

    	}
    }

    video.release();
    close(sockfd);

    waitKey(0);
    return 0;

}

