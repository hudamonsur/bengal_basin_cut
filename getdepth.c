/* finding depth program */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "getdepth.h"

float * getmantlevalues(float inputdepth, float mindepth);
float * getcrustvalues(float inputdepth, float mindepth);
float * getbasinvalues(float inputdepth, float maxdepth);

float * getdepth(float inputlat, float inputlong, float inputdepth)
{
	float result[3];
	FILE *fp=NULL;
	float minlat= 19.9;
	float maxlat= 27;
	float minlong= 86.5;
	float maxlong= 93.4;
	float value;
	float val1;
	float val2;
	float val3;
	float val4;
	float dlat = (maxlat-minlat)/2000;
	float dlong = (maxlong-minlong)/2000;
	long byteval1;
	long byteval2;
	long byteval3;
	long byteval4;

	/*long bytevalue;*/


	/*float inputlat = 23.8;
	float inputlong = 87.45;*/
	float diflat = inputlat-minlat;
	float diflong = inputlong-minlong;

	fp=fopen("depth.bin","rb");
	if (!fp)
	{
		printf("Unable to open file!");
		return 0;
	}

	if(inputlat>maxlat || inputlat<minlat || inputlong<minlong || inputlong>maxlong){
		printf("The co-ordinate is out of the region");
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
		}

		else if (intervalnumberlatitude==0){
			byteval1 = intervalnumberlongitude*4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
		}

		else if(intervalnumberlongitude==0){
			byteval1 = intervalnumberlatitude*8000 + 4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
		}

		else if(intervalnumberlatitude==2000 || intervalnumberlongitude==2000){
			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
			fseek(fp, byteval1, SEEK_SET);
			fread(&value, 4, 1, fp);
		}

		else {
			byteval1 = intervalnumberlatitude*8000 + intervalnumberlongitude*4;
			byteval2 = byteval1 + 8000;
			byteval3 = byteval2 - 4;
			byteval4 = byteval1 - 4;
			float lat1 = intervalnumberlatitude*dlat+minlat;
			float lat2 = lat1 + dlat;
			float long1 = intervalnumberlongitude*dlong+minlong;
			float long2 = long1 - dlong;

			/* seeking values */
			fseek(fp, byteval1, SEEK_SET);
			fread(&val1, 4, 1, fp);
			fseek(fp, byteval2, SEEK_SET);
			fread(&val2, 4, 1, fp);
			fseek(fp, byteval3, SEEK_SET);
			fread(&val3, 4, 1, fp);
			fseek(fp, byteval4, SEEK_SET);
			fread(&val4, 4, 1, fp);


			float R1 = ((long2-inputlong)/(long2-long1))*val1 + ((inputlong-long1)/(long2-long1))*val2;
			float R2 = ((long2-inputlong)/(long2-long1))*val4 + ((inputlong-long1)/(long2-long1))*val3;

			value = ((lat2-inputlat)/(lat2-lat1))*R1 + ((inputlat-lat1)/(lat2-lat1))*R2;
		}

	}
	fclose(fp);


	/*fseek(fp, 11576002, SEEK_SET);
	fread(&value, 4, 1, fp);*/

	/*printf("%f",value);*/
	float * output;
	if (inputdepth>(value*-1)+20000){
		output = getmantlevalues(inputdepth, value);
		result[0] = *(output+0);
		result[1] = *(output+1);
		result[2] = *(output+2);

	}
		else if (inputdepth>(value*-1) && inputdepth<=(value*-1)+20000){
		output = getcrustvalues(inputdepth, value);
		result[0] = *(output+0);
		result[1] = *(output+1);
		result[2] = *(output+2);

		}
		else {
		//result = getbasinvalues(inputdepth, value);
		output = getbasinvalues(inputdepth, value);
		result[0] = *(output+0);
		result[1] = *(output+1);
		result[2] = *(output+2);

		}
	return result;
	fclose(output);
	//printf("%f",value);
	//free(fp);

}

float * getbasinvalues(float inputdepth, float maxdepth){

	float result[3];
	float minvs = 500;
	float maxvs = 2000;
	float mindensity = 2400;
	float maxdensity = 2800;
	float vs;
	float density;
	float vp;

	float mindepth = 0;
	if (maxdepth==0){
		vs = minvs;
		density = mindensity;
		vp = vs*2.2;
	}
	else{
		maxdepth = maxdepth*-1;
		vs = (minvs+((maxvs-minvs)*(inputdepth-mindepth))/(maxdepth-mindepth));
		vp = 2.2*vs;
		density = (mindensity+((maxdensity-mindensity)*(inputdepth-mindepth))/(maxdepth-mindepth));
	}

	result[0]=vs;
	result[1]=vp;
	result[2]=density;

	return result;
}

float * getcrustvalues(float inputdepth, float mindepth){

	float result[3];
	float minvs = 2700;
	float maxvs = 4200;
	float mindensity = 2900;
	float maxdensity = 3100;
	float vs;
	float density;
	float vp;

	mindepth = mindepth*-1;

	float maxdepth = mindepth+20000;
	vs = (minvs+((maxvs-minvs)*(inputdepth-mindepth))/(maxdepth-mindepth));
	vp = 1.8*vs;
	density = (mindensity+((maxdensity-mindensity)*(inputdepth-mindepth))/(maxdepth-mindepth));

	result[0]=vs;
	result[1]=vp;
	result[2]=density;

	return result;
}

float * getmantlevalues(float inputdepth, float mindepth){

	float result[3];
	float minvs = 4500;
	float maxvs = 6500;
	float vs;
	float vp;
	float density;

	mindepth = mindepth*-1 + 20000;
	float maxdepth = 60000;
	vs = (minvs+((maxvs-minvs)*(inputdepth-mindepth))/(maxdepth-mindepth));
	vp = 1.8*vs;
	density = 3300;

	result[0]=vs;
	result[1]=vp;
	result[2]=density;

	return result;
}




