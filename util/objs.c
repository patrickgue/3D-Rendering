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

    obj_init(obj);
    
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

    if (debug_mode)
    {
	printf("STATUS\n-----\nvectors     %d\ncoordinates %d\nnormals     %d\npolygons    %d\n", obj->vectors_count, obj->texture_coordinates_count, obj->vector_normals_count, obj->polygons_count);
    }

    for (i = 0; i < obj->polygons_count; i++)
    {
	face f = obj->polygons[i];
	vec3 a = obj->vectors[f.a],
	     b = obj->vectors[f.d],
	     c = obj->vectors[f.g];
	fprintf(output_file, "%f,%f,%f\n", a.x, a.y, a.z);
	fprintf(output_file, "%f,%f,%f\n", b.x, b.y, b.z);
	fprintf(output_file, "%f,%f,%f\n", c.x, c.y, c.z);
	fprintf(output_file, "00ffffff\n");
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
    float val[10] = {
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f
    };
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
    if (debug_mode) {printf("%3f %3f %3f %3f %3f %3f\n", val[0], val[1], val[2], val[3], val[4], val[5]);}

    if (mode == VECTOR)
    {
	obj->vectors = realloc(obj->vectors, sizeof(vec3) * (obj->vectors_count + 1));
	obj->vectors[obj->vectors_count].x = val[0];
	obj->vectors[obj->vectors_count].y = val[1];
	obj->vectors[obj->vectors_count].z = val[2];
	obj->vectors_count++;
    }
    else if (mode == VECTOR_NORMAL)
    {
	obj->vector_normals = realloc(obj->vector_normals, sizeof(vec3) * (obj->vector_normals_count + 1));
	obj->vector_normals[obj->vector_normals_count].x = val[0];
	obj->vector_normals[obj->vector_normals_count].y = val[1];
	obj->vector_normals[obj->vector_normals_count].z = val[2];
	obj->vector_normals_count++;
    }
    else if (mode == TEXTURE_COORDS)
    {
	obj->texture_coordinates = realloc(obj->texture_coordinates, sizeof(vec3) * (obj->texture_coordinates_count + 1));
	obj->texture_coordinates[obj->texture_coordinates_count].x = val[0];
	obj->texture_coordinates[obj->texture_coordinates_count].y = val[1];
	obj->texture_coordinates[obj->texture_coordinates_count].z = val[2];
	obj->texture_coordinates_count++;
    }
    else if (mode == FACE)
    {
	obj->polygons = realloc(obj->polygons, sizeof(face) * (obj->polygons_count + 1));
	if (j - 1 == 3)
	{
	    obj->polygons[obj->polygons_count].a = (int) val[0];
	    obj->polygons[obj->polygons_count].d = (int) val[1];
	    obj->polygons[obj->polygons_count].g = (int) val[2];
	}
	else if (j - 1 == 6)
	{
	    if (str_contains(line, "//"))
	    {
		obj->polygons[obj->polygons_count].a = (int) val[0];
		obj->polygons[obj->polygons_count].c = (int) val[1];
		obj->polygons[obj->polygons_count].d = (int) val[2];
		obj->polygons[obj->polygons_count].f = (int) val[3];
		obj->polygons[obj->polygons_count].g = (int) val[4];
		obj->polygons[obj->polygons_count].i = (int) val[5];
	    }
	    else
	    {
		obj->polygons[obj->polygons_count].a = (int) val[0];
		obj->polygons[obj->polygons_count].b = (int) val[1];
		obj->polygons[obj->polygons_count].d = (int) val[2];
		obj->polygons[obj->polygons_count].e = (int) val[3];
		obj->polygons[obj->polygons_count].g = (int) val[4];
		obj->polygons[obj->polygons_count].h = (int) val[5];
	    }
	}
	else
	{
	    if (debug_mode) {printf("%d\n", j - 1);}

	    obj->polygons[obj->polygons_count].a = (int) val[0];
	    obj->polygons[obj->polygons_count].b = (int) val[1];
	    obj->polygons[obj->polygons_count].c = (int) val[2];
	    obj->polygons[obj->polygons_count].d = (int) val[3];
	    obj->polygons[obj->polygons_count].e = (int) val[4];
	    obj->polygons[obj->polygons_count].f = (int) val[5];
	    obj->polygons[obj->polygons_count].g = (int) val[6];
	    obj->polygons[obj->polygons_count].h = (int) val[7];
	    obj->polygons[obj->polygons_count].i = (int) val[8];
	}
	obj->polygons_count++;
    }
    
}


void obj_init(struct s_obj *obj)
{
    obj->polygons = malloc(0);
    obj->polygons_count = 0;
    obj->texture_coordinates = malloc(0);
    obj->texture_coordinates_count = 0;
    obj->vector_normals = malloc(0);
    obj->vector_normals_count = 0;
    obj->vectors = malloc(0);
    obj->vector_normals_count = 0;
}



bool str_contains(const char *base, const char *srch)
{
    char *cpy = strdup(base);
    int i, j;
    bool found = true;

    if (strlen(base) == 0 || strlen(srch) == 0 || strlen(srch) > strlen(base))
    {
	return false;
    }

    for (i = 0; i < strlen(base); i++)
    {
	if (base[i] == srch[0])
	{
	    for (j = 1; j < strlen(srch); j++)
	    {
		if (base[i + j] != srch[j])
		{
		    found = false;
		    break;
		}
	    }
	    break;
	}
    }
    
    free(cpy);
    return found;
}
