/*
 * single_query.c
 *
 *  Created on: Mar 27, 2016
 *      Author: monsurul
 */

#include<stdio.h>
#include<stdlib.h>
#include "getdepth.h"
//#include "constants_bengalbasin.h"
//#include "misc.h"

int main(){

	initiate_layers();

	cvmpayload_t result;
	double minlat = 19.9;
	double minlong = 86.5;
	double lattitude = 24.915;
	double longitude = 92.3374;
	double degreetodist = 111.01*1000;
	double north = (lattitude-minlat)*degreetodist;
	double east = (longitude-minlong)*degreetodist;
	const char * binFileNames[] = {
        "depth_sediment.bin",
        "depth_dupitila.bin",
        "depth_tipam.bin",
        "depth_bokabil.bin",
        "depth_bhuban.bin",
        "depth_precambrian.bin",
        "depth_moho.bin",
    };

	int colcount = 2000*2000;
	int rowcount = 7;
	double **surfaces = (double **)malloc(rowcount * sizeof(double *));
	int i;
	int j;
	for(i=0;i<rowcount;i++){
        surfaces[i] = (double *)malloc(colcount * sizeof(double));
	};

	for(i=0;i<rowcount;i++){
        FILE *contourFiles;
        contourFiles = fopen(binFileNames[i], "rb");
        if (!contourFiles)
        {
            printf("Unable to open binary file!");
            return 0;
        }
        for(j=0;j<colcount;j++){
            fread(&surfaces[i][j], sizeof(double), 1, contourFiles);
        }
        fclose(contourFiles);
    }

//    for(i=0;i<colcount;i++){
//        printf("%f\n",surfaces[2000][i]);
//	}

	double z = 0;
	getdepth(north, east, z, &result, surfaces);
	free(surfaces);
	double vs = result.Vs;
	double vp = result.Vp;
	double rho = result.rho;

    printf("%f\n", vs);
    printf("%f\n", vp);
    printf("%f\n", rho);

}
