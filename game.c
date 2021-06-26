#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <MiniFB.h>

#include "vector.h"
#include "text.h"
#include "game.h"

#define WIDTH 320
#define HEIGHT 200

uint32_t *buffer;
poly *poly_set;
int poly_set_len = 3;
vecd3 camera = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
bool keyboard_buffer[0x60];

int main(int argc, char **argv)
{
    int x, y, i, frames = 0, fps = 0;
    vec3 answ, c1, c2, c1r, c2r;
    vec3_ray camera_ray, d1, d2, d3;
    bool res;
    time_t last_sec = 0;
    char character[64],debug[128];
    bool debug_mode = false;

    float d_max = 30.0f, d_min = 3.0f, f_near = 0.01f, f_far = 0.1f, fx, fy; 
    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, WF_RESIZABLE);
    if (!window)
	return 0;

    if (argc > 1)
    {
	if (strcmp(argv[1], "-d") == 0)
	{
	    debug_mode = true;
	}
    }

    
    mfb_set_keyboard_callback(window, keyboard);
    memset(keyboard_buffer, 0, 0x60);

    buffer = (uint32_t *) malloc(WIDTH * HEIGHT * 4);
    poly_set = (poly*) malloc(sizeof(poly) * 12);

    poly_set[2] = (poly) {
	{00.0f,00.0f,10.0f},
	{01.0f,00.0f,10.0f},
	{01.0f,01.0f,10.0f},
	0x00ff0000
    };

    poly_set[1] = (poly) {
	{01.0f,01.0f,10.0f},
	{00.0f,01.0f,10.0f},
	{00.0f,00.0f,10.0f},
	0x0000ff00
    };

    poly_set[0] = (poly) {
	{-5.0f,-1.0f,15.0f},
	{ 5.0f,-1.0f,15.0f},
	{ 0.0f, 5.0f,15.0f},
	0x000000ff
    };

  
    do
    {
	int state;

	for (x = 0; x < WIDTH; x++)
	{
	    for (y = 0; y < HEIGHT; y++)
	    {
		fx = sinf(camera.yaw * DTOR);
		fy = cosf(camera.yaw * DTOR);

		
		c1 = vec3_rotate_y(vecd3_to_vec3(camera), vec3_add(
				       vec3_add(
					   vecd3_to_vec3(camera),
					   (vec3) {0.0f,0.0f,d_min}),
				       (vec3) {f_near * (x - (WIDTH/2)), 0, 0}
				       ), camera.yaw);
		c1.y = (f_near * -1 * (y - (HEIGHT/2)));
		
		c2 = vec3_rotate_y(vecd3_to_vec3(camera), vec3_add(
				       vec3_add(
					   vecd3_to_vec3(camera), 
					   (vec3) {0.0f,0.0f,d_max}),
				       (vec3) {f_far * (x - (WIDTH/2)), 0, 0}
				       ), camera.yaw);
		c2.y = (f_far * -1 * (y - (HEIGHT/2)));
		
		camera_ray = (vec3_ray) {c1, c2};
		buffer[(y * WIDTH) + x] = 0;
	      
		for (i = 0; i < poly_set_len; i++)
		{
		    res = find_intersection(camera_ray, poly_set[i], &answ);
		    if (res)
		    {
			buffer[(y * WIDTH) + x] = poly_set[i].color;
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
		
		/*if (res)
		  {
		  printf("[%03d/%03d] %f:%f:%f\n", x,y, answ.x, answ.y, answ.z);
		  }*/
	    }
	}

	if (debug_mode)
	{

	    sprintf(debug, "FPS: %d X: %2.1f Y: %2.1f Z: %2.1f YAW: %2.1f", fps, camera.x, camera.y, camera.z, camera.yaw);
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

	    for (i = 0; i < poly_set_len; i++)
	    {
		buffer[((((int)poly_set[i].a.z) + 40) * WIDTH) + ((int)poly_set[i].a.x) + 40] = 0x0000ff00;
		buffer[((((int)poly_set[i].a.z) + 40) * WIDTH) + ((int)poly_set[i].a.x) + 40] = 0x0000ff00;
		buffer[((((int)poly_set[i].a.z) + 40) * WIDTH) + ((int)poly_set[i].a.x) + 40] = 0x0000ff00;

		buffer[((((int)poly_set[i].b.z) + 40) * WIDTH) + ((int)poly_set[i].b.x) + 40] = 0x0000ff00;
		buffer[((((int)poly_set[i].b.z) + 40) * WIDTH) + ((int)poly_set[i].b.x) + 40] = 0x0000ff00;
		buffer[((((int)poly_set[i].b.z) + 40) * WIDTH) + ((int)poly_set[i].b.x) + 40] = 0x0000ff00;

		buffer[((((int)poly_set[i].c.z) + 40) * WIDTH) + ((int)poly_set[i].c.x) + 40] = 0x0000ff00;
		buffer[((((int)poly_set[i].c.z) + 40) * WIDTH) + ((int)poly_set[i].c.x) + 40] = 0x0000ff00;
		buffer[((((int)poly_set[i].c.z) + 40) * WIDTH) + ((int)poly_set[i].c.x) + 40] = 0x0000ff00;
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
}


void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed)
{

    // Remember to close the window in some way
    if(key == KB_KEY_ESCAPE)
    {
        mfb_close(window);
    }

    keyboard_buffer[key - 0x20] = isPressed;
}
