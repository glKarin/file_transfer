#include "mesh.h"
#include "gl/gl2.h"

#include <stdlib.h>
#include <string.h>

#ifdef _OS_WIN32
#ifndef __cplusplus
typedef enum
{
    false = 0,
    true
} bool;
#endif
#else
#include <stdbool.h>
#endif

void freemesh(mesh_s *mesh)
{
    int i;
    if(!mesh)
        return;
    for(i = 0; i < mesh->count; i++)
        freemat(mesh->materials + i);
    free(mesh->materials);
    mesh->count = 0;
}

void freemat(material_s *mat)
{
    if(!mat)
        return;
    free(mat->points);
    if(glIsTexture(mat->texture.tex_id))
        glDeleteTextures(1, &mat->texture.tex_id);
    memset(&mat->texture, 0, sizeof(texture2d_s));
    mat->count = 0;
}

void newmesh(mesh_s *mesh, int count)
{
    if(!mesh || count <= 0)
        return;
    mesh->count = count;
    mesh->materials = (material_s *)calloc(mesh->count, sizeof(material_s));
}

void newmat(material_s *mat, int count)
{
    if(!mat || count <= 0)
        return;
    mat->count = count;
    mat->points = (point_s *)calloc(mat->count, sizeof(point_s));
    memset(&mat->texture, 0, sizeof(texture2d_s));
    mat->color[0] = mat->color[1] = mat->color[2] = mat->color[3] = 1.0;
    mat->use_color = true;
}

void rendermesh(const mesh_s *mesh)
{
    int i;
    if(!mesh)
        return;
    for(i = 0; i < mesh->count; i++)
    {
        rendermat(mesh->materials + i);
    }
}

void rendermat(const material_s *mat)
{
    bool use_tex;
#if !defined(_GLSL)
    GLboolean color_enabled;
#endif

    if(!mat)
        return;

    use_tex = glIsEnabled(GL_TEXTURE_2D) && glIsTexture(mat->texture.tex_id);

    if(use_tex)
    {
#if defined(_GLSL)
        glActiveTexture(GL_TEXTURE0);
        GL_Uniform(GL_currprog, u_Texture2D, 1, 0);
#endif
        glBindTexture(GL_TEXTURE_2D, mat->texture.tex_id);
    }
    if(!mat->use_color)
    {
#if defined(_GLSL)
        GL_VertexAttribute(GL_currprog, a_Color, 4, sizeof(point_s), &(mat->points[0].color[0]));
        GL_Uniform(GL_currprog, u_Color, 1, White_Color);
#else
        glColorPointer(4, GL_FLOAT, sizeof(point_s), &(mat->points[0].color[0]));
#endif
    }
    else
    {
#if defined(_GLSL)
        GL_Uniform(GL_currprog, u_Color, 1, mat->color);
#else
        color_enabled = glIsEnabled(GL_COLOR_ARRAY);
        if(color_enabled)
            glDisableClientState(GL_COLOR_ARRAY);
        glColor4fv(mat->color);
#endif
    }

#if defined(_GLSL)
    GL_VertexAttribute(GL_currprog, a_Normal, 3, sizeof(point_s), &(mat->points[0].normal[0]));
    GL_VertexAttribute(GL_currprog, a_Texcoord, 2, sizeof(point_s), &(mat->points[0].texcoord[0]));
    GL_VertexAttribute(GL_currprog, a_Vertex, VERTEX_ELEMENT_COUNT, sizeof(point_s), &(mat->points[0].vertex[0]));
#else
    glNormalPointer(GL_FLOAT, sizeof(point_s), &(mat->points[0].normal[0]));
    glTexCoordPointer(2, GL_FLOAT, sizeof(point_s), &(mat->points[0].texcoord[0]));
    glVertexPointer(VERTEX_ELEMENT_COUNT, GL_FLOAT, sizeof(point_s), &(mat->points[0].vertex[0]));
#endif

    glDrawArrays(GL_TRIANGLES, 0, mat->count);

    if(use_tex)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

#if !defined(_GLSL)
    if(mat->use_color && color_enabled)
        glEnableClientState(GL_COLOR_ARRAY);
#endif
}

#define CPYELE(p) src->p = dst->p
void matcpy(material_s * RESTRICT src, const material_s * RESTRICT dst)
{
    if(!src || !dst || src == dst)
        return;

    newmat(src, dst->count);
    memcpy(src->points, dst->points, sizeof(point_s) * dst->count);

    memcpy(src->color, dst->color, sizeof(GLfloat) * 4);
    memcpy(src->position, dst->position, sizeof(GLfloat) * 3);
    memcpy(src->rotation, dst->rotation, sizeof(GLfloat) * 3);

    CPYELE(use_color);
    CPYELE(texture);
    CPYELE(face);
}

void meshcpy(mesh_s * RESTRICT src, const mesh_s * RESTRICT dst)
{
    int i;
    if(!src || !dst || src == dst)
        return;
    newmesh(src, dst->count);
    for(i = 0; i < dst->count; i++)
    {
        matcpy(src->materials + i, dst->materials + i);
    }
    memcpy(src->position, dst->position, sizeof(GLfloat) * 3);
    memcpy(src->rotation, dst->rotation, sizeof(GLfloat) * 3);
}
#undef CPYELE

int loadtex2d(texture2d_s *r, const void *data)
{
    GLuint tex;
    GLboolean enable_tex2d;

    if(!r || !data)
        return -1;

    enable_tex2d = glIsEnabled(GL_TEXTURE_2D);
    if(!enable_tex2d)
        glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
#ifdef _HARMATTAN
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, r->width, r->height, 0, r->format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    r->tex_id = tex;
    if(!enable_tex2d)
        glDisable(GL_TEXTURE_2D);
    return 0;
}
