#ifndef BRUSH_H
#define BRUSH_H

#include "drawing.h"

typedef unsigned char brush_command;
#define GET_AX_1(bc) ((int)(bc & 0x3) - 2)
#define GET_AY_1(bc) ((int)(bc >> 2 & 0x3) - 2)
#define GET_AX_2(bc) ((int)(bc >> 4 & 0x3) - 2)
#define GET_AY_2(bc) ((int)(bc >> 6 & 0x3) - 2)

struct brush {
	int ax;
	int ay;
	int vx;
	int vy;
	int x;
	int y;
	unsigned int value;
};

struct brush make_brush();
void move_brush(struct brush *b);
void tick_brush(struct brush *b, image img, brush_command cmd, int first);
void draw_image(image img, brush_command *cmds, unsigned int ct);

struct brush make_brush() {
	struct brush b;

	b.ax = b.ay = 0;
	b.vx = b.vy = 0;
	b.x = RES_X/2;
	b.y = RES_X/2;
	b.value = 0;

	return b;
}

void move_brush(struct brush *b) {
	b->vx += b->ax;
	if(b->vx > MAX_VEL) b->vx = MAX_VEL;
	if(b->vx < -MAX_VEL) b->vx = -MAX_VEL;
	b->vy += b->ay;
	if(b->vy > MAX_VEL) b->vy = MAX_VEL;
	if(b->vy < -MAX_VEL) b->vy = -MAX_VEL;
	b->x += b->vx;
	b->y += b->vy;
}

void tick_brush(struct brush *b, image img, brush_command cmd, int first) {
	int px, py;
	unsigned int i;

	b->ax = first ? GET_AX_1(cmd) : GET_AX_2(cmd);
	b->ay = first ? GET_AY_1(cmd) : GET_AY_2(cmd);
	if(b->ax >= 0) ++b->ax;
	if(b->ay >= 0) ++b->ay;

	for(i = 0; i < SMOOTHING; ++i) {
		px = b->x;
		py = b->y;

		move_brush(b);
		draw_line(img, px, py, b->x, b->y, b->value);
	}
	++(b->value);
}

void draw_image(image img, brush_command *cmds, unsigned int ct) {
	unsigned int i;
	struct brush b;

	b = make_brush();

	for(i = 0; i < ct; ++i) {
		tick_brush(&b, img, cmds[i], 1);
		tick_brush(&b, img, cmds[i], 0);
	}
}

#endif

