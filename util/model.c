#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../vector.h"

#include "model.h"

int main(int argc, char **argv)
{
    FILE *input_file, *output_file;
    char line_buffer[64], number_buffer[32], cbuf;
    int i, line_count = 0, polygons_count = 0, vec_buff_index = 0;
    poly *polygons = malloc(0);
    vecd3 vec_buff[3], mov = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    bool debug_mode = false;
    
    
    if (argc < 3)
    {
	usage();
	exit(1);
    }

    if (argc == 4 && strcmp(argv[3], "-d") == 0)
    {
	debug_mode = true;
    }

    input_file = fopen(argv[1], "r");
    output_file = fopen(argv[2], "w");
    
    if (input_file == NULL)
    {
	fprintf(stderr, "Error opening input file %s\n", argv[1]);
	exit(1);
    }

    if (output_file == NULL)
    {
	fprintf(stderr, "Error opening output file %s\n", argv[2]);
	exit(1);
    }

    i = 0;
    while((cbuf = fgetc(input_file)) != EOF)
    {
        line_buffer[i] = cbuf;
	i++;
	if (cbuf == '\n' || cbuf == 0)
	{
	    line_count++;
	    
	    line_buffer[i-1] = 0;
	    i = 0;
	    if (debug_mode)
	    {
		printf("%2d: %s\n", line_count, line_buffer);
	    }

	    /* first line is relative position */
	    if (line_count == 1 && line_buffer[0] == 'R')
	    {
		printf("%s\n", line_buffer);
		mov = parse_vec3(line_buffer + 1);
	    }
	    else
	    {
		if (vec_buff_index < 3)
		{
		    vec_buff[vec_buff_index++] = parse_vec3(line_buffer);
		}
		else {
		    polygons = realloc(polygons, sizeof(poly) * (polygons_count + 1));
		    polygons[polygons_count].a = vecd3_to_vec3(vec_buff[0]);
		    polygons[polygons_count].b = vecd3_to_vec3(vec_buff[1]);
		    polygons[polygons_count].c = vecd3_to_vec3(vec_buff[2]);
		    polygons[polygons_count].color = strtol(line_buffer, NULL, 16);
		    polygons_count++;
		    vec_buff_index = 0;
		}
	    }
	}


    }

    if (debug_mode)
    {
	for(i = 0; i < polygons_count; i++)
	{
	    printf("A: %2.1f / %2.1f / %2.1f\nB: %2.1f / %2.1f / %2.1f\nC: %2.1f / %2.1f / %2.1f\n%08x\n\n",
		   polygons[i].a.x, polygons[i].a.y, polygons[i].a.z,
		   polygons[i].b.x, polygons[i].b.y, polygons[i].b.z,
		   polygons[i].c.x, polygons[i].c.y, polygons[i].c.z, polygons[i].color);
	}
    }

    fwrite("mdl", 3, sizeof(char), output_file);
    fwrite(&polygons_count, 1, sizeof(int), output_file);
    fwrite(&mov, 1, sizeof(vecd3), output_file);
    fwrite(polygons, polygons_count, sizeof(poly), output_file);

    fclose(input_file);
    fclose(output_file);
    
    return 0;
}


void usage()
{
    printf("usage: model infile.mdl outfile.bin\n");
}


vecd3 parse_vec3(char *line)
{
    char num_buff[32];
    int i, v_ind = 0, num_ind = 0;
    vecd3 p;
    for (i = 0; i < strlen(line); i++)
    {
	if (line[i] == ',')
	{
	    num_buff[num_ind] = 0;
	    switch (v_ind)
	    {
	    case 0:
		p.x = atof(num_buff);
		break;
	    case 1:
		p.y = atof(num_buff);
		break;
	    case 2:
		p.z = atof(num_buff);
		break;
	    case 3:
		p.pit = atof(num_buff);
		break;
	    case 4:
		p.yaw = atof(num_buff);
		break;
	    }
	    v_ind++;
	    num_ind = 0;
	}
	else
	{
	    num_buff[num_ind++] = line[i];
	}
    }
    num_buff[num_ind] = 0;
    if (num_ind == 6)
    {
	p.rol = atof(num_buff);
    }
    else
    {
	p.z = atof(num_buff);
    }
    return p;
}
