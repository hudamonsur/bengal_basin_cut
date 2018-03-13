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
    initiate_layers();

	float minlat= 24.3;
	float maxlat= 25.7;
	float minlong= 88.5;
	float maxlong= 89.7;
	float diflat = maxlat-minlat;
	float diflong = maxlong-minlong;
	float dx = 0.01;
	float depth = 20;
	cvmpayload_t result;

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
	int i;
	int j;

	for (i = 1; i<sizex; i++){
		x[i] = x[i-1] + dx * 111.01;
		longitude[i] = longitude[i-1] + dx;
	}

	for (i = 1; i<sizey; i++){
		y[i] = y[i-1] + dx * 111.01;
		lattitude[i] = lattitude[i-1] + dx;
	}

	FILE * vsPtr;
	vsPtr = fopen("horizontal_cut_200m_vs.txt", "w");
	FILE * vpPtr;
	vpPtr = fopen("horizontal_cut_200m_vp.txt", "w");
	FILE * rhoPtr;
	rhoPtr = fopen("horizontal_cut_200m_rho.txt", "w");

	for(i = 0; i<sizex; i++){
		for(j = 0; j<sizey; j++){
			getdepth(lattitude[j], longitude[i], depth, &result);
			double tempvs = result.Vs;
			double tempvp = result.Vp;
			double temprho = result.rho;
			/* in *(result+0),
			0 means shear wave velocity;
			1 means p-wave velocity and
			2 means density*/
			fprintf(vsPtr, "%.2f\t%.2f\t%.2f\n", x[i], y[j], tempvs);
			fprintf(vpPtr, "%.2f\t%.2f\t%.2f\n", x[i], y[j], tempvp);
			fprintf(rhoPtr, "%.2f\t%.2f\t%.2f\n", x[i], y[j], temprho);
		}
		printf("%d %d\n", i, sizex);
	}
	puts("done!!!!!!!!");

}
