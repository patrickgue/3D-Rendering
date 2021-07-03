#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <MiniFB.h>

#include "../model.h"
#include "../text.h"

#define WIDTH 800
#define HEIGHT 600

#define BORDER 0x00ffffff

static uint32_t *buffer;


int main(int argc, char **argv)
{
    int state, x, y;
    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, WF_RESIZABLE);

    buffer = malloc(WIDTH * HEIGHT * sizeof(uint32_t));
    if (!window)
	return 1;

    do
    {
	state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

        for (x = 0; x < WIDTH; x++)
	    buffer[(HEIGHT/2) * WIDTH + x] = BORDER;

	for (y = 0; y < HEIGHT; y++)
	    buffer[(y * WIDTH) + (WIDTH/2)] = BORDER;

	
	if (state < 0) {
	    window = NULL;
	    break;
	}
    } while(mfb_wait_sync(window));
    return 0;
}


void draw_text(int x, int y, char *text)
{
    int l = strlen(text);
    int ix, iy, i;
    char character[64];

    for(i = 0; i < l; i++)
    {
	font_char(character, text[i]);
	for (ix = 0; ix < 8; ix++)
	{
	    for (iy = 0; iy < 8; iy++)
	    {
		if (character[(iy * 8) + ix] == 1)
		    buffer[(iy * WIDTH) + ix + (i * 8)] = 0x00ffffff;
	    }
	}
    }
}
