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

#include "client_serveur.h"

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

	//int socket = createClient("7000", "127.0.0.1");

	Mat image;
	VideoCapture camera(0);

	//Varialbes pour test
	Mat tab[2];// images stocker par la caméra
	Mat gray, gray2;//images en niveau de gris
	Mat diff, diff2;
	//camera.read(tab[1]);
	//camera.read(tab[2]);

	if (camera.isOpened()) {
		while (true) {
			if (camera.read(image)) {

				printf("Lecture caméra.\n");

				camera.read(tab[0]);
				camera.read(tab[1]);
				cvtColor(tab[0], gray, COLOR_BGR2GRAY);
				cvtColor(tab[1], gray2, COLOR_BGR2GRAY);

				diff = compare_contours(gray, gray2);
				//diff2 = compare_pix(gray, gray2);

				//unsigned char* data_char = tab[1].data;

				printf("Pourcentage différences : %lf\n", norm(diff, NORM_L2, noArray()));//100-(countNonZero(diff)*100 /  (diff.cols*diff.rows)
				namedWindow("Test", WINDOW_AUTOSIZE);
				imshow("Test", diff);

			}

			waitKey(20);
		}

		camera.release();
	}
	else
		printf("Lecture camera impossible.\n");

	return (0);
}




