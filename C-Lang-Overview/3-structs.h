#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct vector2f
{
    float x;
    float y;
};

struct vector3f
{
    float x;
    float y;
    float z;
};

typedef struct vector3f vector3f;

struct vector2f vec2_add(struct vector2f v1, struct vector2f v2);
vector3f vec3_add(vector3f v1, vector3f v2);
void vec3_print(vector3f v);

#endif // __STRUCTS_H__
