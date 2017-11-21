/*
 * horizontal_cut.c
 *
 *  Created on: Apr 13, 2016
 *      Author: monsurul
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "getdepth.h"
//#include "getpointer.h"


// the program generates an array of values of material properties at a certain depth

int main(){

	float minlat= 19.9;
	float maxlat= 27;
	float minlong= 86.5;
	float maxlong= 93.4;
	float diflat = maxlat-minlat;
	float diflong = maxlong-minlong;
	float dx = 0.01;
	float depth = 500;

	int sizex = diflong/dx;
	int sizey = diflat/dx;
	float lattitude[sizey];
	float longitude[sizex];
	float x[sizex];
	float y[sizey];

	lattitude[0] = minlat;
	longitude[0] = minlong;
	x[0] = 0;
	y[0] = 0;

	for (int i = 1; i<sizex; i++){
		x[i] = x[i-1] + dx * 111.01;
		longitude[i] = longitude[i-1] + dx;
	}

	for (int i = 1; i<sizey; i++){
		y[i] = y[i-1] + dx * 111.01;
		lattitude[i] = lattitude[i-1] + dx;
	}

	FILE * vsPtr;
	vsPtr = fopen("horizontal_cut_500m_vs.txt", "w");
	FILE * vpPtr;
	vpPtr = fopen("horizontal_cut_500m_vp.txt", "w");
	FILE * rhoPtr;
	rhoPtr = fopen("horizontal_cut_500m_rho.txt", "w");

	float * result;

	for(int i = 0; i<sizex; i++){
		for(int j = 0; j<sizey; j++){
			result = getdepth(lattitude[j], longitude[i], depth);
			float tempvs = *(result+0);
			float tempvp = *(result+1);
			float temprho = *(result+2);
			/* in *(result+0),
			0 means shear wave velocity;
			1 means p-wave velocity and
			2 means density*/
			fprintf(vsPtr, "%.2f\t%.2f\t%.0f\n", x[i], y[j], tempvs);
			fprintf(vpPtr, "%.2f\t%.2f\t%.0f\n", x[i], y[j], tempvp);
			fprintf(rhoPtr, "%.2f\t%.2f\t%.0f\n", x[i], y[j], temprho);
		}
	}

}
