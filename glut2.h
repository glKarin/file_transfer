#ifndef GLUT2_H
#define GLUT2_H

#include "gl/mesh.h"
#include "gl/camera.h"

#include <QVector3D>
#include <GL/gl.h>


void printm(const GLfloat m[16]);
bool maketri(GLfloat r[9], const QVector3D &a, const QVector3D &b, const QVector3D &c);
void karinCube_MinMax(const GLfloat v1[3], const GLfloat v2[3], material_s *r);
void karinPlane_MinMax(const GLfloat v1[3], const GLfloat v2[3], material_s *r);

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
