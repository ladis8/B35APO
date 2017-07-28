#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct pixel{
	unsigned char R ;
	unsigned char G ;
	unsigned char B;
}pixel;



int mask [3][3] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
int histogram [5] = {0,0,0,0,0};

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
void printtofile(pixel* pixelarray, int m, int n, FILE* f){
	for (int i= 0; i < m; i++){
		for (int j=0; j< n; j++){
			pixel pix = *(pixelarray +i*n+j);
			fprintf(f,"%3u%3u%3u    ",pix.R,pix.G, pix.B);
		}
		fprintf(f,"\n");
	}
}
	


int readfile (FILE* picture, int m, int n, pixel** pixarray){
	unsigned char r,g,b;
	*pixarray = (pixel*)malloc(m*n*sizeof(pixel));
	//pixel pix;
	for (int i= 0; i < m; i++){
		for (int j=0; j< n; j++){
		//	if (fread(&pix,sizeof(pixel),1, picture)==-1) return 0;
			if (fread(&r,1,1, picture)==-1) return 0;
		        if (fread(&g,1,1, picture)==-1) return 0;
		        if (fread(&b,1,1, picture)==-1) return 0;
	                pixel pix= {r,g,b};

			*(*pixarray +i*n +j) = pix;		
		}
	}
	return 1;
}

void countgreyscale(pixel pix){
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
/*void countgreyscale2(unsigned char r, unsigned char g, unsigned char b){
	//int greyscale = round(0.2126*pix.R +0.7152*pix.G + 0.0722*pix.B);
	int greyscale = (2126*r + 7152*g + 722*b);
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
}*/
/*pixel evaluatepixel(pixel* input, int m,int n, int i, int j){
	pixel middle= *(input+n*i +j);
	pixel south = *(input+n*(i+1) +j);
	pixel north = *(input+n*(i-1)+j);
	pixel west  = *(input+n*i + j-1);
	pixel east =  *(input+n*i +j+1);

	int Ri = 5*middle.R -1*south.R-1*north.R-1*west.R-1*east.R;
	Ri = (Ri>255)? 255 :((Ri<0)? 0: Ri);
	int Gi = 5*middle.G -1*south.G-1*north.G-1*west.G-1*east.G;
	Gi = (Gi>255)? 255 :((Gi<0)? 0: Gi);
	int Bi = 5*middle.B -1*south.B-1*north.B-1*west.B-1*east.B;
 	Bi = (Bi>255)? 255 :((Bi<0)? 0: Bi);
	pixel out = {.R =Ri, .G=Gi, .B=Bi};
	return out;
}*/
pixel evaluatepixel2(pixel* row1, pixel* row2, pixel* row3, int offset){
	pixel middle= *(row2+ offset);
	pixel south = *(row3+ offset);
	pixel north = *(row1+ offset);
	pixel west  = *(row2+ offset-1);
	pixel east =  *(row2+ offset+1);

	int Ri = 5*middle.R -1*south.R-1*north.R-1*west.R-1*east.R;
	Ri = (Ri>255)? 255 :((Ri<0)? 0: Ri);
	int Gi = 5*middle.G -1*south.G-1*north.G-1*west.G-1*east.G;
	Gi = (Gi>255)? 255 :((Gi<0)? 0: Gi);
	int Bi = 5*middle.B -1*south.B-1*north.B-1*west.B-1*east.B;
 	Bi = (Bi>255)? 255 :((Bi<0)? 0: Bi);
	pixel out = {.R =Ri, .G=Gi, .B=Bi};
	return out;
}
void getoutput(pixel* row1, pixel* row2, pixel* row3,pixel** out, int column){
	//int counter =-1;
	//int r,g,b;
	pixel pix;
	for(int i=0; i< column; ++i){
		if (i !=0 && i != column-1){

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
		countgreyscale(pix);
		*(*out+i) = pix;

		}
		else{
		pix = *(row2 +i);
		countgreyscale(pix);
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
	
/*void getoutput2(unsigned char* row1, unsigned char* row2, unsigned char* row3,unsigned char** out, int column){
	int counter =-1;
	int r,g,b;
	for(int i=0; i< column; ++i){
		if (i !=0 || i != column-1){
		++counter;
		r = 5* row2[counter] -1 *row3[counter] -1*row1[counter] -1* row2[counter-1] -1*row2[counter+1]; 	
		r = (r>255)? 255 :((r<0)? 0: r);
		*(*out + counter)=r;
		++counter;
		g = 5* row2[counter] -1 *row3[counter] -1*row1[counter] -1* row2[counter-1] -1*row2[counter+1]; 	
		g = (g>255)? 255 :((g<0)? 0: g);
		*(*out + counter)=g;
		++counter;
		b = 5* row2[counter] -1 *row3[counter] -1*row1[counter] -1* row2[counter-1] -1*row2[counter+1]; 	
		b = (r>255)? 255 :((b<0)? 0: b);
		*(*out + counter)=b;
		countgreyscale2(r,g,b);
		}
		else{
		r = row2[++counter];
		*(*out +counter) = r;
		g = row2[++counter];
		*(*out +counter) = g;
		b = row2[++counter];
		*(*out +counter) = b;
		countgreyscale2(r,g,b);

		}
	}
	pixel middle= *(row2+ offset);
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
	return out;

}*/
	
int readrow (FILE* picture, pixel** row, int column){
	unsigned char r,g,b;	
	for (int i=0; i< column; i++){
		if (fread(&r,1,1, picture)==-1) return 0;
		if (fread(&g,1,1, picture)==-1) return 0;
		if (fread(&b,1,1, picture)==-1) return 0;
		*(*row +i)= (pixel){r,g,b};
	}
	return 1;
	
}
void printrow(pixel* row, int column){
	for (int i=0;i< column; i++){
		pixel pix = row[i];
		printf("%3u%3u%3u    ",pix.R,pix.G, pix.B);
	}
	printf("\n");
}


int process2 (FILE* picture){
	
	int columns, rows,rgb; 

	fgetc(picture);
	fgetc(picture);
	if (fscanf(picture,"%d", &columns) == -1) return 1;
	if (fscanf(picture,"%d", &rows)==-1) return 1;
	if (fscanf(picture,"%d", &rgb)==-1) return 1;

	fgetc(picture);//reads the new line character
	
	
	FILE* f = fopen("output.ppm","wb");
	fprintf(f, "P6\n%d\n%d\n%d\n", columns,rows, rgb);

	//unsigned char temp;
	pixel pix;	

	//reading the first line
	pixel* row1 = (pixel*) malloc(sizeof(pixel) * columns);
	//unsigned char* row1=(unsigned char*) malloc(3*columns);
	//fread(row1, sizeof(unsigned char), 3*columns,f);
	//if (fread(row1, sizeof(pixel), columns,f)==-1) return 1;
	readrow(picture, &row1, columns);
	//printrow(row1,columns);
	//int counter =-1;
	for (int i=0; i< columns;++i){
		pix = row1[i];
		countgreyscale(pix);
		//countgreyscale(row1[i]);
		//countgreyscale2(row1[++counter],row1[++counter],row1[++counter]);
		//fwrite(&pix,sizeof(pixel),1,f);
	}
//	fwrite(row1, sizeof(unsigned char), 3*columns,f);
	fwrite(row1, sizeof(pixel), columns,f);

	pixel* row2 = (pixel*) malloc(sizeof(pixel) * columns);
	//if (fread(row2, sizeof(pixel), columns,f)==-1) return 1;
	readrow(picture, &row2, columns);
	pixel* row3 = (pixel*) malloc(sizeof(pixel) * columns); 
	//if (fread(row3, sizeof(pixel), columns,f)==-1) return 1;
	readrow(picture, &row3, columns);
	//unsigned char* row2=(unsigned char*) malloc(3*columns);
	//fread(row2, sizeof(unsigned char), 3*columns,f);
	//unsigned char* row3=(unsigned char*) malloc(3*columns);
	//fread(row3, sizeof(unsigned char), 3*columns,f);



	//printrow(row1, columns);
	//printrow(row2, columns);
	//printrow(row3, columns);
	//printf("\n\n");
		
	//unsigned char* out = (unsigne char*)malloc(3*columns);
	pixel* out = (pixel*)malloc(sizeof(pixel)*columns);

	for (int i=0; i <rows-2; ++i){
		getoutput(row1,row2,row3, &out,columns);
		//fwrite(row1, sizeof(unsigned char), 3*columns,f);
		fwrite(out, sizeof(pixel), columns,f);
		/*for (int j=0; j<columns;++j){
			if (j==0 || j==columns-1){
				pix = row2[j];
				countgreyscale(pix);
				fwrite(&pix,sizeof(pixel),1,f);
			}
			else{
				pix =evaluatepixel2(row1, row2,row3,j);
				countgreyscale(pix);
				fwrite(&pix, sizeof(pixel),1,f);
			}
			//printf("%3u%3u%3u    ",pix.R,pix.G, pix.B);


		}
		*/
		//printf("\n");
		if ( i!= rows-3){
			readrow(picture,&row1, columns);
			pixel* temp = row3;
			//unsigned char* temp = row3;
			//if (fread(row1, sizeof(unsigned char), columns,f)==-1) return 1 ;
			row3 = row1;
			row1 = row2;
			row2 = temp;

			//printrow(row1, columns);
			//printrow(row2, columns);
			//printrow(row3, columns);
		}
	}
	//int counter =-1;
	for (int i=0; i< columns;++i){
		pix = row3[i];
		countgreyscale(pix);
		//countgreyscale2(row3[++counter],row3[++counter],row3[++counter]);
		//fwrite(&pix,sizeof(pixel),1,f);
	}
	fwrite(row3, sizeof(pixel), columns,f);
	//write(row3, sizeof(unsigned char), 3*columns,f);

	//read the last line	
	/*for (int i=0; i< columns;i++){
		pix = row3[i];
		countgreyscale(pix);
		fwrite(&pix,sizeof(pixel),1,f);
	}*/
	free(row1);
	free(row2);
	free(row3);
	free(out);
	fclose(f);
	fclose(picture);
	return 0;

}

/*void process (pixel* input, int m, int n, pixel**output){
	*output = (pixel*) malloc(m*n* sizeof (pixel));
	FILE* f = fopen("output.ppm","wb");
	int rgb=255;
	fprintf(f, "P6\n%d\n%d\n%d\n", n, m , rgb);

	pixel pix;
	for (int i=0; i <m; i++){
		for (int j= 0; j<n;j++){
			if (i==0 || i==m-1 ||j==0 ||j==n-1){
				*(*output + i*n +j) = *(input + i*n +j);
				pix =*(input + i*n +j);
			}
			else{
				*(*output +i*n +j) = evaluatepixel(input, m, n, i, j);	
				pix =evaluatepixel(input,m,n,i,j);
			}
			
			countgreyscale(pix);
			fwrite(&pix, sizeof(pixel),1,f);
		}
	}
		fclose(f);

}*/
void writehistogram(){
	FILE* f = fopen("output.txt", "w");
	
	for (int i=0; i< 5;i++){
		fprintf(f, "%d",histogram[i]);
		if (i!=4) fprintf(f," ");
	}
	fclose(f);
}



int main(int argc, char** argv){
	if (argv[1] == NULL){
		printf("NO ARUMETNS\n");
		return 0;
	}
	FILE* picture;
	//FILE* output;
	//int rows, columns,maxrgb;
	picture = fopen (argv[1],"rb");

	if (picture ==NULL){
		printf("UNABLE TO OPEN FILE\n");
		return 0;
	}
	if (1 == process2(picture))
		return 1;
	

	/*fgetc(picture);
	fgetc(picture);
	if (fscanf(picture,"%d", &columns) == -1) return 1;
	if (fscanf(picture,"%d", &rows)==-1) return 1;
	if (fscanf(picture,"%d", &maxrgb)==-1) return 1;

	//printf ("%d %d %d\n", rows, columns, maxrgb);
	fgetc(picture);//reads the new line character
		

	pixel* input;
	if (readfile(picture,rows, columns,&input)!=1)
		return 1;*/
	
	//printpicture(input, rows, columns);
	//printf("\n\n");




	/*FILE* output;
	output = fopen(argv[2],"rb");

	if (output ==NULL){
		printf("UNABLE TO OPEN FILE\n");
		return 0;
	}

	fgetc(output);
	fgetc(output);
	fscanf(output,"%d", &columns);
	fscanf(output,"%d", &rows);
	fscanf(output,"%d", &maxrgb);
	fgetc(output);//reads the new line character


	pixel* out=NULL;
	readfile(output, rows, columns,&out);
	printpicture(out,rows,columns);
	
	FILE* outputright= fopen("outputright.txt", "w");
	printtofile(out,rows,columns, outputright);
	
	free(out);

*/
	/*pixel* myout;
	process(input,rows,columns,&myout);

//	printpicture(myout,rows,columns);
	//FILE* outputmy= fopen("outputmy.txt", "w");
	//printtofile(myout,rows,columns, outputmy);

	
	
	free(myout);
	free(input);

	//fclose(output);
	fclose(picture);*/
	
	writehistogram();	
	return 0;
}

