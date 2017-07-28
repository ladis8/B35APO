#define NUMOFTHREADS 2

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

#define HEIGHT 320
#define WIDTH 480

typedef struct{
	bool reprint;
	bool finished;
	//uint32_t* data;
	uint8_t** data;
	double constRe;
	double constIm;
	int n;
}data;
	
	

void* counter(void *);
void* printer(void *);


pthread_mutex_t mtx;
pthread_cond_t condvar;

int main(int argc, char *argv[])
{
  printf("Hello world\n");
  sleep(2);
  printf("Goodbye world\n");


	void* (*functptr[]) (void*) ={counter,printer};
	pthread_t threads [NUMOFTHREADS];
//	uint32_t* data = (uint32_t*) malloc(HEIGHT * WIDTH *sizeof(uint32_t));
	uint8_t** d = (uint8_t**) malloc(HEIGHT * sizeof(uint8_t*));
	for (int i=0;i< HEIGHT;i++){
		d[i]= (uint8_t*) malloc(WIDTH * sizeof(uint8_t) *3);
		for (int j=0; j<WIDTH*3; j++){
			d[i][j] = 0;
		}
	}

<<<<<<< HEAD
	data state = {.finished = false, .reprint =false, .data=d, .constRe = -0.4, .constIm = 0.6, .n=2000};
=======
	data state = {.finished = false, .reprint =true, .data=d, .constRe = -0.4, .constIm = 0.6, .n=60};
>>>>>>> b55dc0f32daecbe034bf62068a9b5ae4c520352d

	
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

void* printer (void*dat){
	data* d = (data*) dat;
	bool quit =false;
<<<<<<< HEAD
	//sleep(5);

	while (!quit){
	
		pthread_mutex_lock(&mtx);
		while (!d->reprint && !quit){
			pthread_cond_wait(&condvar, &mtx);
			quit = d->finished;
		}

		int fd = open("juliaset.ppm",O_CREAT|O_TRUNC | O_WRONLY,0777);
		write (fd, "P6\n480 320\n255\n",15);
		printf("wait\n");

		for (int i=0; i < HEIGHT; i++)
			write(fd, d->data[i],3*WIDTH);
		
		d->reprint = false;
		close(fd);
		pthread_mutex_unlock(&mtx);

	}
=======

	
	pthread_mutex_lock(&mtx);
	pthread_cond_wait(&condvar, &mtx);
	int fd = open("juliaset.ppm",O_CREAT|O_TRUNC | O_WRONLY,0777);
	write (fd, "P6\n480 320\n255\n",15);

	while (!quit){
		printf("reprint all\n");

		//pthread_mutex_lock(&mtx);
		quit = d->finished;
		for (int i=0; i < HEIGHT; i++){
			write(fd, d->data[i],3*WIDTH);
		}
		printf("wait\n");
			//pthread_cond_wait(&condvar, &mtx);
		pthread_mutex_unlock(&mtx);
	}
	close(fd);
>>>>>>> b55dc0f32daecbe034bf62068a9b5ae4c520352d

	printf("THREAD PRINTER FINISHED\n");
}

void* counter(void* dat){

	//not decided yet
	sleep(2);

	double xmin = -1.5;
	double xmax = 1.5;
	double ymin = -1.5;
	double ymax = 1.5;
	double newR, newI, oldR, oldI;




	data* d = (data*) dat;
	bool done = false;

	pthread_mutex_lock(&mtx);
	double cR = d->constRe;
	double cI = d->constIm;
	int n = d->n;	
	pthread_mutex_unlock(&mtx);
	
	while (!done){
		for (int x = 0; x < HEIGHT; x++){

		uint8_t* row = malloc(sizeof(uint8_t)*WIDTH *3);
		for (int y  =0; y < WIDTH; y++){
			int counter =0;	

			newR= (double)x/HEIGHT * (xmax - xmin) + xmin;
			newI= (double)y/ WIDTH* (ymax - ymin) +ymin;

			while (newR*newR + newI *newI < 4 && counter < n){
				oldR = newR;
				oldI = newI;
				newR = oldR *oldR - oldI* oldI + cR;
				newI = 2* oldR *oldI + cI;
				counter++;
			}
			double t = (double) counter/(double)n;
			double R = 9 * (1-t) * t*t*t *255;
			double G = 15 * (1-t) * (1-t) * t*t *255;
			double B = 8.5 * (1-t) * (1-t) * (1-t) * t *255;
			row [y*3] = (uint8_t) R;
			row[y*3+1] = (uint8_t)G;
			row [y*3+2] =(uint8_t)B;

		}       
		pthread_mutex_lock(&mtx);
<<<<<<< HEAD
		sleep(1);
		free(d->data[x]);
		d->data[x] = row;
		d->reprint = true;
		pthread_cond_signal(&condvar);
=======
		free(d->data[x]);
		d->data[x] = row;
		//pthread_cond_signal(&condvar);
>>>>>>> b55dc0f32daecbe034bf62068a9b5ae4c520352d
		printf ("signal\n");
		pthread_mutex_unlock(&mtx);

	        }
	pthread_mutex_lock(&mtx);
	d->finished = true;
	pthread_cond_signal(&condvar);
	done = true;
	pthread_mutex_unlock(&mtx);
	}
	printf ("THREAD COUNTER finished\n");

}
		

