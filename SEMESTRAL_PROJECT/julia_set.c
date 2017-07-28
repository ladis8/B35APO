#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#define WIDTH 480  
#define HEIGHT 380
#define n 60 


///var
	double constx = -0.4;
	double consty = 0.6;
	
	double xmin = -0.5;
	double xmax = 0.5;
	double ymin = -0.5;
	double ymax = 0.5;


double mapx (int x){
	return ((double)x)/HEIGHT * (xmax - xmin) + xmin;
}
double mapy (int y){
	return ((double)y)/ WIDTH* (ymax - ymin) +ymin;
}

uint8_t picture [HEIGHT][WIDTH] [3];

void printpicture(){


	int fd = open("juliaset.ppm",O_CREAT|O_TRUNC | O_WRONLY,0777);
//	FILE* f = fopen("juliaset.ppm","w");
	
	write (fd, "P6\n480 320\n255\n",15);
	//fprintf (f, "P3\n400 400\n255\n");

	write(fd, picture, 3* HEIGHT*WIDTH);
	
	/*for (int x = 0; x < WIDTH; x++){
		for (int y  =0; y < HEIGHT; y++){
			fprintf(f,"%d %d %d ", picture[x][y][0], picture[x][y][1], picture[x][y][2]);
			
		}
		fprintf(f,"\n");
	}*/
		

	close(fd);
	//fclose(f);
}


int main(){

	int counter = 0;
	//double mappedx, mappedy;
	double newR, newI, oldR, oldI;
	
	for (int x = 0; x < HEIGHT; x++){
		for (int y  =0; y < WIDTH; y++){
			counter =0;	

			newR = mapx(x);
			newI= mapy(y);
			//mappedx = mapx(x);
			//mappedy = mapy(y);

			while (newR*newR + newI *newI < 4 && counter < n){
				oldR = newR;
				oldI = newI;
				newR = oldR *oldR - oldI* oldI + constx;
				newI = 2* oldR *oldI + consty;
				counter++;
			}
			
			
			/*while ((mappedx * mappedx + mappedy*mappedy) < 4 && counter < n){
				mappedx = mappedx * mappedx + constx;
				mappedy = mappedy * mappedy + consty;
				counter++;
			}*/

			

			double t = (double) counter/(double)n;
			printf ("%f\n",t );
			double R = 9 * (1-t) * t*t*t *255;
			double G = 15 * (1-t) * (1-t) * t*t *255;
			double B = 8.5 * (1-t) * (1-t) * (1-t) * t *255;
			picture [x][y][0] = (uint8_t) R;
			picture [x][y][1] = (uint8_t) G;
			picture [x][y][2] = (uint8_t) B;
			printf ("%f %f %f\n",R,G, B);


		}       
	}
	printpicture();

	return 0;
}
