#ifndef _game_h
#define _game_h
    
#include <stdbool.h>
#include <MiniFB.h>

#include "vector.h"

typedef unsigned char byte;

void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed);
void process_input();
void load_model(char *, poly **, int *);

#endif
