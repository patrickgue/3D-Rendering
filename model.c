#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

#include "model.h"

void load_model(char *filename, poly_set *set)
{
    FILE *file = fopen(filename, "r");
    int add_len;
    vecd3 mov;
    char magic[4];
    if (file == NULL)
    {
	fprintf(stderr, "Unable to load model file %s\n", filename);
	exit(1);
    }
    fread(&magic, 3, sizeof(char), file);
    if (strcmp(magic, "mdl") != 0)
    {
	fprintf(stderr, "Invalid magic number \"%s\" try to read file anyways\n", magic);
    }
    fread(&add_len, 1, sizeof(int), file);
    fread(&set->mov, 1, sizeof(vecd3), file);
    set->polygons = malloc(sizeof(poly) * (add_len + 1));
    fread(set->polygons, add_len, sizeof(poly) * add_len, file);
    set->polygons_count = add_len;
    set->mov = mov;
    fclose(file);
}
