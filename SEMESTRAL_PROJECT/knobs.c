
#include "knobs.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

knobs_t* initknobs(){
	unsigned char* knobs;
	knobs = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  	if (knobs == NULL) exit(1);

	knobs_t* k = (knobs_t*)malloc(sizeof(knobs_t));
	//knobs_t k = {.knobs_mem_base = knobs, .constRe=defaultCRe, .constIm=defaultCIm, .xmin=minX, .xmax=maxX, .ymin=minY, .ymax=maxY, .n=minN};
	k->knobs_mem_base= knobs; 
	//k->constRe=defaultCRe; k->constIm=defaultCIm; k->xmin=minX; k->xmax=maxX; k->ymin=minY; k->ymax=maxY; k->n=minN;

	return k;
}

knobs_t* setInitialvalues(knobs_t* k, uint32_t knobs){
			k->or= (knobs & 0x00FF0000)>>16; 
			k->og= 	(knobs & 0x0000FF00)>>8;
			k->ob= 	(knobs & 0x000000FF);
			return k;
}

knobs_t* getIncrement (knobs_t* k, uint32_t knobs){
		k->nr =	(knobs & 0x00FF0000)>>16; 
		k->ir = (k->nr - k->or);
		k->or += k->ir;

		k->ng= 	(knobs & 0x0000FF00)>>8;
		k->ig = (k->ng - k->og);
		k->og += k->ig;
		
		k->nb= 	(knobs & 0x000000FF);
		k->ib = (k->nb - k->ob);
		k->ob += k->ib;
		return k;
}




bool getPUSH (uint32_t knobs, rgb col){
	switch(col){
		case R:
			return (knobs & 0x04000000)>>26;
		case G:
			return (knobs & 0x02000000)>>25;
		case B:
			return (knobs & 0x01000000)>>24;
		default:
			return 0;
	
	}
}
	



