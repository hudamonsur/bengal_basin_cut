/*
 * verticle_cut.c

 *
 *  Created on: Mar 28, 2016
 *      Author: monsurul
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "getdepth.h"
//#include "constants_bengalbasin.h"
//#include "misc.h"
//#include "getpointer.h"

#define PI 3.14159265358979323846

int main(){

    initiate_layers();

	double lat1 = 25;
	double lat2 = 25;
	double long1 = 86.5;
	double long2 = 93.4;
	double dx = 0.01;
	int dz = 20;
	int size;
	int maxdepth = 1000;
	int ndepthpoints = maxdepth/dz;
	double angle;

	double diflat  = lat2 - lat1;
	double diflong = long2 - long1;
	//double *dist = malloc(100*sizeof(double*));

	double z[ndepthpoints];
	z[0] = 0;
	int i;
	int j;

	for (i = 1; i<ndepthpoints; i++){
		z[i] = z[i-1] + dz;
	}

	double distance = sqrt(diflat*diflat+diflong*diflong);
	size = distance/dx;
	double longitude[size];
	double lattitude[size];
	double dist[size];

	dist[0]=0;
	lattitude[0]=lat1;
	longitude[0]=long1;

	if(diflong>0){
		angle = atan2(diflat, diflong);
	}
	else {
		angle = PI/2;
	}

	for (i = 1; i<size; i++){
		dist[i] = dist[i-1]+dx;
		lattitude[i] = lat1 + dist[i]*(fabs(sin(angle)));
		longitude[i] = long1 + dist[i]*(fabs(cos(angle)));
	}
	for(j = 0; j<size; j++){
		dist[j] = dist[j]*111.01;
	}

	FILE * vsPtr;
	vsPtr = fopen("vertical_cut_along25lat_vs1.txt", "w");
	FILE * vpPtr;
	vpPtr = fopen("vertical_cut_along25lat_vp1.txt", "w");
	FILE * rhoPtr;
	rhoPtr = fopen("vertical_cut_along25lat_rho1.txt", "w");
	cvmpayload_t result;
	//FILE *fp;
	//fp=fopen("/Users/monsurul/Documents/uofm/Desktop/qualifying_phd/velocity model/depth.bin","rb");

	for(i = 0; i<(ndepthpoints); i++){
		for(j = 0; j<(size); j++){
			getdepth(lattitude[j], longitude[j], z[i], &result);
			double tempvs = result.Vs;
			double tempvp = result.Vp;
			double temprho = result.rho;
			/* in *(result+0),
			0 means shear wave velocity;
			1 means p-wave velocity and
			2 means density*/
			fprintf( vsPtr, "%.2f\t%.0f\t%.0f\n", dist[j], z[i], tempvs );
			fprintf( vpPtr, "%.2f\t%.0f\t%.0f\n", dist[j], z[i], tempvp );
			fprintf( rhoPtr, "%.2f\t%.0f\t%.0f\n", dist[j], z[i], temprho );
			//break;
		}
		printf("%d %d\n", i, ndepthpoints);
		//break;
	}

	puts("done!!!!!!!!");

}


