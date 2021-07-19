#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <MiniFB.h>

#include "../model.h"
#include "../text.h"

#include "editor.h"

#define WIDTH 800
#define HEIGHT 600

#define BORDER 0x00ffffff

static uint32_t *buffer;

poly *poly_set;
int poly_set_len, current_window = 0, poly_sel = 0;
app_mode mode = MODE_VIEW;

view_props
    front = {WIDTH/4, HEIGHT/4, 10},
    side = {WIDTH/4, HEIGHT/4, 10},
    top = {WIDTH/4, HEIGHT/4, 10};

int main(int argc, char **argv)
{
    int state, x, y, i;
    view_props 
	front_offset = {WIDTH/2, 0},
	side_offset = {0,(HEIGHT-12)/2},
	top_offset = {WIDTH/2, (HEIGHT-12)/2};
    poly2 poly_front, poly_side, poly_top;
    char info_front[16], info_side[16], info_top[16];
    vec2 frame = {WIDTH/2, (HEIGHT-12)/2};
    
    
    struct mfb_window *window = mfb_open_ex("editor", WIDTH, HEIGHT, WF_RESIZABLE);
    mfb_set_keyboard_callback(window, keyboard);

    char status[128] = "--STATUS--";
    poly_set = malloc(0);

    buffer = malloc(WIDTH * HEIGHT * sizeof(uint32_t));
    if (!window)
	return 1;

    load_model("./assets/square.bin", &poly_set, &poly_set_len);

    do
    {
	state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);
	memset(buffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));
	for (i = 0; i < poly_set_len; i++)
	{
	    poly_front = (poly2)
		{
		    {front.x + front.zoom*poly_set[i].a.x + front_offset.x, (front.y + front.zoom*poly_set[i].a.y)},
		    {front.x + front.zoom*poly_set[i].b.x + front_offset.x, (front.y + front.zoom*poly_set[i].b.y)},
		    {front.x + front.zoom*poly_set[i].c.x + front_offset.x, (front.y + front.zoom*poly_set[i].c.y)},
		};

	    draw_line(poly_front.a, poly_front.b, i == poly_sel ? 0x00ffff00 : 0x00ff0000, front_offset, frame);
	    draw_line(poly_front.b, poly_front.c, i == poly_sel ? 0x00ffff00 : 0x00ff0000, front_offset, frame);
	    draw_line(poly_front.c, poly_front.a, i == poly_sel ? 0x00ffff00 : 0x00ff0000, front_offset, frame);

	    poly_side = (poly2)
		{
		    {side.x + side.zoom*poly_set[i].a.z,(side_offset.y + side.y + side.zoom*poly_set[i].a.y)},
		    {side.x + side.zoom*poly_set[i].b.z,(side_offset.y + side.y + side.zoom*poly_set[i].b.y)},
		    {side.x + side.zoom*poly_set[i].c.z,(side_offset.y + side.y + side.zoom*poly_set[i].c.y)}
		};

	    draw_line(poly_side.a, poly_side.b, i == poly_sel ? 0x00ffff00 : 0x00008800, side_offset, frame);
	    draw_line(poly_side.b, poly_side.c, i == poly_sel ? 0x00ffff00 : 0x00008800, side_offset, frame);
	    draw_line(poly_side.c, poly_side.a, i == poly_sel ? 0x00ffff00 : 0x00008800, side_offset, frame);

	    poly_top = (poly2)
		{
		    {top.x + top.zoom*poly_set[i].a.x + top_offset.x, top_offset.y + top.y + top.zoom*poly_set[i].a.z},
		    {top.x + top.zoom*poly_set[i].b.x + top_offset.x, top_offset.y + top.y + top.zoom*poly_set[i].b.z},
		    {top.x + top.zoom*poly_set[i].c.x + top_offset.x, top_offset.y + top.y + top.zoom*poly_set[i].c.z}
		};

	    draw_line(poly_top.a, poly_top.b, i == poly_sel ? 0x00ffff00 : 0x004444ff, top_offset, frame);
	    draw_line(poly_top.b, poly_top.c, i == poly_sel ? 0x00ffff00 : 0x004444ff, top_offset, frame);
	    draw_line(poly_top.c, poly_top.a, i == poly_sel ? 0x00ffff00 : 0x004444ff, top_offset, frame);

	}

	/* UI-Dividing lines horizontal */
        for (x = 0; x < WIDTH; x++)
	{
	    buffer[((HEIGHT - 12)/2) * WIDTH + x] = BORDER;
	    buffer[(HEIGHT - 12) * WIDTH + x] = BORDER;
	}

	/* UI-Dividing lines vertical */
	for (y = 0; y < HEIGHT; y++)
	    buffer[(y * WIDTH) + (WIDTH/2)] = BORDER;

	sprintf(info_front, "%cF%c %3d/%-3d", current_window == 0 ? '[' : ' ', current_window == 0 ? ']' : ' ', front.x, front.y);

	draw_text(front_offset.x, front_offset.y + 2, info_front);
	draw_text(side_offset.x, side_offset.y + 2, current_window == 1 ? "[S]" : " S ");
	draw_text(top_offset.x, top_offset.y + 2, current_window == 2 ? "[T]" : " T ");

	draw_text(0,HEIGHT - 10, status);
	draw_text(0,2,"[HELP]");

	
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
		    buffer[((iy + y) * WIDTH) + (ix + x) + (i * 8)] = 0x00ffffff;
	    }
	}
    }
}

void draw_line(vec2 p1, vec2 p2, uint32_t c, view_props p, vec2 s)
{
    float i, steps;
    int x, y;
    vec2 tmp, dir;

    if (p1.x > p2.x)
    {
	tmp = vec2_cpy(p2);
	p2 = vec2_cpy(p1);
	p1 = vec2_cpy(tmp);
    }

    dir = vec2_sub(p2, p1);
    steps = fabsf(dir.x) > fabsf(dir.y) ? fabsf(dir.x) : fabsf(dir.y);

    for (i = 0; i < steps; i++)
    {
	x = (int) (p1.x + (dir.x / steps * i));
	y = (int) (p1.y + (dir.y / steps * i));
	if (x > p.x && x < p.x + s.x && y > p.y && y < p.y + s.y)
	{
	    buffer[y * WIDTH + x] = c;
	}
    }
    
}

void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool is_pressed)
{
    view_props *current_view;

    if (current_window == 0) { current_view = &front; }
    else if (current_window == 1) { current_view = &side; }
    else if (current_window == 2) { current_view = &top; }
    
    if(key == KB_KEY_ESCAPE)
    {
        mfb_close(window);
    }
    
    if (is_pressed)
    {
	if (key == KB_KEY_TAB)
	{
	    current_window++;
	    if (current_window > 2)
		current_window = 0;
	}

        if (key == KB_KEY_Q)
	{
	    if (poly_sel > 0)
		poly_sel--;
	    else
		poly_sel = poly_set_len - 1;

	}
	else if (key == KB_KEY_E)
	{
	    if (poly_sel < poly_set_len - 1)
		poly_sel++;
	    else
		poly_sel = 0;

	}

	if (mode == MODE_VIEW)
	{
	    if (key == KB_KEY_A)
		current_view->x--;

	    if (key == KB_KEY_D)
		current_view->x++;

	    if (key == KB_KEY_W)
		current_view->y--;

	    if (key == KB_KEY_S)
		current_view->y++;

	    if (key == KB_KEY_Z)
		current_view->zoom++;

	    if (key == KB_KEY_X)
		current_view->zoom--;
	}
    }
}
