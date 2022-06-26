#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "libbmp.h" //works only for 24-bit .bmp

//structure of a .bmp file
typedef struct bitmap_header {
	char format[2];
	unsigned int size;
	int reserved;
	unsigned int offset;
}bitmap_header;

typedef struct dib_header {
	unsigned int size;
	unsigned int width;
	unsigned int height;
	uint16_t color_planes;
	uint16_t bpp; // bits per pixel
	unsigned int compression;
	unsigned int image_size;
}dib_header;

//structure of 8-bit .bmp
typedef struct image
{
	bitmap_header header;
	dib_header dibheader;
	unsigned char** pixels;
}image;

//create image
image read_image(FILE* bitmap, dib_header dibheader, bitmap_header header);
image open_bitmap(FILE* bitmap);

//used depending if file is 8-bit .bmp or 24-bit .bmp
void grayscale_compress(const char* filename);
void compress(const char* filename);