#ifndef KNOBS_H_
#define KNOBS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum{
	R,
	G,
	B,
}rgb;

typedef struct{
	unsigned char* knobs_mem_base;
	uint8_t nr, ng, nb;
	uint8_t or, og, ob;
	int8_t ir, ig, ib;
	

	
}knobs_t;

knobs_t* initknobs();

uint8_t getRGB(uint32_t knobs, char rgb);
bool 	getPUSH(uint32_t knobs, rgb col);


knobs_t* getIncrement (knobs_t* k, uint32_t knobs);
knobs_t* setInitialvalues(knobs_t* k, uint32_t knobs);


#endif
