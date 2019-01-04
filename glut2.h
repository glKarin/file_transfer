#ifndef GLUT2_H
#define GLUT2_H

#include <QVector3D>
#include <GL/gl.h>

enum Direction_e
{
    Direction_Forward = 0,
    Direction_Backward,
    Direction_Left,
    Direction_Right,
    Direction_Up,
    Direction_Down,
    Direction_Total
};

enum Rotation_e
{
    Rotation_Left = 0,
    Rotation_Right,
    Rotation_Up,
    Rotation_Down,
    Rotation_Total
};

#define VERTEX_ELEMENT_COUNT 4
typedef struct _point_s
{
    GLfloat vertex[VERTEX_ELEMENT_COUNT];
    GLfloat normal[3];
    GLfloat texcoord[2];
    GLfloat color[4];
} point_s;

typedef struct _material_s
{
    struct _point_s *points;
    GLuint texture;
    GLfloat color[4];
    GLboolean use_color;
    GLenum face;
    int count;
} material_s;

typedef struct _mesh_s
{
    struct _material_s *materials;
    int count;
} mesh_s;

void rendermesh(mesh_s *mesh);
void rendermat(material_s *mesh);
void freemesh(mesh_s *mesh);
void freemat(material_s *mat);
void newmesh(mesh_s *mesh, int count);
void newmat(material_s *mat, int count);

void printm(const GLfloat m[16]);
bool maketri(GLfloat r[9], const QVector3D &a, const QVector3D &b, const QVector3D &c);
void karinCube_MinMax(const GLfloat v1[3], const GLfloat v2[3], material_s *r);
void karinPlane_MinMax(const GLfloat v1[3], const GLfloat v2[3], material_s *r);

float clampangle(float angle);
int tridir(const QVector3D &a, const QVector3D &b, const QVector3D &c); // > 0 CCW < 0 CW =0
void boxcenter(GLfloat r[3], const material_s *mat);

#define fv4toqvec3(v) QVector3D(v[0] / v[3], v[1] / v[3], v[2] / v[3])
#define fv3toqvec3(v) QVector3D(v[0], v[1], v[2])
#define qvec3tofv3(v, qv) \
{ \
    v[0] = qv.x(); \
    v[1] = qv.y(); \
    v[2] = qv.z(); \
}

#endif // GLUT2_H
