#ifndef DRAWING_H
#define DRAWING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIG_ENDIAN16(V) ( \
	(((V) & 0xFF00) >> 8) | \
	(((V) & 0x00FF) << 8))

#define BIG_ENDIAN32(V) ( \
	(((V) & 0xFF000000) >> 24) | \
	(((V) & 0x00FF0000) >> 8)  | \
	(((V) & 0x0000FF00) << 8)  | \
	(((V) & 0x000000FF) << 24))

typedef unsigned char channel_t;

typedef channel_t **image;

image init_image();
void set_pixel(image img, int x, int y, channel_t value);
channel_t get_pixel(image img, int x, int y);
void draw_line(image img, int x1, int y1, int x2, int y2, channel_t value);
void save_farbfeld(image img, char *name);

image init_image() {
	unsigned int i, j;
	image img;

	img = (image)malloc(RES_X * sizeof(channel_t *));
	for(i = 0; i < RES_X; ++i) {
		img[i] = (channel_t *)malloc(RES_Y * sizeof(channel_t));
		for(j = 0; j < RES_Y; ++j) {
			img[i][j] = (channel_t)-1;
		}
	}
	return img;
}

void set_pixel(image img, int x, int y, channel_t value) {
	img[(x % RES_X + RES_X) % RES_X][(y % RES_Y + RES_Y) % RES_Y] = value;
}

channel_t get_pixel(image img, int x, int y) {
	return img[(x % RES_X + RES_X) % RES_X][(y % RES_Y + RES_Y) % RES_Y];
}

void draw_line(image img, int x1, int y1, int x2, int y2, channel_t value) {
	int dx, dy, sx, sy, error, e2;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	sx = (x1 < x2) ? 1 : -1;
	sy = (y1 < y2) ? 1 : -1;
	error = dx - dy;

	while(x1 != x2 || y1 != y2) {
		set_pixel(img, x1, y1, value);

		e2 = 2 * error;
		if(e2 > -dy) {
			error -= dy;
			x1 += sx;
		}
		if(e2 < dx) {
			error += dx;
			y1 += sy;
		}
	}
	set_pixel(img, x1, y1, value);
}

void save_farbfeld(image img, char *name) {
	unsigned int x, y;
	FILE *f;
	uint32_t d;
	uint16_t v;

	f = fopen(name, "wb");
	if(!f) {
		printf("Could not open image %s\n", name);
		return;
	}

	fwrite("farbfeld", 1, 8, f);
	d = BIG_ENDIAN32(RES_X);
	fwrite(&d, 4, 1, f);
	d = BIG_ENDIAN32(RES_Y);
	fwrite(&d, 4, 1, f);

	for(y = 0; y < RES_Y; ++y) {
		for(x = 0; x < RES_X; ++x) {
#define MAX (HASH_LENGTH)
#define COLOR(S_VALUE, E_VALUE, DEFAULT) (get_pixel(img,x,y) == (channel_t)-1 ? DEFAULT : ((E_VALUE * get_pixel(img,x,y) + S_VALUE * (MAX - get_pixel(img,x,y)) + MAX/2) / MAX))
			v = BIG_ENDIAN16(COLOR(S_R, E_R, D_R) * 257);
			fwrite(&v, 2, 1, f);
			v = BIG_ENDIAN16(COLOR(S_G, E_G, D_G) * 257);
			fwrite(&v, 2, 1, f);
			v = BIG_ENDIAN16(COLOR(S_B, E_B, D_B) * 257);
			fwrite(&v, 2, 1, f);
			v = 65535;
			fwrite(&v, 2, 1, f);
#undef MAX
#undef COLOR
		}
	}
}

#endif

