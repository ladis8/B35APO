#ifndef COMPUTATION_H_
#define COMPUTATION_H_


#include "screen.h"


#define def_bR 0.5
#define def_bI 0.5
#define def_aR -0.5
#define def_aI -0.5
#define def_N 60
#define def_cR -0.4
#define def_cI 0.6


typedef enum{
	NO_PAR,
	C_RE,
	C_IM,
	N,
	A_RE,
	A_IM,
	B_RE,
	B_IM,
} param;

typedef struct {
	int n;
      	double cR, cI;
	double aR, aI, bR, bI;
	double inR, inI;
	param parameter_to_be_changed;
}computation;


int compute (double cR, double cX, double pR, double pI, int n);
display* computeImage(display* img, computation parameters);
computation initParameters();

#endif


