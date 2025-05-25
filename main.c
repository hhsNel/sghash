#include <stdio.h>

#include "config.h"
#include "brush.h"
#include "drawing.h"
#include "hash.h"

void out_image(union hash h, unsigned int i);

int main(int argc, char **argv) {
	unsigned int i;
	union hash h;

	if(argc == 1) {
		printf("No arguments, reading from stdin\n");
		h = make_hash_stdin();
		out_image(h, 0);
		return 0;
	}
	
	for(i = 1; i < argc; ++i) {
		printf("Hashing %s...\n", argv[i]);
		h = make_hash(argv[i]);
		out_image(h, i);
	}

	return 0;
}

void out_image(union hash h, unsigned int i) {
	image img;
	char buff[8];

	printf("Hashed, preparing image...\n");
	img = init_image();
	printf("Drawing image...\n");
	draw_image(img, h.cmds, 64 / sizeof(brush_command));
	printf("Saving image...\n");
	snprintf(buff, 7, "%u.ff", i);
	save_farbfeld(img, buff);
	printf("Done\n");
}

