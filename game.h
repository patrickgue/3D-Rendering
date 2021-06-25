#ifndef _game_h
#define _game_h
    
#include <stdbool.h>
#include <MiniFB.h>

typedef unsigned char byte;

void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed);
void process_input();

#endif
