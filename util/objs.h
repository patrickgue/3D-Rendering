#ifndef _objs_h
#define _objs_h

struct s_obj
{
    vec3 *vectors;
    int vectors_count;
    vec3 *texture_coordinates;
    int texture_coordinates_count;
    vec3 *vector_normals;
    int vector_normals_count;
    poly polygons;
    int polygons_count;
};

enum e_line_mode
{
    VECTOR,
    VECTOR_NORMAL,
    TEXTURE_COORDS,
    FACE
};


void usage();
void process_line(struct s_obj *obj, char *line);

#endif
