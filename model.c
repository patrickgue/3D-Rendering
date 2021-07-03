#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

#include "model.h"

void load_model(char *filename, poly **poly_set, int *poly_set_len)
{
    FILE *file = fopen(filename, "r");
    int add_len;
    if (file == NULL)
    {
	fprintf(stderr, "Unable to load model file %s\n", filename);
	exit(1);
    }
    fread(&add_len, 1, sizeof(int), file);
    *poly_set = realloc(*poly_set, sizeof(poly) * (*poly_set_len + add_len + 1));
    fread(*poly_set + *poly_set_len, add_len, sizeof(poly) * add_len, file);
    *poly_set_len += add_len;
    
    fclose(file);
}
