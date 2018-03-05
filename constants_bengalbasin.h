#ifndef CONSTANTS_BENGALBASIN_H_
#define CONSTANTS_BENGALBASIN_H_
#include<stdio.h>
#include "filenames.h"

double MINLAT = 19.9; // minimum latitude of the extent of simulation region in degree decimal
double MAXLAT = 27; // maximum latitude of the extent of simulation region in degree decimal
double MINLONG = 86.5; // minimum longitude of the extent of simulation region in degree decimal
double MAXLONG = 93.4; // maximum longitude of the extent of simulation region in degree decimal

int GRIDSTEP = 2000; // grid step of the binary files generated from the contours
double DEGREETODIST = 111.01*1000; // degree to distance multiplier in meters

double SEDIMENTDEPTH = 100; // fixed sediment depth assumed for the model in meters
double APPROXCRUSTTHICKNESS = 20000; // average approximated crustal thickness

typedef struct layer_params {
    double minvs, maxvs, minrho, maxrho, vpvsratio;
} layer_params;

const char * LAYER_NAMES[] = {
    "sediment",
    "dupitila",
    "tipam",
    "bokabil",
    "bhuban",
    "premiocene",
    "precambrian",
    "mantle",
};
int LAYER_COUNT = 8;
layer_params LAYERS[LAYER_COUNT];

LAYERS[0].minvs = 100; // minimum shear wave velocity of the quaternary sediment in meter/sec
double SEDMAXVS = 500; // maximum shear wave velocity of the quaternary sediment in meter/sec
double SEDMINDENSITY = 2000; // minimum density of the quaternary sediment in kg/m3
double SEDMAXDENSITY = 2050; // maximum density of the quaternary sediment in kg/m3
double SEDVPVSRATIO = 2.2; // ratio between P-wave and S-wave velocity Dupitila deposit

double DUPITILAMINVS = 750; // minimum shear wave velocity of the Dupitila deposit in meter/sec
double DUPITILAMAXVS = 1000; // maximum shear wave velocity of the Dupitila deposit in meter/sec
double DUPITILAMINDENSITY = 2100; // minimum density of the Dupitila deposit in kg/m3
double DUPITILAMAXDENSITY = 2200; // maximum density of the Dupitila deposit in kg/m3
double DUPITILAVPVSRATIO = 2.1; // ratio between P-wave and S-wave velocity of the Dupitila deposit

double TIPAMMINVS = 1050; // minimum shear wave velocity of the Tipam deposit in meter/sec
double TIPAMMAXVS = 1100; // maximum shear wave velocity of the Tipam deposit in meter/sec
double TIPAMMINDENSITY = 2200; // minimum density of the Tipam deposit in kg/m3
double TIPAMMAXDENSITY = 2350; // maximum density of the Tipam deposit in kg/m3
double TIPAMVPVSRATIO = 2.1; // ratio between P-wave and S-wave velocity of the Tipam deposit

double BOKABILMINVS = 1250; // minimum shear wave velocity of the Bokabil deposit in meter/sec
double BOKABILMAXVS = 1650; // maximum shear wave velocity of the Bokabil deposit in meter/sec
double BOKABILMINDENSITY = 2350; // minimum density of the Bokabil deposit in kg/m3
double BOKABILMAXDENSITY = 2450; // maximum density of the Bokabil deposit in kg/m3
double BOKABILVPVSRATIO = 2.0; // ratio between P-wave and S-wave velocity of the Bokabil deposit

double BHUBANMINVS = 1700; // minimum shear wave velocity of the Bhuban deposit in meter/sec
double BHUBANMAXVS = 2200; // maximum shear wave velocity of the Bhuban deposit in meter/sec
double BHUBANMINDENSITY = 2450; // minimum density of the Bhuban deposit in kg/m3
double BHUBANMAXDENSITY = 2550; // maximum density of the Bhuban deposit in kg/m3
double BHUBANVPVSRATIO = 2.0; // ratio between P-wave and S-wave velocity of the Bhuban deposit

double PREMIOCENEMINVS = 2250; // minimum shear wave velocity of the PreMiocene deposit in meter/sec
double PREMIOCENEMAXVS = 2400; // maximum shear wave velocity of the PreMiocene deposit in meter/sec
double PREMIOCENEMINDENSITY = 2550; // minimum density of the PreMiocene deposit in kg/m3
double PREMIOCENEMAXDENSITY = 2600; // maximum density of the PreMiocene deposit in kg/m3
double PREMIOCENEVPVSRATIO = 1.9; // ratio between P-wave and S-wave velocity of the PreMiocene deposit

double PRECAMBRIANMINVS = 3250; // minimum shear wave velocity of the PreCambrian deposit in meter/sec
double PRECAMBRIANMAXVS = 3500; // maximum shear wave velocity of the PreCambrian deposit in meter/sec
double PRECAMBRIANMINDENSITY = 2670; // minimum density of the PreCambrian deposit in kg/m3
double PRECAMBRIANMAXDENSITY = 2800; // maximum density of the PreCambrian deposit in kg/m3
double PRECAMBRIANVPVSRATIO = 1.8; // ratio between P-wave and S-wave velocity of the PreCambrian deposit

double MANTLEMINVS = 4500; // minimum shear wave velocity of the Mantle in meter/sec
double MANTLEMAXVS = 6000; // maximum shear wave velocity of the Mantle in meter/sec
double MANTLEDENSITY = 3300; // density of the Mantle in kg/m3
double MANTLEVPVSRATIO = 1.7; // ratio between P-wave and S-wave velocity of the Mantle
FILE* fp;
int DEBUG = 0;

//typedef struct loc {
//    double laty, longx;
//} loc;
//
//loc * MINLOC = (loc*) malloc(2 * sizeof(loc));
//MINLOC.laty = MINLAT;
//MINLOC.longx = MINLONG;
//
//loc * loc_dif(loc * p1, loc * p2){
//    loc * p3 = (loc*) malloc(2 * sizeof(loc));
//    p3.laty = p1.laty - p2.laty;
//    p3.longx = p1.longx - p2.longx;
//    return p3;
//
//}

#endif // CONSTANTS_BENGALBASIN_H_
