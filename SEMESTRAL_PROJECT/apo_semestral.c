/*******************************************************************
  Simple program for creating Julia set
:
  (C) Copyright 2004 - 2017 by Ladislav Štefka with great help from Pavel Píša
      e-mail:   stefklad@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "computation.h"
#include "knobs.h"
#include "screen.h"
#include "font_types.h"
#include "netcom.h"

#define NUMOFTHREADS 4
#define MENUICONS 4
//#define minN  0 
//#define maxN 800 
//#define minX  -1.5
//#define maxX  1.5
//#define minY  -1.5
//#define	maxY 1.5




typedef struct{
	bool reprint;
	bool compute;
	bool finished;
	bool menu;
	bool status;
	menuicon icon;
	display* display;
	display* buffer;
	knobs_t* knobs;
	computation parameters;
	bool animation;
}data;
	
	

void* counter(void *);
void* knobschecker (void *);
void* printer(void *);
void* udp (void*);
void* menu(void*);

pthread_mutex_t mtx;
pthread_cond_t condvar;


int main(int argc, char *argv[])
{

	void* (*functptr[]) (void*) ={counter,printer,knobschecker,udp};
	pthread_t threads [NUMOFTHREADS];


	//data state = {.finished = false, .reprint =false, .data=d, .constRe = -0.4, .constIm = 0.6, .xmin = minX, .xmax = maxX, .ymin = minY, .ymax = maxY,.n=minN, .parlcd_mem_base = lcd, .knobs_mem_base = knobs};
	computation parameters = initParameters();
	data state = {.menu = true, .icon = START, .finished = false, .compute = false, .reprint =true, .knobs = initknobs(), .display=initdisplay(), .buffer = initdisplay(), .parameters=parameters};

	
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&condvar,NULL);
	
	for (int i =0; i< NUMOFTHREADS; ++i){
   		pthread_create(&threads[i], NULL, functptr[i],&state);
  	}
   	for (int i =0; i< NUMOFTHREADS; ++i){
   		pthread_join (threads[i], NULL);
   	}

  return 0;
}

void* printer (void* dat){
	data* d = (data*) dat;
	bool quit =false;
	bool inmenu;

	while (!quit){
	
		pthread_mutex_lock(&mtx);
		while (!d->reprint && !quit){
			pthread_cond_wait(&condvar, &mtx);
			
		}
		quit = d->finished;

	

		//writedisplay(d);

		if (d->menu){
			d-> display = showMenu(d->display, d->icon);
			printf ("PARAMETERS: %f %f %d", d->parameters.cR, d->parameters.cI, d->icon);
			printf("INFO: MENU repaint screen request\n");

		}
		else if (d->status){
			d->display = showStatus(d->display, d->parameters.n, d->parameters.cR,d->parameters.cI, d->parameters.aR, d->parameters.aI,d->parameters.bR, d->parameters.bI);
		}

		repaintScreen(d->display);
		
		d->reprint = false;
		pthread_mutex_unlock(&mtx);

	}

	printf("INFO: THREAD PRINTER FINISHED\n");
	return NULL;
}


void* counter(void* dat){

	//not decided yet
	sleep(2);

	//double newR, newI, oldR, oldI;

	data* d = (data*) dat;
	bool quit = false;

	
	while (!quit){
		
		pthread_mutex_lock(&mtx);
		while (!d->compute){
			pthread_cond_wait(&condvar, &mtx);
		}

		

		pthread_mutex_unlock(&mtx);
		if (d->animation){
			bool run = true;
			computation parameters = d->parameters;
			display* img= d->display;

			for (int i = 0; i < 100; i++){
				printf("animation\n");
				pthread_mutex_lock(&mtx);
				img =computeImage(img,parameters);
				d->reprint=true;
				run=d->animation;
				pthread_cond_signal(&condvar);
				pthread_mutex_unlock(&mtx);

				if (!run) break;

				parameters.cR +=(parameters.bR - parameters.aR) *0.001;
				parameters.cI +=(parameters.bI - parameters.aI) *0.001;
				sleep(0.2);
			}
			//go back to menu
			printf("Animation has ended\n");
			pthread_mutex_lock(&mtx);
			d->compute=false;
			d->menu = true;
			d->reprint=true;
			pthread_cond_signal(&condvar);
			pthread_mutex_unlock(&mtx);
		}
		
		else{ 
				
			pthread_mutex_lock(&mtx);
			
			d->display = computeImage(d->display, d->parameters);
			d->reprint = true;
			d->compute = false;
			pthread_cond_signal(&condvar);
			pthread_mutex_unlock(&mtx);
		

			}
		}
		printf ("INFO: THREAD COUNTER finished\n");
		return NULL;

}


void* knobschecker(void* dat){


	
	
	data* d = (data*) dat;
	unsigned char* membase = d-> knobs->knobs_mem_base;
	uint32_t rgb_knobs_value, oldvalue;

 	struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 250 * 1000 * 1000};

	
       //set knobs
       rgb_knobs_value = *(volatile uint32_t*)(membase + SPILED_REG_KNOBS_8BIT_o);
       oldvalue = rgb_knobs_value;
       pthread_mutex_lock(&mtx);
       d->knobs = setInitialvalues(d->knobs, oldvalue);
       printf("INFO: INITIAL KNOBS VALUES: %d %d %d\n",d->knobs->or,d->knobs->og,d->knobs->ob );
       pthread_mutex_unlock(&mtx);



	while (true){
		rgb_knobs_value = *(volatile uint32_t*)(membase + SPILED_REG_KNOBS_8BIT_o);

		if (rgb_knobs_value != oldvalue){
			
			///BASIC APP LOGIC	
			pthread_mutex_lock(&mtx);
			d->knobs = getIncrement(d->knobs, rgb_knobs_value);



			//MENU MODE 
			if (d->menu){
				
				//menu movement
				if (d->knobs->ir>0 || d->knobs->ig >0 || d->knobs->ib >0){
					d->icon = (d->icon+1 < MENUICONS)? d->icon+1: d->icon;
					d->reprint=true;
					pthread_cond_broadcast(&condvar);
				}

				//menu movement
				else if (d->knobs->ir <0 || d->knobs->ig <0 || d->knobs->ib <0){
					d->icon = (d->icon <=0)? 0: d->icon-1;
					d->reprint=true;
					pthread_cond_broadcast(&condvar);
				}

				//menu action
				if (getPUSH(rgb_knobs_value, B)){
					
					switch(d->icon){
						case START:
							d->menu=false;
							d->compute=true;
							d->reprint=true;
							break;
						case CHANGE:
							d->menu=false;
							d->status=true;
							d->reprint=true;
							d->animation=false;
							break;
						case ANIMATION:
							d->menu=false;
							d->compute=true;
							d->animation=true;
							break;
						case QUIT:
							break;
					}

					
					pthread_cond_broadcast(&condvar);
				}
			}
			else if (d->status){
				
				if (getPUSH(rgb_knobs_value, B)){
					
					d-> menu= true;
					d->icon = CHANGE;
					d-> status = false;
					d->reprint = true;
					pthread_cond_broadcast(&condvar);
				}

			}
			else if (d->animation){
				if (getPUSH(rgb_knobs_value, B)){
					d-> menu = true;
					d->animation = false;
					d->icon =ANIMATION;
					d->reprint = true;
					pthread_cond_broadcast(&condvar);
				}
			}

			else{
				//COMPUTE MODE -> GO TO MENU
				if (getPUSH(rgb_knobs_value, B)){
					d->menu = true;
					d->compute =false;
					d->icon = START;
					d->reprint = true;
					pthread_cond_broadcast(&condvar);
					
				}

		

				//COMPUTE MODE -> RECOUNT AND REPRINT
				else{
					
					d->compute = true;

					bool changeButton = getPUSH(rgb_knobs_value, R);

					//constant change
					if (changeButton && d->knobs->ib != 0){
						d->parameters.cR += d->knobs->ib * (d->parameters.bR-d->parameters.aR) *0.0005;
						d->parameters.cI += d->knobs->ib * (d->parameters.bI-d->parameters.aI) *0.0005;

						//d->parameters.cR+= (d->knobs->ib) * 0.0005;
						//d->parameters.cI+= (d->knobs->ib) * 0.0005;
					}
					//iteration change
					else if (changeButton && d->knobs->ig != 0){
						
						d->parameters.n= (d->parameters.n + d->knobs->ig >1)? d->parameters.n + d->knobs->ig : 1;
					}


					//move in x coord
					else if (!changeButton && d->knobs->ib !=0){
						d->parameters.aR += d->knobs->ib * (d->parameters.bR-d->parameters.aR) *0.02;
						d->parameters.bR += d->knobs->ib * (d->parameters.bR-d->parameters.aR) *0.02;
					}
					//move in y coord
					else if (!changeButton && d->knobs->ig !=0){
						d->parameters.aI += d->knobs->ig * (d->parameters.bI-d->parameters.aI) *0.02;
						d->parameters.bI += d->knobs->ig * (d->parameters.bI-d->parameters.aI) *0.02;
					}

					//zoom
					else if (!changeButton && d->knobs->ir !=0){
						d->parameters.aI += d->knobs->ir * 0.005;
						d->parameters.bI -= d->knobs->ir * 0.005;
						d->parameters.aR += d->knobs->ir * 0.005;
						d->parameters.bR -= d->knobs->ir * 0.005;
					}

							
				}

				
			}
			printf("INFO: OLD RGB VALUES: %d %d %d\n",d->knobs->or,d->knobs->og,d->knobs->ob );
			printf("INFO: INCREMENTS: %d %d %d  \n", d->knobs->ir,d->knobs->ig,d->knobs->ib);

			
			pthread_cond_broadcast(&condvar);
			pthread_mutex_unlock(&mtx);

					//printf("INFO: CALCULATED ZOOM: %f %f %f %f\n", d->knobs->xmin, d->knobs->xmax, d->knobs->ymin, d->knobs->ymax);
			oldvalue = rgb_knobs_value;
		}

			//pthread_mutex_lock(&mtx);
				
 		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
		

 
	}
	return NULL;
	
}
void* udp (void* dat){
	data* d = (data*) dat;
	int fd =initCommunication();
	bool quit = false;
	char* add = (char*)malloc(50);
	char* buff = (char*) malloc(256);
	int len;
	while (!quit){
		printf("waiting for bytes..\n");
		printf("waiting for parameter id\n");
		len =receiveBytes(fd, buff, add);
		printf("%s\n", buff);

		if (buff [0] == 'C' && buff[1]=='R'){
			printf("constant real selected\n");
			len = receiveBytes(fd, buff, add);
			double c = atof(buff);
			printf("par changed cR= %f\n",c);

			pthread_mutex_lock(&mtx);
			d->parameters.cR= c;
			d->reprint = true;
			pthread_cond_broadcast(&condvar);
			pthread_mutex_unlock(&mtx);

			}
		else if (buff [0] == 'C' && buff[1]=='I'){
			printf("constant imaginary selected\n");
			len = receiveBytes(fd, buff, add);
			double c = atof(buff);
			printf("par changed cI= %f\n",c);

			pthread_mutex_lock(&mtx);
			d->parameters.cI= c;
			d->reprint = true;
			pthread_cond_broadcast(&condvar);
			pthread_mutex_unlock(&mtx);
			}
		else if (buff [0] == 'N'){
			printf ("iterations selected\n");
			len = receiveBytes(fd, buff, add);
			int n = atoi(buff);
			printf("par changed N= %d\n",n);

			pthread_mutex_lock(&mtx);
			d->parameters.n=n;
			d->reprint = true;
			pthread_cond_broadcast(&condvar);
			pthread_mutex_unlock(&mtx);
		}
		else {
			printf("parameter was not recoginsed\n");
		}

	}
	

	return NULL;
}




