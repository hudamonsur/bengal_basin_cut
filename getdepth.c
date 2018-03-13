/* finding depth program */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
//#include<malloc.h>

#include "getdepth.h"
#include "constants_bengalbasin.h"

//cvmpayload_t getmantlevalues(double depth, double mindepth);
//cvmpayload_t getcrustvalues(double depth, double mindepth);
//cvmpayload_t getbhubanvalues(double depth, double maxdepth, double mindepth);
//cvmpayload_t getbokabilvalues(double depth, double maxdepth, double mindepth);
//cvmpayload_t gettipamvalues(double depth, double maxdepth, double mindepth);
//cvmpayload_t getdupitilavalues(double depth, double maxdepth, double mindepth);
//cvmpayload_t getpremiocenevalues(double depth, double maxdepth, double mindepth);
//cvmpayload_t getsedimentvalues(double depth);
cvmpayload_t getlayervalues(layer_params layer, double depth, double maxdepth, double mindepth);
double * getboreholevalues(double inputlat, double inputlong, double depth);
cvmpayload_t getsurfacevalues(double dlat, double dlong, double diflat, double diflong, double inputlat, double inputlong, double depth);
double getsurfacedepth(double dlat, double dlong, double diflong, double diflat, double east, double north, char* fname);
cvmpayload_t getbhvalues(char* bhname, double depth, double confidence);
double sum_array(double array[], int size);
cvmpayload_t computefinalvalues(cvmpayload_t bh_params, cvmpayload_t surface_params, double bh_confidence);
//double avg_array(double array, int size);

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
	if (DEBUG==1){
        puts("<<<<<<<< primary variables loaded>>>>>>>>>>");
	}

	// choosing among list of boreholes
	// get borehole values and confidence from list of boreholes
	if (DEBUG==1){
        puts("<<<<<<<<<<<<<< calculating borehole values>>>>>>>>>>>>>>>>>>>");
	};

	double * bh_values;
	cvmpayload_t bh_params;
	bh_values = getboreholevalues(inputlat, inputlong, depth_m);
	double bh_confidence = *(bh_values+0);
	bh_params.Vs = *(bh_values+1);
	bh_params.Vp = *(bh_values+2);
	bh_params.rho = *(bh_values+3);

	// get Vs, Vp and rho from surfaces
	if (DEBUG==1){
        puts("<<<<<<<<<<<<<<<<<< calculating surface values>>>>>>>>>>>>>>>>>>>");
	};

	cvmpayload_t surface_params = getsurfacevalues(dlat, dlong, diflat, diflong, inputlat, inputlong, depth_m);
    if (DEBUG==1){
        puts("<<<<<<<<<<<<<< calculating return values>>>>>>>>>>>>>>>>>>>");
	};
	//temp_result = bh_params;

	temp_result = computefinalvalues(bh_params, surface_params, bh_confidence);
    result->Vp  = temp_result.Vp;
    result->Vs  = temp_result.Vs;
    result->rho = temp_result.rho;

	return 0;
	//fclose(output);
	//printf("%f",value);
	//free(fp);

}

cvmpayload_t computefinalvalues(cvmpayload_t bh_params, cvmpayload_t surface_params, double bh_confidence){
    cvmpayload_t result;
    result.rho = bh_params.rho*bh_confidence + surface_params.rho*(1-bh_confidence);
	result.Vp = bh_params.Vp*bh_confidence + surface_params.Vp*(1-bh_confidence);
	result.Vs = bh_params.Vs*bh_confidence + surface_params.Vs*(1-bh_confidence);

	return result;

}

cvmpayload_t getsurfacevalues(double dlat, double dlong, double diflat, double diflong, double inputlat, double inputlong, double depth){

    cvmpayload_t result;
    if (DEBUG==1){
        puts("<<<<<<<<<<<<<< depth calculated>>>>>>>>>>>>>>>>");
    };

	double value_precambrian = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, PRECAMBRIAN_BIN);
	printf("%.2f\n", value_precambrian);
	double value_bhuban = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, BHUBAN_BIN);
	printf("%.2f\n", value_bhuban);
	double value_bokabil = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, BOKABIL_BIN);
	printf("%.2f\n", value_bokabil);
	double value_tipam = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, TIPAM_BIN);
	printf("%.2f\n", value_tipam);
	double value_dupitila = getsurfacedepth(dlat, dlong, diflong, diflat, inputlong, inputlat, DUPITILA_BIN);
	printf("%.2f\n", value_dupitila);

	if (depth>(value_precambrian+APPROXCRUSTTHICKNESS)){
        result = getlayervalues(LAYERS[7], depth, depth, value_precambrian);
		//result = getsedimentvalues(depth_m);
	}
		else if (depth<(value_dupitila) && depth>=(SEDIMENTDEPTH) && (value_precambrian)>(value_dupitila)){
		result = getlayervalues(LAYERS[1], depth, value_dupitila, SEDIMENTDEPTH);
		//result = getdupitilavalues(depth_m, value_dupitila, SEDIMENTDEPTH);
		}
		else if (depth<(value_tipam) && depth>=(value_dupitila) && depth>100 && (value_tipam)>0 && (value_precambrian)>(value_dupitila)){
		result = getlayervalues(LAYERS[2], depth, value_tipam, value_dupitila);
		//result = gettipamvalues(depth_m, value_tipam, value_dupitila);
		}
		else if (depth<(value_bokabil) && depth>=(value_tipam) && depth>100 && (value_bokabil)>0 && (value_precambrian)>(value_dupitila)){
		result = getlayervalues(LAYERS[3], depth, value_bokabil, value_tipam);
		//result = getbokabilvalues(depth_m, value_bokabil, value_tipam);
		}
		else if (depth<(value_bhuban) && depth>=(value_bokabil) && depth>100 && (value_bhuban)>0 && (value_precambrian)>(value_dupitila)) {
		result = getlayervalues(LAYERS[4], depth, value_bhuban, value_bokabil);
		//result = getbhubanvalues(depth_m, value_bhuban, value_bokabil);
		}
		else if (depth<(value_precambrian) && depth>=(value_bhuban)){
        result = getlayervalues(LAYERS[5], depth, value_precambrian, value_bhuban);
        //result = getpremiocenevalues(depth_m, value_precambrian, value_bhuban);
		}
		else if (depth>=(value_precambrian) && depth<(value_precambrian)+20000){
		result = getlayervalues(LAYERS[6], depth, value_precambrian+APPROXCRUSTTHICKNESS, value_precambrian);
		//result = getcrustvalues(depth_m, value_precambrian);
		}
		else{
		result = getlayervalues(LAYERS[7], depth, depth, value_precambrian);
		//result = getmantlevalues(depth_m, value_precambrian);
		}

		return result;


}

cvmpayload_t getlayervalues(layer_params layer, double depth, double maxdepth, double mindepth){

    cvmpayload_t result;
	double vs;
	double density;
	double vp;

	//maxdepth = maxdepth*-1;
	//mindepth = mindepth*-1;
	if (maxdepth==mindepth){
		vs = layer.minvs;
		vp = layer.vpvsratio*vs;
		density = layer.minrho;
	}
	else{
		vs = (layer.minvs+((layer.maxvs-layer.minvs)*(depth-mindepth))/(maxdepth-mindepth));
		vp = layer.vpvsratio*vs;
		density = (layer.minrho+((layer.maxrho-layer.minrho)*(depth-mindepth))/(maxdepth-mindepth));
	}

	result.Vs=vs;
	result.Vp=vp;
	result.rho=density;
	return result;

}

double * getboreholevalues(double inputlat, double inputlong, double depth){

    double bh_confidence_all[100];
    double bh_vs[100];
    double bh_vp[100];
    double bh_rho[100];
    double result[4];
    double bh_confidence = 0;
    double vs = 0;
    double vp = 0;
    double density = 0;
	char ** bh_names = (char**) malloc(4000 * sizeof(char*));
	int bhnumbers = 0;
    int linecount = 0;
	int ch = 0;
	if (DEBUG==1){
        puts("<<<<<<<<<<<<<< opening borehole list >>>>>>>>>>>>>>>>>>>");
	};
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
                //double dist_lim = 100;

                if(borehole_dist<100){
                    double borehole_confidence = (100-borehole_dist)/100;
                    char str1[] = ".txt";
                    char * selected_borehole = (char*) malloc(40 * sizeof(char));
                    strcpy(selected_borehole, borehole_name);
                    strcat(selected_borehole, str1);
                    //puts(selected_borehole);
                    cvmpayload_t bh_values = getbhvalues(selected_borehole, depth, borehole_confidence);
                    bh_names[bhnumbers] = selected_borehole;
                    //strcpy(n1[bhnumbers], selected_borehole);
                    bh_confidence_all[bhnumbers] = borehole_confidence;
                    bh_rho[bhnumbers] = bh_values.rho;
                    bh_vp[bhnumbers] = bh_values.Vp;
                    bh_vs[bhnumbers] = bh_values.Vs;
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
	printf("%s %d\n", "number of boreholes:", bhnumbers);
	if(bhnumbers>0){
        vs = sum_array(bh_vs,bhnumbers)/sum_array(bh_confidence_all,bhnumbers); printf("%0.4f\n", vs);
        vp = sum_array(bh_vp,bhnumbers)/sum_array(bh_confidence_all,bhnumbers); printf("%0.4f\n", vp);
        density = sum_array(bh_rho,bhnumbers)/sum_array(bh_confidence_all,bhnumbers); printf("%0.4f\n", density);
        bh_confidence = sum_array(bh_confidence_all,bhnumbers)/bhnumbers; printf("%0.4f\n", bh_confidence);
	}


	result[0] = bh_confidence;
	result[1] = vs;
	result[2] = vp;
	result[3] = density;
	//puts("<<<<<<<<borehole values calculated>>>>>>");

	return result;


}

double sum_array(double array[], int size){
    double sum = 0; // initialize sum

    // Iterate through all elements
    // and add them to sum
    int i = 0;
    for ( i ; i < size; i++)
    sum += array[i];

    return sum;

}

cvmpayload_t getbhvalues(char* bhname, double depth, double confidence){

    cvmpayload_t result;
    cvmpayload_t tmp_result;
    char ** name_layers = (char**) malloc(400 * sizeof(char*));
    double layer_depth[10];
    int layercount = 0;
    if (DEBUG==1){
        puts("<<<<<<<<<<<<<< opening borehole file >>>>>>>>>>>>>>>>>>>");
	};
    fp_borehole = fopen(bhname,"r");
    if(!fp_borehole){
        printf("Borehole file does not exist!");
    }
    else{
        while(!feof(fp_borehole)){
               char str[60];
               if( fgets (str, 60, fp_borehole)!=NULL ){
                    char *str1 = strtok(str, " ");
                    char *depth = strtok(NULL, " ");
                    double lr_depth = atof(depth);
                    char * lr_name = (char*) malloc(40 * sizeof(char));
                    strcpy(lr_name, str1);
                    //printf("%s %f\n", lr_name, lr_depth);
                    name_layers[layercount] = lr_name;
                    layer_depth[layercount] = lr_depth;
                    layercount++;
               }
        }
    }
    fclose(fp_borehole);
    //printf("%s %d\n", "number of layers in the borehole file:", layercount);
    int i = 1;
    int lr_id=0;
    if(depth>layer_depth[0]){
        tmp_result.rho = 0;
        tmp_result.Vp = 0;
        tmp_result.Vs = 0;
        if(DEBUG==1){
           puts("<<<<<<<<<<<< depth greater than last layer >>>>>>>>>>>>");
        }
    }
    else{
        for(i;i<layercount;i++){
            if(depth>layer_depth[i]){
                lr_id = i;
                //printf("%d\n", lr_id);
                //puts("<<<<<<<<< borehole layer determination loop breaks >>>>>>>>");
                break;
            }
            //break;
        }
        int j = 0;
        int layer_number=-1;
        //puts("<<>>>>>>>>>><<<<<<<<<>>>>>>>>>>>");
        for(j;j<5;j++){
            //puts(LAYER_NAMES[j]);
            //puts(name_layers[lr_id]);
            int a = strcmp(LAYER_NAMES[j],name_layers[lr_id]);
            if(a==0){
                layer_number = j;
                break;
            }
            //printf("%s %d\n", "Layer number:", layer_number);

        }
        //puts(LAYER_NAMES[layer_number]);
        if(layer_number<0){layer_number = 5;}
        //printf("%s %d %s %f %s %f %s %f\n", "Layer number:", layer_number, "input depth:", depth, "maximum depth:", layer_depth[lr_id-1], "minimum depth:", layer_depth[lr_id]);
        tmp_result = getlayervalues(LAYERS[layer_number], depth, layer_depth[lr_id-1], layer_depth[lr_id]);
    }

    result.rho = confidence*tmp_result.rho;
    result.Vs = confidence*tmp_result.Vs;
    result.Vp = confidence*tmp_result.Vp;
    return result;

}

double getsurfacedepth(double dlat, double dlong, double diflong, double diflat, double east, double north, char* fname){

	double value;
	double val1;
	double val2;
	double val3;
	double val4;
	long byteval1;
	long byteval2;
	long byteval3;
	long byteval4;
    if (DEBUG==1){
        puts("<<<<<<<<<<<<<<<<<<< loading binary file>>>>>>>>>>>>>>>>>>>");
    }
	fp = fopen(fname, "rb");
	if (DEBUG==1){
        puts("<<<<<<<<<<<<<<<<<<<<<<<<<<binary file loaded>>>>>>>>>>>>>>>>>>>>>>>>>");

	}

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
			printf("l>>>>>>>");
		}
		else if (intervalnumberlatitude==0){
			byteval1 = intervalnumberlongitude*4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			printf("2>>>>>>>");
		}
		else if(intervalnumberlongitude==0){
			byteval1 = intervalnumberlatitude*8000 + 4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			printf("3>>>>>>>");
		}
		else if(intervalnumberlatitude==2000 || intervalnumberlongitude==2000){
			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
			printf("4>>>>>>>");
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
			printf("5>>>>>>>\n");
		}
	}
	fclose(fp);
	//int a;
	//printf("%f >>>>>>> %d\n", value, a);
	//value = value;
	return value;
}
