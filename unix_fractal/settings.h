/*
 * settings.h
 * --------
 * By Mark Garro and Shammah Chancellor
 * Date: September 06, 2007
 */

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <vector>
#include <cstdlib>
#include <math.h>
#include <time.h>

typedef float GLfloat;
typedef double GLdouble;

//Local Headers

#include "Vector_2.h"
#include "Vector_3.h"
#include "LinRegression.h"

// Program Parameters
const double MEAN_RADIUS = 1.0;
const double EPSILON = 0.05;
const double FRACTAL_DIMENSION = 1.82;
const double PREFACTOR = 1.19;
const int MAX_MONOMER_TRIES = 1000;
const int PRECISION = 40;
const int NUM_COLORS = 8;

const double PI = 3.14159;

//const int CORRELATION_PRECISION = 500;
const int PARTS_PER_RADIUS = 10;
//const double SHELL_RADIUS = 2.0 * MEAN_RADIUS / PARTS_PER_RADIUS;
//const double CORRELATION_INCR = MEAN_RADIUS / PARTS_PER_RADIUS / 2;
const double FILTER_DEPTH = MEAN_RADIUS;
#endif
