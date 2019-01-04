#ifndef QMATH_H
#define QMATH_H

#include <QVector3D>
#include <GL/gl.h>

#include "vector3.h"

#define VEC3_X(x) ((x)[0])
#define VEC3_Y(x) ((x)[1])
#define VEC3_Z(x) ((x)[2])
#define VEC3_W(x) ((x)[3])

inline void vector3_s_to_GLfloatv(GLfloat *r, const vector3_s &v)
{
    if(!r)
        return;
    //memcpy(r, v.v, sizeof(float) * 3);
    VEC3_X(r) = VECTOR3_X(v);
    VEC3_Y(r) = VECTOR3_Y(v);
    VEC3_Z(r) = VECTOR3_Z(v);
    VEC3_W(r) = 1;
}

inline vector3_s Vector3D(float x, float y, float z)
{
    vector3_s r = { {x, y, z} };
    return r;
}

inline vector3_s Vector3D(const float v[3])
{
    vector3_s r = { {0, 0, 0} };
    if(v)
    {
        VECTOR3_X(r) = VEC3_X(v);
        VECTOR3_Y(r) = VEC3_Y(v);
        VECTOR3_Z(r) = VEC3_Z(v);
    }
    return r;
}

inline vector3_s Vector3D(float x, float y, float z, float w)
{
    vector3_s r = { {x, y, z} };
    if(w != 0)
        vector3_scale_self(&r, 1 / w);
    return r;
}

inline vector3_s Vector4D(const float v[4])
{
    vector3_s r = { {0, 0, 0} };
    if(v)
    {
        VECTOR3_X(r) = VEC3_X(v);
        VECTOR3_Y(r) = VEC3_Y(v);
        VECTOR3_Z(r) = VEC3_Z(v);
        if(VEC3_W(v) != 0)
            vector3_scale_self(&r, 1 / VEC3_W(v));
    }
    return r;
}

inline QVector3D vector3_s_to_QVector3D(const vector3_s &v)
{
    return QVector3D(VECTOR3_X(v), VECTOR3_Y(v), VECTOR3_Z(v));
}

inline vector3_s vector3_s_to_QVector3D(const QVector3D &v)
{
    vector3_s r = { {v.x(), v.y(), v.z()} };
    return r;
}



// line_s
typedef struct _line_s
{
    vector3_s a;
    vector3_s b;
    bool operator==(const _line_s &o)
    {
        return((vector3_equals(&a, &o.a) == 0 && vector3_equals(&b, &o.b) == 0)
               || (vector3_equals(&a, &o.b) == 0 && vector3_equals(&b, &o.a) == 0));
    }
    template <int I> void set(const GLfloat v[3])
    {
        if(!v)
            return;
        vector3_s &p = I == 0 ? a : b;
        VECTOR3_X(p) = VEC3_X(v);
        VECTOR3_Y(p) = VEC3_Y(v);
        VECTOR3_Z(p) = VEC3_Z(v);
    }
    template <int I> void get(GLfloat v[3]) const
    {
        if(!v)
            return;
        vector3_s &p = I == 0 ? a : b;
        VEC3_X(v) = VECTOR3_X(p);
        VEC3_Y(v) = VECTOR3_Y(p);
        VEC3_Z(v) = VECTOR3_Z(p);
    }
} line_s;

#endif // QMATH_H
