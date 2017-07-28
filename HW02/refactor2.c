#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define loop(n) for(int i = 0; i < n; ++i)		

typedef struct px{
	unsigned char R ;
	unsigned char G ;
	unsigned char B;
}px;

void hiscount(px p, int his [5]){
	int greyscale = round(0.2126*p.R +0.7152*p.G + 0.0722*p.B);
	if (greyscale < 51) his[0]++ ;
	else if (greyscale<102) his[1]++;
	else if (greyscale<153) his[2]++;
	else if (greyscale<204) his[3]++;
	else his[4]++;
}

int main(int argc, char** argv){
	FILE* picture; int r, c,maxrgb; px p;	
	
	picture = fopen (argv[1],"rb");
	fgetc(picture); fgetc(picture);
	if (fscanf(picture,"%d %d %d", &c, &r, &maxrgb) == -1) return -1;
	fgetc(picture);	
	
	FILE* out = fopen("output.ppm","wb");
	fprintf(out, "P6\n%d\n%d\n%d\n", c,r, maxrgb);
	int his [5] = {0,0,0,0,0};

	
	px* row1 = (px*) malloc(sizeof(px) * c);	
	px* row2 = (px*) malloc(sizeof(px) * c);
	px* row3 = (px*) malloc(sizeof(px) * c); 
	if (fread(row1, 1, 3*c,picture)==-1) return -1;
	if (fread(row2, 1, 3*c,picture)==-1) return -1;
	if (fread(row3, 1, 3*c,picture)==-1) return -1;

	loop(c){
		p =row1[i];
		hiscount(p,his);
	}
	fwrite(row1, sizeof(px), c,out);

	loop(r-2){
	     	loop(c){
			if (i ==0 || i == c-1)
				p = *(row2 +i);
			else{
				int nR = 5*(row2[i]).R - row3[i].R - row1[i].R- row2[i-1].R - row2[i+1].R;
				int nG = 5*(row2[i]).G - row3[i].G - row1[i].G- row2[i-1].G - row2[i+1].G;
				int nB = 5*(row2[i]).B - row3[i].B - row1[i].B- row2[i-1].B - row2[i+1].B;
				p= (px){(nR>255)? 255 :((nR<0)? 0: nR), (nG>255)? 255 :((nG<0)? 0: nG),(nB>255)? 255 :((nB<0)? 0: nB)};
			}
			hiscount(p,his);
			*(row1+i) = p;
	    	 }
	     	fwrite(row1, sizeof(px), c,out);
		
	     	if ( i!= r-3){
			if (fread(row1, 1, 3*c,picture)==-1) return -1;
			px* temp = row3;
			row3 = row1; row1 = row2; row2 = temp;
		}
	}

	loop(c){
		p=row3[i];
		hiscount(p,his);
	}
	fwrite(row3, sizeof(px), c,out);
	
	FILE* h = fopen("output.txt", "w");
	fprintf(h, "%d %d %d %d %d",his[0],his[1],his[2],his[3],his[4]);

	free(row1);free(row2);free(row3);fclose(out);fclose(picture);fclose(h);	
	return 0;
}

