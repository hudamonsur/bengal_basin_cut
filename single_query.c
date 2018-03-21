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

//double getsurfacedepth(double dlat, double dlong, double diflong, double diflat, double east, double north, char* fname);
int main(){

	initiate_layers();

	cvmpayload_t result;
	double minlat = 19.9;
	double minlong = 86.5;
	double lattitude = 25;
	double longitude = 90;
	double degreetodist = 111.01*1000;
	double north = (lattitude-minlat)*degreetodist;
	double east = (longitude-minlong)*degreetodist;

//	double dlat = (MAXLAT-MINLAT)/GRIDSTEP;
//	double dlong = (MAXLONG-MINLONG)/GRIDSTEP;
//	double inputlat = MINLAT + north/(DEGREETODIST);
//	//printf("%.4f\n", inputlat);
//	double inputlong = MINLONG + east/(DEGREETODIST);
//	//printf("%.4f\n", inputlong);
//    double diflat = inputlat-MINLAT;
//	double diflong = inputlong-MINLONG;
	double z = 200;
//	double value_precambrian = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, PRECAMBRIAN_BIN);
//	double value_bhuban = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, BHUBAN_BIN);

	getdepth(north, east, z, &result);
	double vs = result.Vs;
	double vp = result.Vp;
	double rho = result.rho;

    printf("%f\n", vs);
    printf("%f\n", vp);
    printf("%f\n", rho);
//    printf("%f\n", value_precambrian);
//    printf("%f\n", value_bhuban);

}

//double getsurfacedepth(double dlat, double dlong, double diflong, double diflat, double east, double north, char* fname){
//
//	double value=1;
//	float val1=1;
//	float val2=1;
//	float val3=1;
//	float val4=1;
//	long byteval1;
//	long byteval2;
//	long byteval3;
//	long byteval4;
//    if (DEBUG==1){
//        puts("<<<<<<<<<<<<<<<<<<< loading binary file>>>>>>>>>>>>>>>>>>>");
//    }
//	fp = fopen(fname, "rb");
//	if (DEBUG==1){
//        puts("<<<<<<<<<<<<<<<<<<<<<<<<<<binary file loaded>>>>>>>>>>>>>>>>>>>>>>>>>");
//
//	}
//
//	if (!fp)
//	{
//		printf("Unable to open binary file!");
//		return 0;
//	}
//	else {
//		/* bilinear interpolation*/
//		long intervalnumberlongitude = diflong/dlong;
//		printf("%d\n", intervalnumberlongitude);
//		long intervalnumberlatitude = diflat/dlat;
//		printf("%d\n", intervalnumberlatitude);
//
//
//		if (intervalnumberlatitude==0 && intervalnumberlongitude==0){
//			byteval1 = intervalnumberlongitude*4 + 4;
//			fseek(fp, byteval1, SEEK_SET);
//			fread(&value, 4, 1, fp);
//			printf("l>>>>>>>");
//		}
//		else if (intervalnumberlatitude==0){
//			byteval1 = intervalnumberlongitude*4;
//			fseek(fp, byteval1, SEEK_SET);
//			fread(&value, 4, 1, fp);
//			printf("2>>>>>>>");
//		}
//		else if(intervalnumberlongitude==0){
//			byteval1 = intervalnumberlatitude*8000 + 4;
//			fseek(fp, byteval1, SEEK_SET);
//			fread(&value, 4, 1, fp);
//			printf("3>>>>>>>");
//		}
//		else if(intervalnumberlatitude==2000 || intervalnumberlongitude==2000){
//			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
//			fseek(fp, byteval1, SEEK_SET);
//			fread(&value, 4, 1, fp);
//			printf("4>>>>>>>");
//		}
//		else {
//			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
//			byteval2 = byteval1 + 8000;
//			byteval3 = byteval2 - 4;
//			byteval4 = byteval1 - 4;
//			double lat1 = intervalnumberlatitude*dlat+MINLAT;
//			double lat2 = lat1 + dlat;
//			double long1 = intervalnumberlongitude*dlong+MINLONG;
//			double long2 = long1 - dlong;
//
//			/* seeking values */
//			fseek(fp, byteval1, SEEK_SET);
//			fread(&val1, 4, 1, fp);
//			//printf("%f\n", val1);
//			fseek(fp, byteval2, SEEK_SET);
//			fread(&val2, 4, 1, fp);
//			fseek(fp, byteval3, SEEK_SET);
//			fread(&val3, 4, 1, fp);
//			fseek(fp, byteval4, SEEK_SET);
//			fread(&val4, 4, 1, fp);
//			//printf("%f\n", val4);
//
//			double R1 = ((long2-east)/(long2-long1))*val1 + ((east-long1)/(long2-long1))*val2;
//			printf("%.4f %.4f %.4f\n", long2, long1, east);
//			double R2 = ((long2-east)/(long2-long1))*val4 + ((east-long1)/(long2-long1))*val3;
//			value = ((lat2-north)/(lat2-lat1))*R1 + ((north-lat1)/(lat2-lat1))*R2;
//			printf("%.4f\n", R2);
//			printf("5>>>>>>>\n");
//		}
//	}
//	fclose(fp);
//	//int a;
//	//printf("%f >>>>>>> %d\n", value, a);
//	//value = value;
//	return value;
//}
