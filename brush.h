#ifndef BRUSH_H
#define BRUSH_H

#include "drawing.h"

struct brush_command {
	int ax : 4;
	int ay : 4;
};

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
void tick_brush(struct brush *b, image img, struct brush_command cmd);
void draw_image(image img, struct brush_command *cmds, unsigned int ct);

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

void tick_brush(struct brush *b, image img, struct brush_command cmd) {
	int px, py;
	unsigned int i;

	b->ax = cmd.ax;
	b->ay = cmd.ay;

	for(i = 0; i < SMOOTHING; ++i) {
		px = b->x;
		py = b->y;

		move_brush(b);
		draw_line(img, px, py, b->x, b->y, b->value);
	}
	++b->value;
}

void draw_image(image img, struct brush_command *cmds, unsigned int ct) {
	unsigned int i;
	struct brush b;

	b = make_brush();

	for(i = 0; i < ct; ++i) {
		tick_brush(&b, img, cmds[i]);
	}
}

#endif

