#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

bool is_blank(char* str) {
	int len = strlen(str);
	char* suffix = "blank.png";
	int suffix_len = strlen(suffix);
	for (int i = 0; i < suffix_len; i++)
		if (str[len - (suffix_len - i)] != suffix[i])
			return false;
	return true;
}

// processing copies the entire bitmaps many times -> unnecessary -> slow -> todo
void process_image(void* args) {
	char** filenames;
	filenames = (char**)args;
	// READ IMAGES

	int w, h, n;
	unsigned char *imgs[4];
	for (int i = 0; i < 4; i++) {
		unsigned char *big = stbi_load(filenames[i], &w, &h, &n, 4);
		imgs[i] = malloc(w * h);
		stbir_resize_uint8(big, w, h, 0, imgs[i], w / 2, h / 2, 0, 4);
		free(big);
	}
	w /= 2; // the images get scaled down
	h /= 2;

	// MERGE IMAGES

	int mw = w * 2; // final image is 2x2 smaller images
	int mh = h * 2;
	unsigned char merged[mw * mh * 4]; // 4 chars per pixel (rgba)

	for (int l = 0; l < h; l++) { memcpy(&merged[(((l    ) * mw)    ) * 4], &imgs[0][l * w * 4], w * 4); }
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l    ) * mw) + w) * 4], &imgs[1][l * w * 4], w * 4); }
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l + h) * mw)    ) * 4], &imgs[2][l * w * 4], w * 4); }
	for (int l = 0; l < h; l++) { memcpy(&merged[(((l + h) * mw) + w) * 4], &imgs[3][l * w * 4], w * 4); }

	for (int i = 0; i < 4; i++) free(imgs[i]);

	// WRITE MERGED IMAGE

	stbi_write_png(filenames[4], mw, mh, 4, merged, 0);
	printf("Wrote %s\n", filenames[4]);

	for (int i = 0; i < 4; i++)
		if (!is_blank(filenames[i]))
			free(filenames[i]);

	free(filenames[4]);
	free(filenames);
}
