#ifndef _objs_h
#define _objs_h

typedef struct s_face face;

struct s_face
{
    int a, b, c,
	d, e, f,
	g, h, i;
};

struct s_obj
{
    vec3 *vectors;
    int vectors_count;
    vec3 *texture_coordinates;
    int texture_coordinates_count;
    vec3 *vector_normals;
    int vector_normals_count;
    face *polygons;
    int polygons_count;
};

enum e_line_mode
{
    VECTOR,
    VECTOR_NORMAL,
    TEXTURE_COORDS,
    FACE
};




void obj_init(struct s_obj *);
void usage();
void process_line(struct s_obj *obj, char *line);
bool str_contains(const char *, const char *);

#endif
