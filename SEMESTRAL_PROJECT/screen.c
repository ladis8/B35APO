#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "screen.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "knobs.h"



void showScreenPPM(display* d){
	FILE* fd = fopen("tempscreen.ppm", "wb");
	if(!fd) exit(1);

	fprintf(fd, "P6 %d %d %d\n", d->H, d->W, RGB_MAX);
	for (int i = 0; i < d->H; i++)
		fwrite(d->data[i], 3, d->W, fd);
	fclose(fd);
}




uint16_t rgbTo16 (uint8_t r, uint8_t g, uint8_t b){
	uint8_t nr = (r & 0b11111000)>>3;
	uint8_t ng = (g & 0b11111100)>>2;
	uint8_t nb = (b & 0b11111100)>>2;
	uint16_t con = (nr<<11) | (ng<<5)|nb;
	return con;
}
void repaintScreen(display* d){
	parlcd_write_cmd(d->parlcd_mem_base, 0x2c);
    	for (int x=0; x < d->H; x++){
         	for (int y=0; y< d->W; y++){
                 	parlcd_write_data(d->parlcd_mem_base, rgbTo16(d->data[x][y].r,d->data[x][y].g,d->data[x][y].b));
         }
    }
}





display* initdisplay(){

	//uint16_t** d = (uint16_t**) malloc(HEIGHT * sizeof(uint16_t*));
	pix ** d = (pix**) malloc(HEIGHT * sizeof (pix*));
	for (int i=0;i< HEIGHT;i++){
		//d[i]= (uint16_t*) malloc(WIDTH * sizeof(uint16_t));
		d[i] = (pix*) malloc(WIDTH* sizeof(pix));

		for (int j=0; j<WIDTH; j++){
			//d[i][j] = 0;
			d[i][j] = (pix) {.r=0,.g=0,.b=0};
		}
	}
	
	///map LCD memory base
	unsigned char *lcd;
	lcd = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  	if (lcd == NULL) exit(1);

	display* dis =(display*)malloc(sizeof(display));
	dis->data = d; dis->H = HEIGHT; dis->W = WIDTH; dis->parlcd_mem_base=lcd;
	return dis;
}


display* setBlackScreen(display* d){
	for (int x = 0; x< d->H;x++){
		for (int y=0; y< d->W; y++){
			d->data[x][y] = (pix) {.r=0,.g=0, .b=0};
		}
	}
	return d;
}


display* setPixel(display* d, int x, int y, uint8_t r, uint8_t g, uint8_t b){
	if(!d) exit(1);
	
	if(x > d->H|| x<0|| y> d->W|| y<0){
		fprintf(stderr, "ERROR: Wrong dim - setPixel()\n");
		return d;
	}
	
	d->data[x][y].r = r;
	d->data[x][y].g = g;
	d->data[x][y].b = b;
	
	return d;
}
display* writeLetter(display* d, int x, int y, unsigned char c){

	int offset = ((int)c) * 16;
	
	for(int i = 0; i< font_rom8x16.height;i++){

		uint16_t line = font_rom8x16.bits[offset+i];	
			
		for(int j = 0;j<font_rom8x16.maxwidth;j++){
			uint16_t mask = 1;
			mask = mask<<15;
			mask = mask>>j;
			if((line&mask)){
				setPixel(d, x+i, y+j, TEXT_R, TEXT_G, TEXT_B);
			}
		}
	}
	
	
	return d;
}


display* writeText(display* d, int posx, int posy, char* text){
	int length = strlen(text);
	int x = posx;	
	int y = posy;

	for(int i = 0; i<length;i++){
		
		if( (int)text[i] == 10){
			x += font_rom8x16.height;
			y = posy;
			continue;
		}
		
		writeLetter(d, x, y, text[i]);
		y += font_rom8x16.maxwidth;

	}
	return d;
}



display* showStatus (display* d, int n,double cR, double cI, double aR, double aI, double bR, double bI){

	setBlackScreen(d);
	int offset = font_rom8x16.height;
	//needs normalize
	int padding = 50;
	char buffer [200];


	writeText (d,0*offset, padding, "STATUS OF THE COMPUTATION");
	sprintf(buffer,"N: %d", n);
	writeText(d, 1*offset, padding, buffer); 
	sprintf(buffer, "C_RE: %f C_IM: %f", cR, cI);
	writeText(d, 2*offset, padding,buffer ); 
	sprintf(buffer,"xmin: %f xmax: %f",aR, aI);
	writeText(d, 3*offset, padding,buffer ); 
	sprintf(buffer,"ymin: %f ymax: %f",bR, bI);
	writeText(d, 4*offset, padding, buffer); 
	return d;
	
}


display* showMenu(display* d, menuicon icon){
	//set black screen
	d = setBlackScreen(d);
	int offset = font_rom8x16.height;
	//needs normalize
	int padding = 50;

	//set cursor
	for(int x = font_rom8x16.height * (icon+1); x< font_rom8x16.height * (icon+2);x++){
		for(int y = 0;y< d->W ;y++){
			d = setPixel(d, x, y, 0, 0 ,255);
		}
	}

	writeText (d,0*offset, padding, "APO SEMESTRAL PROJECT MENU");
	writeText (d,1*offset, padding, "START"); 
	writeText (d,2*offset, padding, "SHOW STATUS"); 
	writeText (d,3*offset, padding, "ANIMATION"); 
	writeText (d,4*offset, padding, "QUIT"); 
	return d;
}

/*display* showMenu2(display* d, menu2icon icon){

	d = setBlackScreen(d);
	int offset = font_rom8x16.height;
	//needs normalize
	int padding = 50;
	char buffer [200];


	//set cursor
	for(int x = font_rom8x16.height * (icon+1); x< font_rom8x16.height * (icon+2);x++){
		for(int y = 0;y< d->W ;y++){
			d = setPixel(d, x, y, 0, 0 ,255);
		}
	}

	writeText (d,0*offset, padding, "CHANGING CONSTANT MENU");
	writeText (d,1*offset, padding, "CHANGE VALUE BY KNOBS"); 
	writeText (d,2*offset, padding, "CONSTANT"); 
	writeText (d,3*offset, padding, "ITERATIONS"); 
	writeText (d,4*offset, padding, "BACK"); 

	sprintf(buffer,"N: %d", parameters.n);
	writeText(d, 1*offset, padding, buffer); 
	sprintf(buffer, "C_RE: %f C_IM: %f", parameters.cR, parameters.cI);
	writeText(d, 2*offset, padding,buffer ); 
	return d;
}
*/






