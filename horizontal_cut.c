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

	double lat1 = 19.9;
	double lat2 = 27;
	double long1 = 86.5;
	double long2 = 93.4;
	double minlat = 19.9;
	double minlong = 86.5;
	double diflat = lat2-lat1;
	double diflong = long2-long1;
	double dx = 0.01;
	double depth = 2000;
	cvmpayload_t result;
	double degreetodist = 111.01*1000;

	int sizex = diflong/dx;
	int sizey = diflat/dx;
	double lattitude[sizey];
	double longitude[sizex];
	double x[sizex];
	double y[sizey];
    double north[sizey];
	double east[sizex];

	lattitude[0] = lat1;
	longitude[0] = long1;
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
	for(i=0;i<sizex;i++){
        east[i] = (longitude[i]-minlong)*degreetodist;
	}
	for(i=0;i<sizey;i++){
        north[i] = (lattitude[i]-minlat)*degreetodist;
	}

	FILE * vsPtr;
	vsPtr = fopen("horizontal_cut_20km_exp2000m_vs.txt", "w");
	FILE * vpPtr;
	vpPtr = fopen("horizontal_cut_20km_exp2000m_vp.txt", "w");
	FILE * rhoPtr;
	rhoPtr = fopen("horizontal_cut_20km_exp2000m_rho.txt", "w");

	for(i = 0; i<sizex; i++){
		for(j = 0; j<sizey; j++){
			getdepth(north[j], east[i], depth, &result);
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
