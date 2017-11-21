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

	float * output;
	float result[3];

	output= getdepth(24.41, 90.58, 14000);
	result[0] = *(output+0);
	result[1] = *(output+1);
	result[2] = *(output+2);


	for(int i=0; i<3; i++){
		printf("%f\n",result[i]);
	}

}
