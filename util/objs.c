#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../vector.h"

#include "objs.h"

#define streq (a, b) (strcmp(a, b) == 0) 

bool debug_mode = false;

int main(int argc, char **argv)
{
    FILE *input_file = stdin, *output_file = stdout;
    int i, j;
    char line_buffer[128], c;
    struct s_obj *obj = malloc(sizeof(struct s_obj));
    
    for (i = 1, j = 0; i < argc; i++)
    {
	if (strcmp(argv[i], "-d") == 0)
	{
	    debug_mode = true;
	}
	else if(strcmp(argv[i], "-h") == 0)
	{
	    usage();
	    exit(0);
	}
	else
	{
	    switch(j++)
	    {
	    case 0:
		input_file = fopen(argv[i], "r");
		break;
	    case 1:
		output_file = fopen(argv[i], "w");
		break;
	    }
	}
    }

    i = 0;
    while ((c = fgetc(input_file)) != EOF)
    {
	line_buffer[i++] = c;
	if (c == '\n')
	{
	    line_buffer[i] = 0;
	    i = 0;
	    process_line(obj, line_buffer);
	}
    }

    
    return 0;
}


void usage()
{
    fprintf(stderr, "usage: objs <objs file> [<mdl file>]\n");
    fprintf(stderr, "       objs (from stdin to stdout)\n");
}

bool char_is_digit(char c)
{
    return ((c >= '0' && c <= '9') || c == '-' || c == '.');
}

void process_line(struct s_obj *obj, char *line)
{
    char number_buffer[64];
    int i, j, k;
    float val[7] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    enum e_line_mode mode;
    
    if (strlen(line) == 0 || line[0] == '#')
    {
	return;
    }

    if (line[0] == 'v')
    {
	if (line[1] == ' ')
	    mode = VECTOR;
	if (line[1] == 'n')
	    mode = VECTOR_NORMAL;
	if (line[1] == 't')
	    mode = TEXTURE_COORDS;
    }
    else if (line[0] == 'f')
    {
	mode = FACE;
    }

    j = 0, k = 0;
    for (i = 0; i < strlen(line); i++)
    {
	if (!char_is_digit(line[i]))
	{
	    if (j > 0)
	    {
		if (debug_mode) {printf("%s %d\n", number_buffer, j);}
		val[j - 1] = strtof(number_buffer, NULL);
	    }
	    j++, k = 0;
	    while(i < strlen(line) && !char_is_digit(line[i]))
		i++;
	    	    
	}
	if (char_is_digit(line[i]))
	{
	    number_buffer[k] = line[i];
	    number_buffer[k+1] = 0;
	    k++;
	}
    }
    if (debug_mode) {printf("%s %d *\n", number_buffer, j);}
    val[j - 1] = strtof(number_buffer, NULL);
    if (debug_mode) {printf("%3f %3f %3f %3f %3f %3f\n", val[0], val[1], val[2], val[3], val[4], val[5]);}

    if (mode == VECTOR)
    {
	obj->vectors = realloc(obj->vectors, sizeof(vec3) * (obj->vectors_count + 1));
	obj->vectors[obj->vectors_count++].x = val[0];
	obj->vectors[obj->vectors_count++].y = val[1];
	obj->vectors[obj->vectors_count++].z = val[2];
    }
    
}
