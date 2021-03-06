#include <math.h>
#include <stdbool.h>

#include "vector.h"

inline vec3 vec3_crossproduct(vec3 A, vec3 B)
{
    return (vec3) {A.y*B.z-A.z*B.y, A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x};
}

inline float vec3_dotproduct(vec3 a, vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline vec3 vec3_add(vec3 a, vec3 b)
{
    return (vec3) {
	a.x + b.x,
	a.y + b.y,
	a.z + b.z
    };
}

inline vec3 vec3_sub(vec3 a, vec3 b)
{
    vec3 p;
    p.x = a.x - b.x;
    p.y = a.y - b.y;
    p.z = a.z - b.z;
    return p;
}

inline vec3 vec3_sum(vec3 a, vec3 b)
{
    vec3 p;
    p.x = a.x * b.x;
    p.y = a.y * b.y;
    p.z = a.z * b.z;
    return p;
}

inline vec3 vec3_div(vec3 a, vec3 b)
{
    vec3 p;
    p.x = a.x / b.x;
    p.y = a.y / b.y;
    p.z = a.z / b.z;
    return p;
}


inline float vec3_len(vec3 a)
{
    return cbrtf( (a.x*a.x) + (a.y*a.y) + (a.z*a.z) );
}


vec3 vec3_mul_scalar(float a, vec3 B)
{
    return (vec3) {a*B.x, a*B.y, a*B.z};
}

inline vec3 vec3_abs(vec3 v)
{
    return (vec3) {
	fabs(v.x),
	fabs(v.y),
	fabs(v.z)
    };
}

inline float vec3_distance(vec3 a, vec3 b)
{
    return vec3_len(vec3_abs(vec3_sub(a,b)));
}

inline vec3 poly_center(poly polygon)
{
    return (vec3) {
	(polygon.a.x + polygon.b.x + polygon.c.x) / 3.0f,
	(polygon.a.y + polygon.b.y + polygon.c.y) / 3.0f,
	(polygon.a.z + polygon.b.z + polygon.c.z) / 3.0f
    };
}

vec3 poly_set_center(poly_set set)
{
    vec3 center = {0.0f, 0.0f, 0.0f}, tmp;
    int i;
    for (i = 0; i < set.polygons_count; i++)
    {
	tmp = poly_center(set.polygons[0]);
	center = vec3_add(center, tmp);
    }
    return (vec3) {
	tmp.x / set.polygons_count,
	tmp.y / set.polygons_count,
	tmp.z / set.polygons_count
    };
}

inline vec3 vec3_rotate_y(vec3 vc, vec3 v, float deg)
{
    vec3 p = v;
    p.y = vc.y + v.y;
    float s = sinf(deg * DTOR);
    float c = cosf(deg * DTOR);

    // translate point back to origin:
    p.x -= vc.x;
    p.z -= vc.z;

    // rotate point
    float xnew = p.x * c - p.z * s;
    float znew = p.x * s + p.z * c;

    // translate point back:
    p.x = xnew + vc.x;
    p.z = znew + vc.z;
    return p;
}

/* vec3 vecd3_to_vec3(vecd3 v)
{
    return (vec3) {v.x, v.y, v.z};
    } */

poly poly_transform(poly a, vec3 center, vecd3 mod)
{
    poly p = a;
    
    p.a = vec3_add(p.a, vecd3_to_vec3(mod));
    p.b = vec3_add(p.b, vecd3_to_vec3(mod));
    p.c = vec3_add(p.c, vecd3_to_vec3(mod));

    if (mod.yaw != 0)
    {
	p.a = vec3_rotate_y(p.a, center, mod.yaw);
	p.b = vec3_rotate_y(p.b, center, mod.yaw);
	p.c = vec3_rotate_y(p.c, center, mod.yaw);
    }
    return p;
}

inline bool find_intersection(vec3_ray ray, poly polygon, vec3 *answer)
{
    const float EPSILON = 0.0001;

    vec3 rayOrigin = ray.r1;
    vec3 rayVector = ray.r2;
    
    vec3 vertex0 = polygon.a;
    vec3 vertex1 = polygon.b;  
    vec3 vertex2 = polygon.c;
    vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    
    edge1 = vec3_sub(vertex1, vertex0);
    edge2 = vec3_sub(vertex2, vertex0);

    h = vec3_crossproduct(rayVector, edge2);
    a = vec3_dotproduct(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = vec3_sub(rayOrigin, vertex0);
    u = f * vec3_dotproduct(s,h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = vec3_crossproduct(s, edge1);
    v = f * vec3_dotproduct(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * vec3_dotproduct(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        *answer = vec3_add(rayOrigin, vec3_mul_scalar(t, rayVector));
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}


vec2 vec2_sub(vec2 a, vec2 b)
{
    return (vec2) {a.x - b.x, a.y - b.y};
}


vec2 vec2_cpy(vec2 a)
{
    return (vec2) {a.x, a.y};
}
