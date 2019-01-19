#ifndef SHADOW_H
#define SHADOW_H

#include "vector3.h"
#include "gl_std.h"
#include "mesh.h"
#include "mesa_gl_math.h"

vector3_s shadowlightingdir(const GLfloat v[3], const vector3_s *lightpos, bool dirlight);

void shadowcaletrans(material_s *r, const material_s *src, const GLmatrix *mat);
void shadowcalevol(mesh_s *r, const vector3_s *lightpos, const material_s *mat, bool dirlighting);
void shadowrvol(const mesh_s *cube, const vector3_s *lpos, bool dirlighting);
void shadowrmesh(const mesh_s *mesh, const vector3_s *lightpos, bool dirlighting = false, int start = 0, int count = -1);
void shadowrmask(const GLfloat color[4]);

#endif // SHADOW_H
