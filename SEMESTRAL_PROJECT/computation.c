#include "computation.h"
#include "screen.h"


computation initParameters(){
	computation parameters = { .n=def_N, .cR=def_cR, .cI=def_cI, .aR=def_aR, .aI=def_aI, .bR=def_bR, .bI=def_bI, .parameter_to_be_changed=NO_PAR};
	return parameters;
}








//function that comptues the image
display* computeImage(display* img, computation parameters){

	parameters.inR = (parameters.bR - parameters.aR)/img->W;
	parameters.inI = (parameters.bI - parameters.aI)/img->H;

	for (int x=img->H-1; x >= 0; x--){
		for (int y=0; y < img->W; y++){
			int iter= compute(parameters.cR, parameters.cI, parameters.aR + y*parameters.inR, parameters.aI + x* parameters.inI, parameters.n);
			double t = (double) iter/(double)parameters.n;
			double R = 9 * (1-t) * t*t*t *255;
			double G = 15 * (1-t) * (1-t) * t*t *255;
			double B = 8.5 * (1-t) * (1-t) * (1-t) * t *255;
			//img->data[x* img->W +y] = (pix){.r=(int8_t) R, .g=(uint8_t) G, .b=(uint8_t) B};
			img->data[x][y] = (pix){.r=(uint8_t) R, .g=(uint8_t) G, .b=(uint8_t) B};
		}
	}
	return img;
}

int compute(double cR, double cI, double px, double py, int n){

	double newR,newI, oldR,oldI;
	int counter =0;	
	newR= px; newI= py;

	while (newR*newR + newI *newI < 4 && counter < n){
		oldR = newR;
		oldI = newI;
		newR = oldR *oldR - oldI* oldI + cR;
		newI = 2* oldR *oldI + cI;
		counter++;
	}
	//printf("Compute pixel: %f %f %f %f %u %u \n", px,py,cR,cI,counter,n);
	return counter;
}



