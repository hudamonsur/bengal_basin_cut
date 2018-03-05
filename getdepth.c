/* finding depth program */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
//#include<malloc.h>

#include "getdepth.h"
#include "constants_bengalbasin.h"

cvmpayload_t getmantlevalues(double depth, double mindepth);
cvmpayload_t getcrustvalues(double depth, double mindepth);
cvmpayload_t getbhubanvalues(double depth, double maxdepth, double mindepth);
cvmpayload_t getbokabilvalues(double depth, double maxdepth, double mindepth);
cvmpayload_t gettipamvalues(double depth, double maxdepth, double mindepth);
cvmpayload_t getdupitilavalues(double depth, double maxdepth, double mindepth);
cvmpayload_t getpremiocenevalues(double depth, double maxdepth, double mindepth);
cvmpayload_t getsedimentvalues(double depth);
double * getboreholevalues(double inputlat, double inputlong, double depth);
cvmpayload_t getsurfacevalues(double dlat, double dlong, double diflat, double diflong, double inputlat, double inputlong, double depth_m);
double getsurfacedepth(double dlat, double dlong, double diflong, double diflat, double east, double north, char* fname);

int getdepth(double north_m, double east_m, double depth_m, cvmpayload_t* result)
{
	cvmpayload_t temp_result;
	double dlat = (MAXLAT-MINLAT)/GRIDSTEP;
	double dlong = (MAXLONG-MINLONG)/GRIDSTEP;

	/*long bytevalue;*/
	// converting east and north values to lattitude and longitude
//	loc * inputloc = (loc*) malloc(2 * sizeof(loc));
//	inputloc.laty = MINLOC.laty + north_m/(DEGREETODIST);
//	inputloc.longx = MINLOC.longx + east_m/(DEGREETODIST);
//
//	loc * difloc = loc_dif(inputloc, MINLOC);

	double inputlat = MINLAT + north_m/(DEGREETODIST);
	//printf("%.4f\n", inputlat);
	double inputlong = MINLONG + east_m/(DEGREETODIST);
	//printf("%.4f\n", inputlong);

	double diflat = inputlat-MINLAT;
	double diflong = inputlong-MINLONG;

	if(inputlat>MAXLAT || inputlat<MINLAT || inputlong<MINLONG || inputlong>MAXLONG){
		printf("The co-ordinate is out of the region");
		return 0;
	}
	if (DEBUG==1)
	puts("<<<<<<<< primary variables loaded>>>>>>>>>>");

	// choosing among list of boreholes
	// get borehole values and confidence from list of boreholes
	if (DEBUG==1)
	puts("<<<<<<<<<<<<<< calculating borehole values>>>>>>>>>>>>>>>>>>>");
	double * bh_values;
	cvmpayload_t bh_params;
	bh_values = getboreholevalues(inputlat, inputlong, depth_m);
	double bh_confidence = *(bh_values+0);
	bh_params.Vs = *(bh_values+1);
	bh_params.Vp = *(bh_values+2);
	bh_params.rho = *(bh_values+3);

	// get Vs, Vp and rho from surfaces
	if (DEBUG==1)
	puts("<<<<<<<<<<<<<<<<<< calculating surface values>>>>>>>>>>>>>>>>>>>");
	cvmpayload_t surface_params = getsurfacevalues(dlat, dlong, diflat, diflong, inputlat, inputlong, depth_m);

	temp_result = surface_params;
    result->Vp  = temp_result.Vp;
    result->Vs  = temp_result.Vs;
    result->rho = temp_result.rho;

	return 0;
	//fclose(output);
	//printf("%f",value);
	//free(fp);

}


cvmpayload_t getsurfacevalues(double dlat, double dlong, double diflat, double diflong, double inputlat, double inputlong, double depth_m){

    cvmpayload_t result;
    if (DEBUG==1)
    puts("<<<<<<<<<<<<<< depth calculated>>>>>>>>>>>>>>>>");
	double value_precambrian = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, PRECAMBRIAN_BIN);
	double value_bhuban = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, BHUBAN_BIN);
	double value_bokabil = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, BOKABIL_BIN);
	double value_tipam = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, TIPAM_BIN);
	double value_dupitila = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, DUPITILA_BIN);

	if (depth_m<SEDIMENTDEPTH && (value_precambrian*-1)>SEDIMENTDEPTH){
		result = getsedimentvalues(depth_m);
	}
		else if (depth_m<(value_dupitila*-1) && depth_m>=(SEDIMENTDEPTH) && (value_precambrian*-1)>(value_dupitila*-1)){
		result = getdupitilavalues(depth_m, value_dupitila, SEDIMENTDEPTH);
		}
		else if (depth_m<(value_tipam*-1) && depth_m>=(value_dupitila*-1) && depth_m>100 && (value_tipam*-1)>0 && (value_precambrian*-1)>(value_dupitila*-1)){
		result = gettipamvalues(depth_m, value_tipam, value_dupitila);
		}
		else if (depth_m<(value_bokabil*-1) && depth_m>=(value_tipam*-1) && depth_m>100 && (value_bokabil*-1)>0 && (value_precambrian*-1)>(value_dupitila*-1)){
		result = getbokabilvalues(depth_m, value_bokabil, value_tipam);
		}
		else if (depth_m<(value_bhuban*-1) && depth_m>=(value_bokabil*-1) && depth_m>100 && (value_bhuban*-1)>0 && (value_precambrian*-1)>(value_dupitila*-1)) {
		result = getbhubanvalues(depth_m, value_bhuban, value_bokabil);
		}
		else if (depth_m<(value_precambrian*-1) && depth_m>=(value_bhuban*-1)){
        result = getpremiocenevalues(depth_m, value_precambrian, value_bhuban);
		}
		else if (depth_m>=(value_precambrian*-1) && depth_m<(value_precambrian*-1)+20000){
		result = getcrustvalues(depth_m, value_precambrian);
		}
		else{
		result = getmantlevalues(depth_m, value_precambrian);
		}

		return result;


}


double getsurfacedepth(double dlat, double dlong, double diflong, double diflat, double east, double north, char* fname){

	double value = 0.1;
	double val1;
	double val2;
	double val3;
	double val4;
	long byteval1;
	long byteval2;
	long byteval3;
	long byteval4;
    if (DEBUG==1)
	puts("<<<<<<<<<<<<<<<<<<< loading binary file>>>>>>>>>>>>>>>>>>>");
	fp = fopen(fname, "rb");
	if (DEBUG==1)
	puts("<<<<<<<<<<<<<<<<<<<<<<<<<<binary file loaded>>>>>>>>>>>>>>>>>>>>>>>>>");

	if (!fp)
	{
		printf("Unable to open binary file!");
		return 0;
	}

	else {
		/* bilinear interpolation*/
		long intervalnumberlongitude = diflong/dlong;
		long intervalnumberlatitude = diflat/dlat;

		if (intervalnumberlatitude==0 && intervalnumberlongitude==0){
			byteval1 = intervalnumberlongitude*4 + 4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			//printf("l>>>>>>>");
		}

		else if (intervalnumberlatitude==0){
			byteval1 = intervalnumberlongitude*4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			//printf("2>>>>>>>");
		}

		else if(intervalnumberlongitude==0){
			byteval1 = intervalnumberlatitude*8000 + 4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			//printf("3>>>>>>>");
		}

		else if(intervalnumberlatitude==2000 || intervalnumberlongitude==2000){
			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			//printf("4>>>>>>>");
		}

		else {
			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
			byteval2 = byteval1 + 8000;
			byteval3 = byteval2 - 4;
			byteval4 = byteval1 - 4;
			double lat1 = intervalnumberlatitude*dlat+MINLAT;
			double lat2 = lat1 + dlat;
			double long1 = intervalnumberlongitude*dlong+MINLONG;
			double long2 = long1 - dlong;

			/* seeking values */
			fseek(fp, byteval1, SEEK_SET);
			fread(&val1, 4, 1, fp);
			fseek(fp, byteval2, SEEK_SET);
			fread(&val2, 4, 1, fp);
			fseek(fp, byteval3, SEEK_SET);
			fread(&val3, 4, 1, fp);
			fseek(fp, byteval4, SEEK_SET);
			fread(&val4, 4, 1, fp);


			double R1 = ((long2-east)/(long2-long1))*val1 + ((east-long1)/(long2-long1))*val2;
			//printf("%.4f %.4f\n", long2, east);
			double R2 = ((long2-east)/(long2-long1))*val4 + ((east-long1)/(long2-long1))*val3;

			value = ((lat2-north)/(lat2-lat1))*R1 + ((north-lat1)/(lat2-lat1))*R2;
			//printf("%.4f\n", R2);
			//printf("5>>>>>>>\n");
		}

	}
	fclose(fp);
	//int a;
	//printf("%f >>>>>>> %d\n", value, a);
	return value;
}

cvmpayload_t getsedimentvalues(double depth){

    cvmpayload_t result;
	double vs;
	double density;
	double vp;

	double mindepth = 0;
	double maxdepth = SEDIMENTDEPTH;

    vs = (SEDMINVS+((SEDMAXVS-SEDMINVS)*(depth-mindepth))/(maxdepth-mindepth));
    vp = SEDVPVSRATIO*vs;
    density = (SEDMINDENSITY+((SEDMAXDENSITY-SEDMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));

	result.Vs = vs;
	result.Vp=vp;
	result.rho=density;
	//puts("sediment values:");

	return result;
}

cvmpayload_t getdupitilavalues(double depth, double maxdepth, double mindepth){

	cvmpayload_t result;
	double vs;
	double density;
	double vp;

	maxdepth = maxdepth*-1;
	if (maxdepth==mindepth){
		vs = DUPITILAMINVS;
		vp = DUPITILAVPVSRATIO*vs;
		density = DUPITILAMINDENSITY;
	}
	else{
		vs = (DUPITILAMINVS+((DUPITILAMAXVS-DUPITILAMINVS)*(depth-mindepth))/(maxdepth-mindepth));
		vp = DUPITILAVPVSRATIO*vs;
		density = (DUPITILAMINDENSITY+((DUPITILAMAXDENSITY-DUPITILAMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));
	}

	result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	return result;
}

cvmpayload_t gettipamvalues(double depth, double maxdepth, double mindepth){

	cvmpayload_t result;
	double vs;
	double density;
	double vp;

	maxdepth = maxdepth*-1;
	mindepth = mindepth*-1;
	if (maxdepth==mindepth){
		vs = TIPAMMINVS;
		vp = TIPAMVPVSRATIO*vs;
		density = TIPAMMINDENSITY;
	}
	else{
		vs = (TIPAMMINVS+((TIPAMMAXVS-TIPAMMINVS)*(depth-mindepth))/(maxdepth-mindepth));
		vp = TIPAMVPVSRATIO*vs;
		density = (TIPAMMINDENSITY+((TIPAMMAXDENSITY-TIPAMMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));
	}

	result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	//puts("tipam values:");

	return result;
}

cvmpayload_t getbokabilvalues(double depth, double maxdepth, double mindepth){

	cvmpayload_t result;
	double vs;
	double density;
	double vp;

    maxdepth = maxdepth*-1;
	mindepth = mindepth*-1;
	if (maxdepth==mindepth){
		vs = BOKABILMINVS;
		vp = BOKABILVPVSRATIO*vs;
		density = BOKABILMINDENSITY;
	}
	else{
		vs = (BOKABILMINVS+((BOKABILMAXVS-BOKABILMINVS)*(depth-mindepth))/(maxdepth-mindepth));
		vp = BOKABILVPVSRATIO*vs;
		density = (BOKABILMINDENSITY+((BOKABILMAXDENSITY-BOKABILMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));
	}
    result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	//puts("bokabil values:");

	return result;
}

cvmpayload_t getbhubanvalues(double depth, double maxdepth, double mindepth){

	cvmpayload_t result;
	double vs;
	double density;
	double vp;

    maxdepth = maxdepth*-1;
	mindepth = mindepth*-1;
	if (maxdepth==mindepth){
		vs = BHUBANMINVS;
		vp = BHUBANVPVSRATIO*vs;
		density = BHUBANMINDENSITY;
	}
	else{
		vs = (BHUBANMINVS+((BHUBANMAXVS-BHUBANMINVS)*(depth-mindepth))/(maxdepth-mindepth));
		vp = BHUBANVPVSRATIO*vs;
		density = (BHUBANMINDENSITY+((BHUBANMAXDENSITY-BHUBANMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));
	}

    result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	//puts("bhuban values:");

	return result;
}

cvmpayload_t getpremiocenevalues(double depth, double maxdepth, double mindepth){

 	cvmpayload_t result;
 	double vs;
 	double density;
 	double vp;

 	maxdepth = maxdepth*-1;
 	mindepth = mindepth*-1;
 	if (maxdepth==mindepth){
 		vs = PREMIOCENEMINVS;
 		vp = PREMIOCENEVPVSRATIO*vs;
 		density = PREMIOCENEMINDENSITY;
 	}
 	else{
 		vs = (PREMIOCENEMINVS+((PREMIOCENEMAXVS-PREMIOCENEMINVS)*(depth-mindepth))/(maxdepth-mindepth));
 		vp = PREMIOCENEVPVSRATIO*vs;
 		density = (PREMIOCENEMINDENSITY+((PREMIOCENEMAXDENSITY-PREMIOCENEMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));
 	}
    result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
 	//puts("premiocene values:");

 	return result;
 }

cvmpayload_t getcrustvalues(double depth, double mindepth){

	cvmpayload_t result;
	double vs;
	double density;
	double vp;

	mindepth = mindepth*-1;

	double maxdepth = mindepth+APPROXCRUSTTHICKNESS;
	vs = (PRECAMBRIANMINVS+((PRECAMBRIANMAXVS-PRECAMBRIANMINVS)*(depth-mindepth))/(maxdepth-mindepth));
	vp = PRECAMBRIANVPVSRATIO*vs;
	density = (PRECAMBRIANMINDENSITY+((PRECAMBRIANMAXDENSITY-PRECAMBRIANMINDENSITY)*(depth-mindepth))/(maxdepth-mindepth));

    result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	//puts("precambrian values:");
	return result;
}

cvmpayload_t getmantlevalues(double depth, double mindepth){

	cvmpayload_t result;
	double vs;
	double vp;
	double density;

	mindepth = mindepth*-1 + APPROXCRUSTTHICKNESS;
	double maxdepth = depth;
	vs = (MANTLEMINVS+((MANTLEMAXVS-MANTLEMINVS)*(depth-mindepth))/(maxdepth-mindepth));
	vp = MANTLEVPVSRATIO*vs;
	density = MANTLEDENSITY;

    result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	//puts("mantle values:");
	return result;
}

double * getboreholevalues(double inputlat, double inputlong, double depth){

    double bh_confidence_all[100];
    double result[4];
    double bh_confidence = 0;
    double vs = 0;
    double vp = 0;
    double density = 0;
	char ** bh_names = (char**) malloc(4000 * sizeof(char*));
	int bhnumbers = 0;
    int linecount = 0;
	int ch = 0;
	fp = fopen(BHNAMES_LIST,"r");

	if (!fp)
	{
		printf("Unable to open borehole list!");
		return 0;
	}
	else {
		while(!feof(fp)) {
//			ch = fgetc(fp6);
//			if(ch=='\n') {
//				linecount++;
//
//			}
			char str[60];
			if( fgets (str, 60, fp)!=NULL ) {
            /* writing content to stdout */
                if(str[0]=='\n'){
                    linecount++;
                }
                char *borehole_name = strtok(str, " ");
                char *borehole_lat = strtok(NULL, " ");
                char *borehole_long = strtok(NULL, " ");
//                puts(borehole_name);
//                puts(boreholeinfo[1]);
                double borehole_latf =  atof(borehole_lat);
                double borehole_longf =  atof(borehole_long);
                double borehole_dist = (sqrt(pow((borehole_latf-inputlat),2) + pow((borehole_longf-inputlong),2)))*111.01;
                double dist_lim = 100;

                if(borehole_dist<100){
                    double borehole_confidence = (100-borehole_dist)/100;
                    char str1[] = ".txt";
                    char * selected_borehole = (char*) malloc(40 * sizeof(char));
                    strcpy(selected_borehole, borehole_name);
                    strcat(selected_borehole, str1);
                    bh_names[bhnumbers] = selected_borehole;
                    //strcpy(n1[bhnumbers], selected_borehole);
                    bh_confidence_all[bhnumbers] = borehole_confidence;
                    //printf("%d %s\n",bhnumbers,bh_names[bhnumbers]);
                    bhnumbers++;
//                    puts(selected_borehole);

                }
//                printf("%.4f\n", dist);


//                printf("%.3f\n", borehole_latf);
//                printf("%.3f\n", borehole_longf);

            }

		}
	}
	fclose(fp);

	result[0] = bh_confidence;
	result[1] = vs;
	result[2] = vp;
	result[3] = density;
	//puts("<<<<<<<<borehole values calculated>>>>>>");

	return result;

}


