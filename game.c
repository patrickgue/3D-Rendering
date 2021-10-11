#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <MiniFB.h>

#include "vector.h"
#include "text.h"
#include "model.h"
#include "game.h"

#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 80

uint32_t *buffer;
poly *polygon_set;
int polygon_set_len = 0, WIDTH = 0, HEIGHT = 0;
vecd3 camera = {0.0f,1.0f,-50.0f,0.0f,0.0f,0.0f};
bool keyboard_buffer[0x60], last_moved = true;

int main(int argc, char **argv)
{
    int x, y, i, j, k, sets_len, frames = 0, fps = 0, state;
    vec3 answ, c1, c2, c1r, c2r, set_center;
    vec3_ray camera_ray, d1, d2, d3;
    float tmp_dist;
    time_t last_sec = 0;
    char character[64],debug[128];
    bool debug_mode = false, swapped, res;
    poly tmp;

    float d_max = 30.0f, d_min = 3.0f, f_near = 0.01f, f_far = 0.1f, fx, fy;
    poly_set square, plane, test, cube, teapot, *sets;
  
    if (argc > 1)
    {
	for (i = 1; i < argc; i++)
	{
	    if (strcmp(argv[i], "-d") == 0)
	    {
		debug_mode = true;
	    }
	    if (strcmp(argv[i], "-w") == 0 && i < argc - 1)
	    {
		WIDTH = atoi(argv[i + 1]);
	    }
	    if (strcmp(argv[i], "-h") == 0 && i < argc - 1)
	    {
	        HEIGHT = atoi(argv[i + 1]);
	    }
	}
    }

    if (WIDTH == 0)
	WIDTH = DEFAULT_WIDTH;

    if (HEIGHT == 0)
	HEIGHT = DEFAULT_HEIGHT;

    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, WF_RESIZABLE);
    if (!window)
	return 0;

    
    mfb_set_keyboard_callback(window, keyboard);
    memset(keyboard_buffer, 0, 0x60);

    buffer = (uint32_t *) malloc(WIDTH * HEIGHT * 4);
    
    load_model("assets/square.bin", &square);
    load_model("assets/plane.bin", &plane);
    load_model("assets/test.bin", &test);
    load_model("assets/cube.bin", &cube);
    load_model("test/teapot.bin", &teapot);
    sets_len = 5;
    sets = malloc(sizeof(poly_set) * sets_len);
    sets[0] = square;
    sets[1] = plane;
    sets[2] = test;
    sets[3] = cube;
    sets[4] = teapot;

    for (i = 0; i < sets_len; i++)
    {
	polygon_set_len += sets[i].polygons_count;
    }
    polygon_set = (poly*) malloc(sizeof(poly) * polygon_set_len);

    do
    {
	sets[3].mov.yaw += 1;
	
	j = 0;
	for (i = 0; i < sets_len; i++)
	{
	    set_center = poly_set_center(sets[i]);
	    for (k = 0; k < sets[i].polygons_count; k++)
	    {
	       
		polygon_set[k + j] = poly_transform(sets[i].polygons[k], set_center, sets[i].mov) ;
	    }
	    j += sets[i].polygons_count;
	}
 
	qsort((void*)polygon_set, polygon_set_len, sizeof(poly), distance_comparison);
        
	for (x = 0; x < WIDTH; x++)
	{
	    for (y = 0; y < HEIGHT; y++)
	    {
		fx = sinf(camera.yaw * DTOR);
		fy = cosf(camera.yaw * DTOR);

		c1 = vec3_rotate_y(
		    vecd3_to_vec3(camera),
		    vec3_add(
			vec3_add(
			    vecd3_to_vec3(camera),
			    (vec3) {0.0f,0.0f,d_min}),
			(vec3) {f_near * (x - (WIDTH/2)), 0.0f, 0.0f}),
		    camera.yaw);

		c1.y = camera.y + (f_near * -1 * (y - (HEIGHT/2)));

		c2 = vec3_rotate_y(
		    vecd3_to_vec3(camera),
		    vec3_add(
			vec3_add(
			    vecd3_to_vec3(camera), 
			    (vec3) {0.0f,0.0f,d_max}),
			(vec3) {f_far * (x - (WIDTH/2)), 0.0f, 0.0f}),
		    camera.yaw);

		c2.y = camera.y + (f_far * -1 * (y - (HEIGHT/2)));

		camera_ray = (vec3_ray) {c1, c2};
		buffer[(y * WIDTH) + x] = 0;
	      
		for (i = polygon_set_len -1; i >= 0; i--)
		{
		    res = find_intersection(camera_ray, polygon_set[i], &answ);
		    if (res)
		    {
			buffer[(y * WIDTH) + x] = polygon_set[i].color;
		    }
		}

		if (y == HEIGHT/2)
		{
		    if (x == 0)
			d1 = (vec3_ray) {c1, c2};
		    if (x == WIDTH/2)
			d2 = (vec3_ray) {c1, c2};
		    if (x == WIDTH-1)
			d3 = (vec3_ray) {c1, c2};
		}
	    }
	}

	if (debug_mode)
	{
	    sprintf(debug, "FPS: %d X: %2.1f Y: %2.1f Z: %2.1f R: %2.1f", fps, camera.x, camera.y, camera.z, camera.yaw);
	    frames++;
	    if (last_sec < time(NULL))
	    {
		last_sec = time(NULL);
		fps = frames;
		frames = 0;
	    }

	    for (i = 0; i < strlen(debug); i++)
	    {
		font_char(character, debug[i]);
		for (x = 0; x < 8; x++)
		{
		    for(y = 0; y < 8; y++)
		    {
			if (character[(y * 8) + x] == 1)
			{
			    buffer[(y * WIDTH) + x + (i*8)] = 0xffffffff;
			}
		    }
		}
	    }



	    for (x = 10; x < 70; x++)
	    {
		for (y = 10; y < 70; y++)
		{
		    if (x != 40 && y != 40) {
			buffer[(y * WIDTH) + x] = 0xffffffff;
		    }
		}
	    }

	    buffer[((((int)camera.z) + 40) * WIDTH) + ((int)camera.x) + 40] = 0x00ff0000;


	    buffer[((((int)d1.r1.z) + 40) * WIDTH) + ((int)d1.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d1.r1.z) + 40) * WIDTH) + ((int)d1.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d1.r1.z) + 40) * WIDTH) + ((int)d1.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d2.r1.z) + 40) * WIDTH) + ((int)d2.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d2.r1.z) + 40) * WIDTH) + ((int)d2.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d2.r1.z) + 40) * WIDTH) + ((int)d2.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d3.r1.z) + 40) * WIDTH) + ((int)d3.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d3.r1.z) + 40) * WIDTH) + ((int)d3.r1.x) + 40] = 0x000000ff;
	    buffer[((((int)d3.r1.z) + 40) * WIDTH) + ((int)d3.r1.x) + 40] = 0x000000ff;

	    buffer[((((int)d1.r2.z) + 40) * WIDTH) + ((int)d1.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d1.r2.z) + 40) * WIDTH) + ((int)d1.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d1.r2.z) + 40) * WIDTH) + ((int)d1.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d2.r2.z) + 40) * WIDTH) + ((int)d2.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d2.r2.z) + 40) * WIDTH) + ((int)d2.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d2.r2.z) + 40) * WIDTH) + ((int)d2.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d3.r2.z) + 40) * WIDTH) + ((int)d3.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d3.r2.z) + 40) * WIDTH) + ((int)d3.r2.x) + 40] = 0x000000ff;
	    buffer[((((int)d3.r2.z) + 40) * WIDTH) + ((int)d3.r2.x) + 40] = 0x000000ff;

	    for (i = 0; i < polygon_set_len; i++)
	    {
		buffer[((((int)polygon_set[i].a.z) + 40) * WIDTH) + ((int)polygon_set[i].a.x) + 40] = 0x0000ff00;
		buffer[((((int)polygon_set[i].a.z) + 40) * WIDTH) + ((int)polygon_set[i].a.x) + 40] = 0x0000ff00;
		buffer[((((int)polygon_set[i].a.z) + 40) * WIDTH) + ((int)polygon_set[i].a.x) + 40] = 0x0000ff00;

		buffer[((((int)polygon_set[i].b.z) + 40) * WIDTH) + ((int)polygon_set[i].b.x) + 40] = 0x0000ff00;
		buffer[((((int)polygon_set[i].b.z) + 40) * WIDTH) + ((int)polygon_set[i].b.x) + 40] = 0x0000ff00;
		buffer[((((int)polygon_set[i].b.z) + 40) * WIDTH) + ((int)polygon_set[i].b.x) + 40] = 0x0000ff00;

		buffer[((((int)polygon_set[i].c.z) + 40) * WIDTH) + ((int)polygon_set[i].c.x) + 40] = 0x0000ff00;
		buffer[((((int)polygon_set[i].c.z) + 40) * WIDTH) + ((int)polygon_set[i].c.x) + 40] = 0x0000ff00;
		buffer[((((int)polygon_set[i].c.z) + 40) * WIDTH) + ((int)polygon_set[i].c.x) + 40] = 0x0000ff00;
	    }

	}

	
	process_input();
	state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

	if (state < 0) {
	    window = NULL;
	    break;
	}
    } while(mfb_wait_sync(window));
    return 0;
}

void process_input()
{
    if (keyboard_buffer['Q' - 0x20] == true)
    {
	camera.yaw += 1;
    }
    else if (keyboard_buffer['E' - 0x20] == true)
    {
	camera.yaw -= 1;
    }

    if (keyboard_buffer['W' - 0x20] == true)
    {
	camera.x -= sinf(camera.yaw * DTOR)/4.0f;
	camera.z += cosf(camera.yaw * DTOR)/4.0f;
    }
    else if (keyboard_buffer['S' - 0x20] == true)
    {
	camera.x += sinf(camera.yaw * DTOR)/4.0f;
	camera.z -= cosf(camera.yaw * DTOR)/4.0f;
    }

    if (keyboard_buffer['A' - 0x20] == true)
    {
	camera.x -= cosf(camera.yaw * DTOR)/4.0f;
	camera.z -= sinf(camera.yaw * DTOR)/4.0f;
    }
    else if (keyboard_buffer['D' - 0x20] == true)
    {
	camera.x += cosf(camera.yaw * DTOR)/4.0f;
	camera.z += sinf(camera.yaw * DTOR)/4.0f;
    }

    if (keyboard_buffer['Z' - 0x20] == true)
    {
	camera.y -= 0.5;
    }
    else if (keyboard_buffer['X' - 0x20] == true)
    {
	camera.y += 0.5;
    }
}


void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed)
{

    // Remember to close the window in some way
    if(key == KB_KEY_ESCAPE)
    {
        mfb_close(window);
    }

    keyboard_buffer[key - 0x20] = isPressed;
    last_moved = true;
}


int distance_comparison(const void *a, const void *b)
{
    vec3 va = *(vec3*) a;
    vec3 vb = *(vec3*) b;
    /* int conversion floors float values, therefore, if not scaled up, sorting
       polygons near to each other won't work properly. */
    return (int) (100.0f * (vec3_distance(vecd3_to_vec3(camera), va) - vec3_distance(vecd3_to_vec3(camera), vb)));
}
