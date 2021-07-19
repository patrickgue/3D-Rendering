#ifndef _editor_h
#define _editor_h

#include <stdint.h>

#include "../vector.h"

typedef struct s_view_props view_props;
typedef enum e_app_mode app_mode;

struct s_view_props
{
    int x,
	y,
	zoom;
};

enum e_app_mode
{
    MODE_VIEW,
    MODE_EDIT
};




void draw_text(int, int, char *);
void draw_line(vec2, vec2, uint32_t, view_props, vec2);
void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed);


#endif
