#include <stdio.h>

#include "config.h"
#include "brush.h"
#include "drawing.h"
#include "hash.h"

int main(int argc, char **argv) {
	unsigned int i;
	union  hash h;
	image img;
	char buff[8];

	if(argc == 1) {
		printf("No arguments, exiting\n");
		return 0;
	}
	
	for(i = 1; i < argc; ++i) {
		printf("Hashing %s...\n", argv[i]);
		h = make_hash(argv[i]);
		printf("Hashed, preparing image...\n");
		img = init_image();
		printf("Drawing image...\n");
		draw_image(img, h.cmds, 64 / sizeof(struct brush_command));
		printf("Saving image...\n");
		snprintf(buff, 7, "%u.ff", i);
		save_farbfeld(img, buff);
		printf("Done\n");
	}

	return 0;
}

