#ifndef _KARIN_MESH_H
#define _KARIN_MESH_H

#include "gl_std.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _texture2d_s
{
    GLuint tex_id;
    GLint width;
    GLint height;
    GLenum format;
} texture2d_s;

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
    int count;

    texture2d_s texture;
    GLfloat color[4];
    GLboolean use_color;
    GLenum face;

    GLfloat position[3];
    GLfloat rotation[3];
    //GLfloat scaler[3];
} material_s;

typedef struct _mesh_s
{
    struct _material_s *materials;
    int count;

    GLfloat position[3];
    GLfloat rotation[3];
    //GLfloat scaler[3];
} mesh_s;

void rendermesh(const mesh_s *mesh);
void rendermat(const material_s *mesh);
void freemesh(mesh_s *mesh);
void freemat(material_s *mat);
void newmesh(mesh_s *mesh, int count);
void newmat(material_s *mat, int count);
void matcpy(material_s * RESTRICT src, const material_s * RESTRICT dst);
void meshcpy(mesh_s * RESTRICT src, const mesh_s * RESTRICT dst);

int loadtex2d(texture2d_s *r, const void *data);

#ifdef __cplusplus
}
#endif

#endif // MESH_H
