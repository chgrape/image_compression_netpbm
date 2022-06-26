#define _CRT_SECURE_NO_WARNINGS
#include "image_compression_pbm.h"

image read_image(FILE* bitmap, dib_header dibheader, bitmap_header header) {
	image im;
	im.header = header;
	im.dibheader = dibheader;
	fseek(bitmap, header.offset, SEEK_SET);
	if (im.dibheader.bpp == 8) {
		im.pixels = malloc(im.dibheader.height*4);
		for (int i = im.dibheader.height-1; i>=0; i--) {
			im.pixels[i] = malloc(im.dibheader.width);
			if (!im.pixels[i]) return;
			fread(im.pixels[i], im.dibheader.width, 1, bitmap);
		}
	}
	return im;
}

image open_bitmap(FILE* bitmap) {
	dib_header dibheader;
	bitmap_header header;
	fread(&header, 2, 1, bitmap);
	fread(&header.size, 3 * 4, 1, bitmap);
	fread(&dibheader, 24, 1, bitmap);
	image im = read_image(bitmap, dibheader, header);
	return im;
	fclose(bitmap);
}

void grayscale_compress(const char* filename) {
	FILE* f = fopen("image3.bmp", "rb");
	bitmap_header header;
	dib_header dibheader;
	image input = open_bitmap(f, &header, &dibheader);
	FILE* comp_img = fopen("output.pgm", "wb");
	int height = input.dibheader.height;
	int width = input.dibheader.width;
	char format[3];
	strcpy(format, "P5");
	fprintf(comp_img, "%s\n%d %d\n%d\n", format, width, height, 255);
	for (int j = 0; j < height; ++j)
	{
			fwrite(input.pixels[j], input.dibheader.height, 1, comp_img);
	}
	fclose(comp_img);
}

void compress(const char* filename) {
	bmp_img img;//compatible only with 24 bpp bmp
	bmp_img_read(&img, filename);

	if (img.img_header.biBitCount == 8) {
		grayscale_compress(filename);
		return;
	}
	if (img.img_header.biBitCount != 24 || img.img_header.biCompression != 0) {
		printf("Image is already compressed or wrong format. Exiting program...");
		return;
	}

	FILE* comp_img = fopen("output.ppm", "wb");
	int height = img.img_header.biHeight;
	int width = img.img_header.biWidth;
	char format[3];
	strcpy(format, "P6");
	fprintf(comp_img, "%s\n%d %d\n%d\n", format, width, height, 255);
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			static unsigned char color[3];
			color[0] = img.img_pixels[j][i].red;
			color[1] = img.img_pixels[j][i].green;
			color[2] = img.img_pixels[j][i].blue;
			fwrite(color, 1, 3, comp_img);
		}
	}
	fclose(comp_img);
	bmp_img_free(&img);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Incorrect number of arguments");
		return 1;
	}
	if (!strstr((const char*)argv[1], ".bmp")) {
		printf("File format not supported");
		return 1;
	}
	compress(argv[1]);
	return 0;
}
