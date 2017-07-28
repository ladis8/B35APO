#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct pixel{
	unsigned char R ;
	unsigned char G ;
	unsigned char B;
}pixel;




void printpicture(pixel* pixelarray, int m, int n){
	for (int i= 0; i < m; i++){
		for (int j=0; j< n; j++){
			pixel pix = *(pixelarray +i*n+j);
			printf("%3u%3u%3u    ",pix.R,pix.G, pix.B);
		}
		printf("\n");
	}
	printf("\n\n");
}
void countgreyscale(pixel pix,int histogram[5]){
	//int greyscale = round(0.2126*pix.R +0.7152*pix.G + 0.0722*pix.B);
	int greyscale = (2126*pix.R + 7152*pix.G + 722*pix.B);
	greyscale/=1000;

	if (greyscale % 10 >4)
		greyscale+=5;
	greyscale/=10;

	int c;
	if (greyscale < 51) c=0;
	else if (greyscale<102) c=1;
	else if (greyscale<153) c=2;
	else if (greyscale<204) c=3;
	else c=4;
	histogram[c]++;
}

void getoutput(pixel* row1, pixel* row2, pixel* row3,pixel** out, int column,int histogram[5]){
	int r,g,b;
	pixel pix;
	for(int i=0; i< column; ++i){
		if (i !=0 && i != column-1){

		r = 5* (*(row2 +i)).R -(*(row2 +i -1)).R - (*(row2 +i+1)).R;
		g = 5* (*(row2 +i)).G -(*(row2 +i -1)).G - (*(row2 +i+1)).G;
		b = 5* (*(row2 +i)).B -(*(row2 +i -1)).B - (*(row2 +i+1)).B;

		r-= 1 * (*(row1 +i)).R;
		g-= 1 * (*(row1 +i)).G;
		b-= 1 * (*(row1 +i)).B;
		
		r-= 1 * (*(row3 +i)).R;
		g-= 1 * (*(row3 +i)).G;
		b-= 1 * (*(row3 +i)).B;


		r = (r>255)? 255 :((r<0)? 0: r);
		g = (g>255)? 255 :((g<0)? 0: g);
		b = (b>255)? 255 :((b<0)? 0: b);

		/*
		pixel middle= *(row2+ i);
		pixel south = *(row3+ i);
		pixel north = *(row1+ i);
		pixel west  = *(row2+ i-1);
		pixel east =  *(row2+ i+1);
		
			

		int Ri = 5*middle.R -1*south.R-1*north.R-1*west.R-1*east.R;
		Ri = (Ri>255)? 255 :((Ri<0)? 0: Ri);
		int Gi = 5*middle.G -1*south.G-1*north.G-1*west.G-1*east.G;
		Gi = (Gi>255)? 255 :((Gi<0)? 0: Gi);
		int Bi = 5*middle.B -1*south.B-1*north.B-1*west.B-1*east.B;
 		Bi = (Bi>255)? 255 :((Bi<0)? 0: Bi);
		
		pix= (pixel){.R =Ri, .G=Gi, .B=Bi};
	*/	
		pix = (pixel) {r,g,b};
		countgreyscale(pix,histogram);
		*(*out+i) = pix;

		}
		else{
		pix = *(row2 +i);
		countgreyscale(pix,histogram);
		*(*out+i) = pix;
		
		}
	}
	/*pixel middle= *(row2+ offset);
	pixel south = *(row3+ offset);
	pixel north = *(row1+ offset);
	pixel west  = *(row2+ offset-1);
	pixel east =  *(row2+ offset+1);*/

	/*int Ri = 5*middle.R -1*south.R-1*north.R-1*west.R-1*east.R;
	Ri = (Ri>255)? 255 :((Ri<0)? 0: Ri);
	int Gi = 5*middle.G -1*south.G-1*north.G-1*west.G-1*east.G;
	Gi = (Gi>255)? 255 :((Gi<0)? 0: Gi);
	int Bi = 5*middle.B -1*south.B-1*north.B-1*west.B-1*east.B;
 	Bi = (Bi>255)? 255 :((Bi<0)? 0: Bi);
	pixel out = {.R =Ri, .G=Gi, .B=Bi};
	return out;*/

}
	
	void printrow(pixel* row, int column){
	for (int i=0;i< column; i++){
		pixel pix = row[i];
		printf("%3u%3u%3u    ",pix.R,pix.G, pix.B);
	}
	printf("\n");
}

void convert (unsigned char* buffer, int l, int* var){
	
	int sum=0;

	for(int i=0; i<l; i++){
		sum+=buffer[i];
		sum*=10;
	}
	sum/=10;
	*var = sum;	
}
int readvariable(int picture, int* var){
	
	unsigned char buffer [6];
	unsigned char temp;
	int counter=0;
	
	if (read(picture,&temp, 1)==-1) return 100;
	while (temp != '\n'){
		buffer[counter] = temp-'0';
		counter++;
		if (read (picture, &temp, 1)==-1) return 100;
	}

	convert(buffer, counter,var);
	return counter;
}


		




//int process2 (FILE* picture){
int process2 (int picture){
	
	int columns, rows,rgb; 
	int histogram [5] = {0,0,0,0,0};
	int size [3];

	//fgetc(picture);
	//fgetc(picture);
	unsigned char temp [3];
	if (read (picture, &temp, 3)==-1) return 100;
	size[0] = readvariable(picture, &columns);
	size[1] = readvariable(picture, &rows);
	size[2] = readvariable(picture, &rgb);
		
	
	//printf("%d %d %d\n", columns, rows, rgb);
	
	
	int fd = open("output.ppm",O_CREAT| O_TRUNC| O_WRONLY, 0777);
	if (write (fd, "P6\n", 3)==-1) return 100;
	char buffer[size[0]];
	sprintf(buffer, "%d", columns);
	if (write (fd, buffer, size[0])==-1) return 100;
	if (write (fd, "\n",1) ==-1) return 100;
	char buffer1[size[1]];
	sprintf(buffer1, "%d", rows);
	if (write (fd, buffer1, size[1])==-1) return 100;
	if (write (fd, "\n",1)==-1) return 100;
	char buffer2[size[2]];
	sprintf(buffer2, "%d", rgb);
	if (write (fd, buffer2, size[2])==-1) return 100;
	if (write (fd, "\n",1)==-1) return 100;
	
	pixel pix;	


	pixel* out = (pixel*)malloc(sizeof(pixel)*columns);
	pixel* row1 = (pixel*) malloc(sizeof(pixel) * columns);
	if (read(picture, row1, 3* columns)==-1) return 100;
	//printrow(row1, columns);
	for (int i=0; i< columns;++i){
		pix = row1[i];
		countgreyscale(pix,histogram);
	}
	//fwrite(row1, sizeof(pixel), columns,f);
	if (write(fd, (unsigned char*) row1, 3*columns)==-1) return 100;

	pixel* row2 = (pixel*) malloc(sizeof(pixel) * columns);
	if (read(picture, row2, 3* columns)==-1) return 100;
	//printrow(row2, columns);
	pixel* row3 = (pixel*) malloc(sizeof(pixel) * columns); 
	if (read(picture, row3, 3* columns)==-1) return 100;
	//printrow(row3, columns);

	for (int i=0; i <rows-2; ++i){
		getoutput(row1,row2,row3, &out,columns,histogram);
		if(write(fd, (unsigned char*) out, 3*columns)==-1) return 100;
			if ( i!= rows-3){
			if(read(picture,row1, 3* columns)==-1) return 100;
			//printrow(row1, columns);
			pixel* temp = row3;
			row3 = row1;
			row1 = row2;
			row2 = temp;

		}
	}
	for (int i=0; i< columns;++i){
		pix = row3[i];
		countgreyscale(pix,histogram);
	}
	if (write (fd, (unsigned char *) row3, 3*columns)==-1) return 100;

	//writing histogram	
	FILE* h = fopen("output.txt", "w");
	for (int i=0; i< 5;i++){
		fprintf(h, "%d",histogram[i]);
		if (i!=4) fprintf(h," ");
	}
	fclose(h);

	free(row1);
	free(row2);
	free(row3);
	free(out);
	close(fd);
	close(picture);
	return 0;

}



int main(int argc, char** argv){
	if (argv[1] == NULL){
		printf("NO ARUMETNS\n");
		return 0;
	}
	
	int picture;
	picture = open(argv[1], O_RDONLY);
	if (100 == process2(picture))
		return 100;
	return 0;
}

