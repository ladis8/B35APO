#ifndef SCREEN_H_
#define SCREEN_H_

#include "knobs.h"


#define HEIGHT 320 
#define WIDTH 480 
#define RGB_MAX 255

#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255

//unsigned char* initScreen();

typedef enum{
	START,
	CHANGE,
	ANIMATION,
	QUIT,
} menuicon;

typedef enum{
	CONASTANT,
	ITERATIONS,
	BACK,
} menu2icon;

typedef struct {
	uint8_t r;
	uint8_t g; 
	uint8_t b;
} pix;



typedef struct {
	int H;	//display height
	int W;	//display width
	pix** data;
	unsigned char* parlcd_mem_base;
} display;

uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b);

void repaintScreen(display* display);

void showScreenPPM (display* display);

display* initdisplay();



//Image* createTextScreen(int posx, int posy, char* text);

display* setBlackScreen(display* dis);

display* writeText(display* dis, int x, int y, char* text);

display* setPixel(display* dis, int x, int y, uint8_t r, uint8_t g, uint8_t b);

display* writeLetter(display* dis, int x, int y, unsigned char c);

//display* showAnimation(display* dis, computation parameters);

display* showStatus (display* dis, int n, double cR, double cI, double aR, double aI, double bR, double bI);

display* showMenu (display* dis, menuicon icon);

#endif
