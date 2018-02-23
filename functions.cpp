/*
 * functions.cpp
 *
 *  Created on: 11 févr. 2018
 *      Author: pi
 */

#include "functions.hpp"

void create_socket(int argc, char *argv[], captureConf* setCap) {

	if (argc < 2) {
		fprintf(stderr, "Aucun port spécifié.\n");
		exit(1);
	}
	setCap->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (setCap->sockfd < 0)
		error("Erreur ouverture socket");

	bzero((char *) &setCap->serv_addr, sizeof(setCap->serv_addr));
	setCap->portno = atoi(argv[1]);
	setCap->serv_addr.sin_family = AF_INET;
	setCap->serv_addr.sin_addr.s_addr = INADDR_ANY;
	setCap->serv_addr.sin_port = htons(setCap->portno);
	if (bind(setCap->sockfd, (struct sockaddr *) &setCap->serv_addr,
			sizeof(setCap->serv_addr)) < 0)
		error("Erreur liaison de la socket");
	listen(setCap->sockfd, 5);
	setCap->clilen = sizeof(setCap->cli_addr);

}

void requests(captureConf* setCap) {
	int numReq = 0;

	while (numReq != -1) {

		if (recv(setCap->newsockfd, &numReq, sizeof(int), 0) == -1)
		 error("Erreur réception numéro requête");

		switch (numReq) {
		case 1:
			getImageParameters(setCap);
			break;
		case 2:
			receiveImage(setCap);
			break;

		}
	}
}

void getImageParameters(captureConf* setCap)

{
	if ((recv(setCap->newsockfd, &setCap->width, sizeof(unsigned int), 0))
			== -1)
		error("Erreur réception taille");
	else {
		if ((recv(setCap->newsockfd, &setCap->height, sizeof(unsigned int), 0))
				== -1)
			error("Erreur réception taille");
		else
			cout << "Réception taille : " << setCap->width << "x"
					<< setCap->height << endl;
	}
}

void receiveImage(captureConf* setCap) {

	//Déclarations variables

	time_t currentTime;
	//Récupération du temps en millisecondes et microsecondes
	struct timeval tps;
	char currentTimeString[SIZE_OF_TIME_STRING];
	char ext_nom[5]; //char pour les millisecondes dans le nom de l'image

	//long long debut = (long long) tps.tv_sec * 1000L + tps.tv_usec / 1000;



	Mat img = Mat::zeros(setCap->height, setCap->width, CV_8UC3);
	int imgSize = img.total() * img.elemSize();
	uchar sockData[imgSize];
	int bytes;

	//Réception des images

	for (int i = 0; i < imgSize; i += bytes) {
		if ((bytes = recv(setCap->newsockfd, sockData + i, imgSize - i, 0))
				== -1) {
			exit(1);
		}
	}

	currentTime = time(NULL);
	strftime(currentTimeString, SIZE_OF_TIME_STRING, "%d-%m-%Y_%H-%M-%S",
			localtime(&currentTime));
	gettimeofday(&tps, NULL);

	//Création du répertoire

	if (time(NULL) - setCap->temps > 60 || setCap->first_rec) {
		//strftime(subDir_name, SIZE_OF_TIME_STRING, "dest/%d-%m-%Y_%H-%M", localtime(&currentTime));

		//Création arborescence répertoire de destination
		strcpy(setCap->lastDir, "dest/");
		strcat(setCap->lastDir, currentTimeString);

		umask(0);
		const int dir = mkdir(setCap->lastDir, S_IRWXU | S_IRWXG | S_IRWXO);
		setCap->first_rec = false;

	}

	char *imageName = (char *) malloc(
			(strlen("dest/") + strlen(setCap->lastDir) + strlen(currentTimeString)
					+ strlen("/") + strlen(ext_nom) + strlen(".jpg"))
					* sizeof(char));

	sprintf(ext_nom, "-%03d", getMillisec(&tps));
	strcpy(imageName, setCap->lastDir);
	strcat(imageName, "/");
	strcat(imageName, currentTimeString);
	strcat(imageName, ext_nom);
	strcat(imageName, ".jpg");



	Mat img2(Size(setCap->width, setCap->height), CV_8UC3, sockData);

	printf("Ecriture image : %s\n", imageName);

	imwrite(imageName, img2);
	free(imageName);

	img.release();
	img2.release();

	setCap->temps = time(NULL);

	/*gettimeofday(&tps, NULL);
	long long fin = (long long) tps.tv_sec * 1000L + tps.tv_usec / 1000;
	cout << time(NULL) - fin * 1000 << " ms" << endl;
	cout << 1000 / (fin - debut) << " FPS" << endl;*/

}

int getMillisec(struct timeval *temps)
{
	return lrint(temps->tv_usec/1000.0); //Nombres de millisecondes dans les microsecs

}

void error(const char *msg) {
	perror(msg);
	exit(1);
}
