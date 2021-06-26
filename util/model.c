#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../vector.h"

#include "model.h"

int main(int argc, char **argv)
{
    FILE *input_file, *output_file;
    char line_buffer[64], number_buffer[32], cbuf;
    int i, line_count = 0, polygons_count = 0, vec_buff_index = 0;
    poly *polygons = malloc(0);
    vec3 vec_buff[3];
    
    
    if (argc < 3)
    {
	usage();
	exit(1);
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
	    printf("%2d: %s\n", line_count, line_buffer);
	    vec_buff[vec_buff_inde++] = parse_vec3(line_buffer);
	} 


    }
    
    
    

    fclose(input_file);
    fclose(output_file);
    
    return 0;
}


void usage()
{
    printf("usage: model infile.mdl outfile.bin\n");
}


vec3 parse_vec3(char *line)
{
    char num_buff[32];
    int i, v_ind = 0, num_ind = 0;
    vec3 p;
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
    p.z = atof(num_buff);
    return p;
}
