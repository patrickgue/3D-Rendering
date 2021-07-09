#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <MiniFB.h>

#include "../vector.h"
#include "../model.h"
#include "../text.h"

#include "editor.h"

#define WIDTH 800
#define HEIGHT 600

#define BORDER 0x00ffffff

static uint32_t *buffer;

poly *poly_set;
int poly_set_len;

int main(int argc, char **argv)
{
    int state, x, y, i, front_y = HEIGHT/4, front_x= WIDTH/4, front_zoom = 10,
	side_y = HEIGHT/4, side_x = WIDTH/4, side_zoom = 10;
    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, WF_RESIZABLE);
    char status[128] = "--STATUS--";
    poly_set = malloc(0);

    buffer = malloc(WIDTH * HEIGHT * sizeof(uint32_t));
    if (!window)
	return 1;

    load_model("./assets/cube.bin", &poly_set, &poly_set_len);

    do
    {
	state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

	for (i = 0; i < poly_set_len; i++)
	{

	    buffer[(int) (((front_y + front_zoom*poly_set[i].a.y) * WIDTH) + front_x + front_zoom*poly_set[i].a.x + WIDTH/2)] = 0x00ff0000;
	    buffer[(int) (((front_y + front_zoom*poly_set[i].b.y) * WIDTH) + front_x + front_zoom*poly_set[i].b.x + WIDTH/2)] = 0x00ff0000;
	    buffer[(int) (((front_y + front_zoom*poly_set[i].c.y) * WIDTH) + front_x + front_zoom*poly_set[i].c.x + WIDTH/2)] = 0x00ff0000;

	    buffer[(int) ((((HEIGHT-12)/2 + side_y + front_zoom*poly_set[i].a.y) * WIDTH) + side_x + side_zoom*poly_set[i].a.z)] = 0x00ff0000;
	    buffer[(int) ((((HEIGHT-12)/2 + side_y + front_zoom*poly_set[i].b.y) * WIDTH) + side_x + side_zoom*poly_set[i].b.z)] = 0x00ff0000;
	    buffer[(int) ((((HEIGHT-12)/2 + side_y + front_zoom*poly_set[i].c.y) * WIDTH) + side_x + side_zoom*poly_set[i].c.z)] = 0x00ff0000;

	    buffer[(int) ((((HEIGHT-12)/2 + side_y + front_zoom*poly_set[i].a.y) * WIDTH) + side_x + side_zoom*poly_set[i].a.z)] = 0x00ff0000;
	    buffer[(int) ((((HEIGHT-12)/2 + side_y + front_zoom*poly_set[i].b.y) * WIDTH) + side_x + side_zoom*poly_set[i].b.z)] = 0x00ff0000;
	    buffer[(int) ((((HEIGHT-12)/2 + side_y + front_zoom*poly_set[i].c.y) * WIDTH) + side_x + side_zoom*poly_set[i].c.z)] = 0x00ff0000;

	}
	
        for (x = 0; x < WIDTH; x++)
	{
	    buffer[((HEIGHT - 12)/2) * WIDTH + x] = BORDER;
	    buffer[(HEIGHT - 12) * WIDTH + x] = BORDER;
	}
	    
	for (y = 0; y < HEIGHT; y++)
	    buffer[(y * WIDTH) + (WIDTH/2)] = BORDER;

	draw_text(0,HEIGHT - 10, status);

	draw_text(0,0,"[HELP]");

	
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
		    buffer[((iy + y) * WIDTH) + ix + (i * 8)] = 0x00ffffff;
	    }
	}
    }
}
