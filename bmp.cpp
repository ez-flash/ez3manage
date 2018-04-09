#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

unsigned char colors[256][3];
int reduce = 0;
int all_cols = 0;
unsigned short *col_list;
unsigned char index_list[256];


void memswap(unsigned char *p1, unsigned char *p2, int size)
{
	int i;
	unsigned char t;
	for(i=0; i<size; i++) {
		t = *p1; *p1++ = *p2; *p2++ = t;
	}
}

int fread_word(FILE *fp)
{
	unsigned char t[2];
	fread(t, 2, 1, fp);
	return t[0] | (t[1]<<8);
}

int fread_long(FILE *fp)
{
	unsigned short l,h;
	l = fread_word(fp);
	h = fread_word(fp);
	return l | (h<<8);
}

int read_bmp(char *fname, void **dest, int dest_bpp, int *w, int *h, int alpha, int header_size)
{
	int start, size, width, height, bpp, compr, i,j,l,w4;
	size_t readlen;
	unsigned int color, col16;
	unsigned char *dest8, *src8, *src;
	unsigned short *dest16, *src16;
	
	FILE *fp;
	char id[3];
	unsigned char cols[3];
	unsigned char v;

	id[2] = 0;
	fp = fopen(fname, "rb");
	if(!fp) {
		//printf("File %s not found!\n", fname);
		return ERR_BMP_NOTFOUND;
	}
	{
		//printf("Checking tag\n");
		fread(id, 2, 1, fp);
		if((strcmp(id, "BM") != 0))
			return ERR_BMP_NOBMP;
		
		//printf("File is BMP\n");
		fseek(fp, 10, SEEK_SET);
		start = fread_long(fp);
		size = fread_long(fp);
		width = fread_long(fp);
		height = fread_long(fp);
		fread_word(fp);
		bpp = fread_word(fp);
		compr = fread_word(fp);
		if(compr != 0)
			return ERR_BMP_COMPRESS;
	
		//printf("BMP is %dx%d bpp=%d\n", width, height, bpp);
	
		//printf("Seeking to %d\n", 14 + size);
		fseek(fp, 14 + size, SEEK_SET);	
		if(bpp == 8) {
			for(i=0; i<256; i++) {
				fread(colors[i], 3, 1, fp);

				fread(&v, 1, 1, fp);
				////printf("%03d  %02x %02x %02x\n", i, colors[i][0], colors[i][1], colors[i][2]);
			}
		}
	        
		fseek(fp, start, SEEK_SET);	

	    //printf("Allocating memory (%d x %d x %d)\n", width, height, dest_bpp);


		if(width%4!=0)
			w4 = ((width+3)/4)*4;
		else
			w4 = width;

		//printf("width=%d,\tw4=%d\n",width,w4);


		*dest = (unsigned char *)malloc(w4*height*(dest_bpp/8) + header_size);
		dest8 = ((unsigned char *)*dest) + header_size;
		dest16 = (unsigned short *)dest8;

		src = (unsigned char *)malloc(w4*height*(bpp/8));

		if(reduce)
			col_list = (unsigned short *)malloc(sizeof(unsigned int) * 1024);


		//printf("Converting colors\n");
		
		readlen = fread(src, 1,width*height*(bpp/8),  fp);
		if(readlen != (unsigned int)(width*height*(bpp/8)))
		{
			printf("\nERROR: READLEN NOT EQUARE WANTED!  %d\n",readlen);
			return 1;
		}
		src8 = (unsigned char *)src;
		src16 = (unsigned short *)src;
		
		j=0;		
		for(i=0; i<width*height; i++) {
			switch(bpp) {
			case 8:
				//fread(fp, &v, 1);
				v = *src8++;
				cols[0] = colors[v][0];
				cols[1] = colors[v][1];
				cols[2] = colors[v][2];
				if(width%4!=0)
					if(i%width==width-1)
						src8+=4-(width%4);
				break;
			case 24:
				cols[0] = *src8++;
				cols[1] = *src8++;
				cols[2] = *src8++;
				if(width%4!=0)
					if(i%width==width-1)
						src8+=4-((width*3)%4);
				break;
			}
	
			color = (cols[0]<<16) | (cols[1]<<8) | (cols[2]);
			
			if(alpha) {
				col16 = (cols[0]>>3) | ((cols[1]&0xf8)<<2) | ((cols[2]&0xf8)<<7);
				//col16 = (cols[0]>>4) | (cols[1]&0xf0) | ((cols[2]&0xf0)<<4);
				if(col16 == 0x7FFF)
					col16 = 0;
				else
	                col16 |= 0x8000;
			} else
				col16 = (cols[2]>>3) | ((cols[1]&0xf8)<<2) | ((cols[0]&0xf8)<<7);
	
			if(reduce) {
				int i;
				for(i=0; i<all_cols; i++)
					if(col_list[i] == col16)
						i = all_cols+1;;

				if(i == all_cols) {
					col_list[all_cols] = col16;
					index_list[v] = all_cols;
					all_cols++;
				}
			}

			switch(dest_bpp) {
			case 8:
				dest8[j] = v;
				j++;
				break;
			case 16:
				dest16[j] = col16;
				j++;
				if(width%4!=0)
					if(i%width==width-1)
					{
						int ii;
						for(ii=0;ii<4-(width%4);ii++)
						{
							dest16[j] = 0;	
							j++;
						}
					}
 				break;
			case 24:
		        dest8[j] = cols[0];
				j++;
		        dest8[j] = cols[1];
				j++;
		        dest8[j] = cols[2];
				j++;
				if(width%4!=0)
					if(i%width==width-1)
					{
						int ii;
						for(ii=0;ii<4-(width%4);ii++)
						{
							dest8[j] = 0;	
							j++;
							dest8[j] = 0;
							j++;
							dest8[j] = 0;
							j++;
						}
					}
		        break;
	        }
        }
		//printf("Closing File\n");
        fclose(fp);

		free(src);

		l = w4*(dest_bpp/8);
		for(i=0; i<height/2; i++)
			memswap(&dest8[i*l], &dest8[(height-1-i)*l], l);
		
		*w = w4;
		*h = height;
		
		return ERR_BMP_OK;
	}

	return ERR_BMP_NOTFOUND;
}
