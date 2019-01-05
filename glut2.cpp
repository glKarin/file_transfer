#include "glut2.h"
#include "karin_std.h"

#include <math.h>
#include <stdio.h>

#include <QDebug>

#define VEC3_X(x) ((x)[0])
#define VEC3_Y(x) ((x)[1])
#define VEC3_Z(x) ((x)[2])

static const GLushort CubePlane4Index[] =
{
    // 正
    0, 2, 1, 1, 2, 3,
    // 背
    4, 5, 6, 6, 5, 7,
    // 左
    4, 0, 5, 5, 0, 1,
    // 右
    2, 6, 3, 3, 6, 7,
    // 上
    1, 3, 5, 5, 3, 7,
    // 下
    0, 4, 2, 2, 4, 6
};

static const GLfloat CubePlane4Texcoord[] =
{
    // 正
    0.0, 1.0,  1.0, 1.0,  0.0, 0.0,  0,0, 1.0,  1.0, 1.0,  0.0, 0.0,
    // 背
    1.0, 1.0,  1.0, 0.0,  0.0, 0.0,  0.0, 0.0,  1.0, 0.0,  1.0, 1.0,
    // 左
    0.0, 0.0,  1.0, 0.0,  0.0, 1.0,  0.0, 1.0,  1.0, 0.0,  1.0, 1.0,
    // 右
    0.0, 0.0,  1.0, 1.0,  0.0, 1.0,  0.0, 1.0,  1.0, 1.0,  1.0, 0.0,
    // 上
    0.0, 0.0,  1.0, 0.0,  0.0, 1.0,  0.0, 1.0,  1.0, 0.0,  1.0, 1.0,
    // 下
    0.0, 1.0,  0.0, 0.0,  1.0, 1.0,  1.0, 1.0,  0.0, 0.0,  1.0, 0.0
};

static const GLfloat CubePlane4Normal[] =
{
    // 正
    0.0, 0.0, 1.0,
    // 背
    0.0, 0.0, -1.0,
    // 左
    -1.0, 0.0, 0.0,
    // 右
    1.0, 0.0, 0.0,
    // 上
    0.0, 1.0, 0.0,
    // 下
    0.0, -1.0, 0.0
};

#define FULL_COLOR_FACTOR 0.2
const GLfloat CubePlane4Color[] = {
        // 正
        FULL_COLOR_FACTOR, 0.0, 0.0, 1.0,
        // 背
        0.0, FULL_COLOR_FACTOR, FULL_COLOR_FACTOR, 1.0,
        // 左
        0.0, FULL_COLOR_FACTOR, 0.0, 1.0,
        // 右
        FULL_COLOR_FACTOR, 0.0, FULL_COLOR_FACTOR, 1.0,
        // 上
        0.0, 0.0, FULL_COLOR_FACTOR, 1.0,
        // 下
        FULL_COLOR_FACTOR, FULL_COLOR_FACTOR, 0.0, 1.0,
};

void printm(const GLfloat m[16])
{
    if(!m)
        return;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
            printf("%f  ", m[j * 4 + i]);
        printf("\n");
    }
}

// CCW is front
bool maketri(GLfloat r[9], const QVector3D &a, const QVector3D &b, const QVector3D &c)
{
#define TRI(pa, pb, pc) \
    if(tridir(pa, pb, pc) > 0) \
        points << &pa << &pb << &pc; \
    if(points.size()) \
        goto __Exit;
    //2S=|(OA-OB)×(OA-OC)|
    if(!r)
        return false;
    QList<const QVector3D *> points;
    TRI(a, b, c)
            TRI(a, c, b)
            TRI(b, a, c)
            TRI(b, c, a)
            TRI(c, a, b)
            TRI(c, b, a)

__Exit:
    for(int i = 0; i < points.size(); i++)
    {
        r[i * 3] = points.at(i)->x();
        r[i * 3 + 1] = points.at(i)->y();
        r[i * 3 + 2] = points.at(i)->z();
    }

    return points.size() == 3;
}


static void karinCubeVertex(const GLfloat v1[3], const GLfloat v2[3], GLfloat vertex[24])
{
    if(!v1 || !v2 || !vertex)
        return;
    GLfloat minx = VEC3_X(v1);
    GLfloat miny = VEC3_Y(v1);
    GLfloat minz = VEC3_Z(v1);
    GLfloat maxx = VEC3_X(v2);
    GLfloat maxy = VEC3_Y(v2);
    GLfloat maxz = VEC3_Z(v2);
    // 前左下 0
    vertex[0] = minx;
    vertex[1] = miny;
    vertex[2] = maxz;
    // 前左上 1
    vertex[3] = minx;
    vertex[4] = maxy;
    vertex[5] = maxz;
    // 前右下 2
    vertex[6] = maxx;
    vertex[7] = miny;
    vertex[8] = maxz;
    // 前右上 3
    vertex[9] = maxx;
    vertex[10] = maxy;
    vertex[11] = maxz;
    // 后左下 4
    vertex[12] = minx;
    vertex[13] = miny;
    vertex[14] = minz;
    // 后左上 5
    vertex[15] = minx;
    vertex[16] = maxy;
    vertex[17] = minz;
    // 后右下 6
    vertex[18] = maxx;
    vertex[19] = miny;
    vertex[20] = minz;
    // 后右上 7
    vertex[21] = maxx;
    vertex[22] = maxy;
    vertex[23] = minz;
}


void karinCube_MinMax(const GLfloat v1[3], const GLfloat v2[3], material_s *r)
{
    GLfloat v[24];
    point_s *p;

    if(!r)
        return;

    karinCubeVertex(v1, v2, v);

    int i;
    int count = countof(CubePlane4Index);
    newmat(r, count);
    for(i = 0; i < count; i++)
    {
        p = r->points + i;
        p->vertex[0] = v[CubePlane4Index[i] * 3];
        p->vertex[1] = v[CubePlane4Index[i] * 3 + 1];
        p->vertex[2] = v[CubePlane4Index[i] * 3 + 2];
        p->vertex[3] = 1.0;
        if(1)
        {
            p->normal[0] = CubePlane4Normal[i / 6 * 3];
            p->normal[1] = CubePlane4Normal[i / 6 * 3 + 1];
            p->normal[2] = CubePlane4Normal[i / 6 * 3 + 2];
        }
        if(1)
        {
            p->texcoord[0] = CubePlane4Texcoord[i * 2];
            p->texcoord[1] = CubePlane4Texcoord[i * 2 + 1];
        }
        if(1)
        {
            p->color[0] = CubePlane4Color[i / 6 * 4];
            p->color[1] = CubePlane4Color[i / 6 * 4 + 1];
            p->color[2] = CubePlane4Color[i / 6 * 4 + 2];
            p->color[3] = CubePlane4Color[i / 6 * 4 + 3];
        }
    }
}

void karinPlane_MinMax(const GLfloat v1[3], const GLfloat v2[3], material_s *r)
{
    point_s *p;
    if(!r || !v1 || !v2)
        return;

    // always up
    const GLfloat avg_y = (VEC3_Y(v1) + VEC3_Y(v2)) / 2.0;

    const GLfloat vertex[] = {
        VEC3_X(v1), avg_y, VEC3_Z(v1),

        VEC3_X(v2), avg_y, VEC3_Z(v1),
        VEC3_X(v1), avg_y, VEC3_Z(v2),

        VEC3_X(v1), avg_y, VEC3_Z(v2),
        VEC3_X(v2), avg_y, VEC3_Z(v1),

        VEC3_X(v2), avg_y, VEC3_Z(v2),
    };

    const GLfloat texcoord[] = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,

        1.0, 0.0,
        0.0, 1.0,

        1.0, 1.0,
    };

    const GLfloat normal[] = {
        0.0, 1.0, 0.0,
    };

    const GLfloat color[] = {
        1.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 1.0,

        0.0, 0.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 1.0,

        0.0, 1.0, 0.0, 1.0,
    };

    int i;
    newmat(r, 6);
    for(i = 0; i < 6; i++)
    {
        p = r->points + i;
        p->vertex[0] = vertex[i * 3];
        p->vertex[1] = vertex[i * 3 + 1];
        p->vertex[2] = vertex[i * 3 + 2];
        p->vertex[3] = 1.0;

        if(1)
        {
            p->texcoord[0] = texcoord[i * 2];
            p->texcoord[1] = texcoord[i * 2 + 1];
        }
        if(1)
        {
            p->color[0] = color[i * 4];
            p->color[1] = color[i * 4 + 1];
            p->color[2] = color[i * 4 + 2];
            p->color[3] = color[i * 4 + 3];
        }
        if(1)
        {
            p->normal[0] = normal[0];
            p->normal[1] = normal[1];
            p->normal[2] = normal[2];
        }
    }
}

int tridir(const QVector3D &a, const QVector3D &b, const QVector3D &c)
{
    float m00 = a.x();
    float m01 = a.y();
    float m02 = a.z();

    float m10 = b.x();
    float m11 = b.y();
    float m12 = b.z();

    float m20 = c.x();
    float m21 = c.y();
    float m22 = c.z();

    float f =
            m00 * (m11 * m22 - m12 * m21)
                + m01 * (m12 * m20 - m10 * m22)
                + m02 * (m10 * m21 - m11 * m20);

    return f > 0 ? 1 : f < 0 ? -1 : 0;
}


void boxcenter(GLfloat r[3], const material_s *mat)
{
    GLfloat maxx, maxy, maxz;
    GLfloat minx, miny, minz;
    GLfloat x, y, z;
    point_s *p;
    int i;

    if(!r || !mat || !mat->count)
        return;

    p = mat->points;
    maxx = minx = p->vertex[0] / p->vertex[3];
    maxy = miny = p->vertex[1] / p->vertex[3];
    maxz = minz = p->vertex[2] / p->vertex[3];
    for(i = 1; i < mat->count; i++)
    {
        p = mat->points + i;
        x = p->vertex[0] / p->vertex[3];
        y = p->vertex[1] / p->vertex[3];
        z = p->vertex[2] / p->vertex[3];
        if(x < minx) minx = x; else if(x > maxx) maxx = x;
        if(y < miny) miny = y; else if(y > maxy) maxy = y;
        if(z < minz) minz = z; else if(z > maxz) maxz = z;
    }
    r[0] = (maxx + minx) / 2;
    r[1] = (maxy + miny) / 2;
    r[2] = (maxz + minz) / 2;
}
