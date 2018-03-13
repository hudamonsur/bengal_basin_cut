/*
 * single_query.c
 *
 *  Created on: Mar 27, 2016
 *      Author: monsurul
 */

#include<stdio.h>
#include<stdlib.h>
#include "getdepth.h"


int main(){

	initiate_layers();
	cvmpayload_t result;
	double minlat = 19.9;
	double minlong = 86.5;
	double lattitude = 25.57;
	double longitude = 89.07;
	double degreetodist = 111.01*1000;
	double north = (lattitude-minlat)*degreetodist;
	double east = (longitude-minlong)*degreetodist;
	double z = 20;

	getdepth(north, east, z, &result);
	double vs = result.Vs;
	double vp = result.Vp;
	double rho = result.rho;

    printf("%f\n", vs);
    printf("%f\n", vp);
    printf("%f\n", rho);






}
