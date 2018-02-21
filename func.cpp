/*
 * func.cpp
 *
 *  Created on: 15 févr. 2018
 *      Author: pi
 */


#include "func.hpp"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

Conf* menu(Conf* setRP)
{
	int numb = -1;

	while(numb < 1 || numb > 3)
	{
		cout << "Configuration de la capture d'images :" << endl;
		cout << "1. Modification taille image." << endl;
		cout << "2. Démarrer capture." << endl;
		cout << "3. Choix méthode de comparaison." << endl;

		cin.clear();
		cin >> numb;
	}

	switch(numb) {

		case 1:
			setSize(setRP);
			break;
		case 2:
			cout << "Lancement capture ('q' pour quitter)." << endl;
			break;
		case 3:
			compChoice(setRP);
			break;
		default :
			menu(setRP);
			break;
	}

	return setRP;
}

void setSize(Conf* setRP)
{

	//bool valide = false;

	cout << "Configuration de la taille (minimum 200px) :" << endl;

	cout << "Configuration largeur (min : 200px , max : 1920px) : " << endl;
	cout << "Largeur :";
	cin >> setRP->width;

	while(setRP->width < 200 || setRP->width > 1920)
	{
		cout << "Largeur :";
		cin >> setRP->width;
	}

	cout << "Configuration hauteur (min : 200px , max : 1080px) : " << endl;
	cout << "Hauteur :";
	cin >> setRP->height;

	while(setRP->height < 200 || setRP->width > 1080)
	{
		cout << "Hauteur :";
		cin >> setRP->height;
	}


	menu(setRP);
}

void compChoice(Conf* setRP)
{
	int choix;

	cout <<"Méthode de comparaison :"<< endl;
	cout <<"1. Comparaison par pixels: Compare chaque pixel de deux images. (par défaut)" << endl;
	cout <<"2. Comparaison par contours: Compare les contours (des formes) dans les images (en noir et blanc)." << endl;
	cout <<"3. Comparaison par suppression de fond : Supprime les éléments statiques d'une image et détecte les changements." << endl;
	cout <<"4. Retour menu principal." << endl;

	cin >> choix;

	switch(choix) {
		case 1 :
			setRP->comp_method = 1;
			menu(setRP);
			break;
		case 2 :
			setRP->comp_method = 1;
			menu(setRP);
			break;
		case 3 :
			setRP->comp_method = 1;
			menu(setRP);
			break;
		case 4 :
			menu(setRP);
			break;
		default :
			compChoice(setRP);
			break;

	}

}

void createSocket(int argc, char *argv[] ,Conf* setRP )
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    if (argc < 3) {
       fprintf(stderr,"Utilisation %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERREUR ouverture socket");
    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERREUR, hôte inconnu\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERREUR connection");

    setRP->sock = sockfd;

}

void sendParameters(Conf* setRP)
{
	int numReq = 1;

	if(send(setRP->sock, &numReq, sizeof ( int), 0 ) < 0)
		error("Erreur envoi numéro requête paramètre");
	else {
		if( send(setRP->sock, &setRP->width, sizeof (unsigned int), 0 ) < 0 )
			error("Erreur envoi taille");
		else
		{

			if( send(setRP->sock, &setRP->height, sizeof (unsigned int), 0 ) < 0 )
				error("Erreur envoi taille");
			else
				cout <<"Envoi taille :" << setRP->width << "x" << setRP->height << endl;
		}
	}
}

void sendInit(Conf* setRP)
{
	int init = -1;
    if( send(setRP->sock, &init, sizeof (int), 0 ) < 0 )
    		error("Erreur envoi initialisation");
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

/*void apply_bgs(Conf_bgs* bgs)
{
	bgs->pMOG2 = createBackgroundSubtractorMOG2();


}
*/
