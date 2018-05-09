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

	double minlat = 19.9;
	double minlong = 86.5;
	int dz = 20;
	int maxdepth = 4500;
	int ndepthpoints = maxdepth/dz;
	double z[ndepthpoints];
	z[0] = 0;
	int i;
	int j;

	for (i = 1; i<ndepthpoints; i++){
		z[i] = z[i-1] + dz;
	}

	cvmpayload_t result;
	double degreetodist = 111.01*1000;
	FILE * fp = fopen("borehole_list.txt","r");
		if (!fp)
	{
		printf("Unable to open borehole list!");
		return 0;
	}
	else {
		while(!feof(fp)) {
			char str[60];
			if( fgets (str, 60, fp)!=NULL ) {
            /* writing content to stdout */
                char *borehole_name = strtok(str, " ");
                char *borehole_lat = strtok(NULL, " ");
                char *borehole_long = strtok(NULL, " ");
//                puts(borehole_name);
//                puts(boreholeinfo[1]);
                double borehole_latf =  atof(borehole_lat);
                double borehole_longf =  atof(borehole_long);
                double north = (borehole_latf-minlat)*degreetodist;
                double east = (borehole_longf-minlong)*degreetodist;
                FILE * vsPtr;
                char str1[] = "_with_borehole.txt";
                char * borehole = (char*) malloc(60 * sizeof(char));
                strcpy(borehole, borehole_name);
                strcat(borehole, str1);
                vsPtr = fopen(borehole, "w");
                for(i=0;i<ndepthpoints;i++){
                    getdepth(north, east, z[i], &result);
                    fprintf( vsPtr, "%.0f\t%.0f\t%.0f\t%.0f\n", z[i], result.Vs, result.Vp, result.rho);
                    //printf("%d %d\n", i, ndepthpoints);
                }
                free(borehole);
            }
		}
	}
	fclose(fp);

	puts("done!!!!!!!!");

}
