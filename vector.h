#ifndef _vector_h
#define _vector_h

#include <stdbool.h>
#include <stdint.h>

#define DTOR 0.017453293f

#define vecd3_to_vec3(v) ((vec3) {v.x, v.y, v.z})
#define vec3_to_vecd3(v) ((vecd3) {v.x, v.y, v.z,0,0,0})

typedef struct s_vector3 vec3;
typedef struct s_vector_dir3 vecd3;
typedef struct s_vector3_ray vec3_ray;
typedef struct s_polygon poly;
typedef struct s_vector2 vec2;
typedef struct s_polygon2 poly2;
typedef struct s_polygon_set poly_set;

struct s_vector3
{
    float x, y, z;
};

struct s_vector_dir3
{
    float x, y, z;
    float pit, yaw, rol;
};

struct s_vector3_ray
{
    vec3 r1, r2;
};

struct s_polygon
{
    vec3 a, b, c;
    uint32_t color;
    bool render;
};

struct s_polygon_set
{
    poly *polygons;
    int polygons_count;
    vecd3 mov;
};

struct s_vector2
{
    float x, y;
};

struct s_polygon2
{
    vec2 a, b, c;
};


vec3 vec3_crossproduct(vec3, vec3);
float vec3_dotproduct(vec3, vec3);

vec3 vec3_add(vec3,vec3);
vec3 vec3_sub(vec3,vec3);
vec3 vec3_sum(vec3,vec3);
vec3 vec3_div(vec3,vec3);
vec3 vec3_mul_scalar(float, vec3);
float vec3_len(vec3);
vec3 vec3_rotate_y(vec3 c, vec3 v, float deg);
vec3 poly_center(poly);
vec3 vec3_abs(vec3);
float vec3_distance(vec3, vec3);

poly poly_transform(poly);

vec2 vec2_sub(vec2, vec2);
vec2 vec2_cpy(vec2);

bool find_intersection(vec3_ray, poly, vec3 *);


#endif
