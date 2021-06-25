#ifndef _vector_h
#define _vector_h

#include <stdbool.h>
#include <stdint.h>

#define DTOR M_PI/180.0f


typedef struct s_vector3 vec3;
typedef struct s_vector_dir3 vecd3;
typedef struct s_vector3_ray vec3_ray;
typedef struct s_polygon poly;

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
};


vec3 vec3_crossproduct(vec3, vec3);
float vec3_dotproduct(vec3, vec3);

vec3 vec3_add(vec3,vec3);
vec3 vec3_sub(vec3,vec3);
vec3 vec3_sum(vec3,vec3);
vec3 vec3_div(vec3,vec3);
float vec3_len(vec3);
vec3 vec3_rotate_y(vec3 c, vec3 v, float deg);
vec3 vecd3_to_vec3(vecd3 v);

bool find_intersection(vec3_ray, poly, vec3 *);


#endif
