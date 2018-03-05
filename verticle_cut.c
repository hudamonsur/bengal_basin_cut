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

	double lat1 = 19.9;
	double lat2 = 27;
	double long1 = 91;
	double long2 = 91;
	double dx = 0.01;
	int dz = 200;
	int size;
	int maxdepth = 50000;
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
	vsPtr = fopen("vertical_cut_along91long_vs1.txt", "w");
	FILE * vpPtr;
	vpPtr = fopen("vertical_cut_along91long_vp1.txt", "w");
	FILE * rhoPtr;
	rhoPtr = fopen("vertical_cut_along91long_rho1.txt", "w");
	cvmpayload_t result;
	//FILE *fp;
	//fp=fopen("/Users/monsurul/Documents/uofm/Desktop/qualifying_phd/velocity model/depth.bin","rb");

	for(i = 0; i<ndepthpoints; i++){
		for(j = 0; j<size; j++){
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
			break;
		}
		printf("%d %d\n", i, ndepthpoints);
		break;
	}
	//fclose(fp);
	//fclose(filePtr);
	//fclose(result);

	/*double dif = 0.1;
	int n = diflong/dif;
	double x[n];
	FILE *stations;
	stations = fopen("stations along 26 latitude.txt", "w");

	for(int i = 0; i < n; i++){
		x[i] = long1 + i * dif;
		fprintf( stations, "%.2f\t%.2f\n", lat1, x[i] );
	}*/
	puts("done!!!!!!!!");

}


