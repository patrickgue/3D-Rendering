#include "text.h"

#include <string.h>

#include "font.h"


void font_char(char *dest, char c)
{
    char index = c - 0x20;
    memcpy(dest, font[index], 8 * 8);
}
