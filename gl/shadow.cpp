#include "shadow.h"
#include "qmath.h"

#include <QList>

#ifdef GL_ARB_depth_clamp
#define SHADOW_VOLUME_FAR_W 0
#define SHADOW_VOLUME_LENGTH 10
#else
#define SHADOW_VOLUME_FAR_W 1
#define SHADOW_VOLUME_LENGTH 100
#endif

#define SHADOW_MASK_W 5000
#define SHADOW_MASK_Z -1

vector3_s shadowlightingdir(const GLfloat v[3], const vector3_s *lightpos, bool dirlight)
{
    bool d;
    vector3_s r = Vector3D(0, 0, 0);

    if(!lightpos)
        return r;

    d = !v ? true : dirlight;

    if(d)
    {
        r = vector3_normalize(lightpos);
        vector3_invert_self(&r);
    }
    else
    {
        r = Vector3D(v);
        r = vector3_direction(lightpos, &r);
    }

    return r;
}

void shadowcaletrans(material_s *r, const material_s *src, const GLmatrix *mat)
{
    GLmatrix nor_mat;

    if(!r || !src || !mat)
        return;

    newmat(r, src->count);
    memcpy(r->points, src->points, sizeof(point_s) * src->count);
    r->use_color = src->use_color;
    r->texture = src->texture;
    memcpy(r->color, src->color, sizeof(GLfloat) * 4);

    Mesa_AllocGLMatrix(&nor_mat);
    Mesa_NormalMatrix(&nor_mat, mat->m);

    for(int i = 0; i < src->count; i++)
    {
        Mesa_glTransform4(r->points[i].vertex, src->points[i].vertex, mat);

        // normal
        Mesa_glTransform(r->points[i].normal, src->points[i].normal, &nor_mat);
        vector3_s n = Vector3D(r->points[i].normal);
        vector3_normalize_self(&n);
        vector3_s_to_GLfloatv(r->points[i].normal, n);
    }

#if 0
    glDisable(GL_DEPTH_TEST);
    glColor4f(0.0, 0.0, 1.0, 1.0);
    for(int i = 0; i < 36; i++)
    {
        GLfloat nv[6];
        nv[0] = nv[1] = nv[2] = 0;
        nv[3] = r->points[i].normal[0];
        nv[4] = r->points[i].normal[1];
        nv[5] = r->points[i].normal[2];
        glVertexPointer(3, GL_FLOAT, 0, nv);
        glDrawArrays(GL_LINES, 0, 2);
    }
    glEnable(GL_DEPTH_TEST);
#endif
    Mesa_FreeGLMatrix(&nor_mat);
}

// cale shadow volume
void shadowcalevol(mesh_s *r, const vector3_s *lightpos, const material_s *mat, bool dirlighting)
{
    bool has;
    line_s lp;
    int k;
    point_s *pa;
    material_s *m;
    QList<line_s> lines;
    QList<vector3_s> tops, bottoms;
    GLfloat *v;

    if(!r || !lightpos || !mat || !mat->count)
        return;

    newmesh(r, 3); // top[1] bottom[2] caps, and side[0]
    for(int i = 0; i < mat->count / 3; i++) // cale triangle
    {
        pa = mat->points + (i * 3);
        v = pa->vertex;
        vector3_s nor = Vector3D(pa->normal);
        //vector3_s p = Vector3D(v);
        vector3_s p2l = shadowlightingdir(v, lightpos, dirlighting);
        vector3_invert_self(&p2l);
        float dot_p = vector3_dot(&p2l, &nor);
        //qDebug()<<i<<nor<<p<<dot_p;
        if(dot_p > 0.0) // face to light source
        {
            for(int n = 0; n < 3; n++) // 3 points of triangle
            {
                lp.set<0>(pa[n].vertex);
                lp.set<1>(pa[(n + 1) % 3].vertex);
                has = false;
                for(int m = 0; m < lines.size(); m++) // find in lines list
                {
                    if(lp == lines.at(m)) // if exists, remove this line, then continue
                    {
                        //qDebug()<<"exist";
                        lines.removeAt(m);
                        has = true;
                        break;
                    }
                }
                if(!has) // if not exists, add new line to list
                    lines.push_back(lp);
            }
            // top cap triangle
            tops << Vector4D(pa[0].vertex)
                 << Vector4D(pa[1].vertex)
                 << Vector4D(pa[2].vertex);
        }
        else
        {
            // bottom cap triangle
            bottoms << Vector4D(pa[0].vertex)
                    << Vector4D(pa[1].vertex)
                    << Vector4D(pa[2].vertex);
        }
    }
#if 0
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    {
        glColor4f(1.0, 1.0, 1.0, 1.0);
        GLfloat vs[6];
        Q_FOREACH(const line_s &l, lines)
        {
            glLineWidth(4.0);
            qDebug()<<l.a<<l.b;
            l.get<0>(vs);
            l.get<1>(vs + 3);
            glVertexPointer(3, GL_FLOAT, 0, vs);
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
#endif

    // cale sides of shadow volume
    m = r->materials;
    newmat(m, lines.size() * 6); // 2 triangles(6 points) every a line
    k = 0;
    // TODO: cale clock wise, now the lighting source must be above all cubes
    Q_FOREACH(const line_s &l, lines)
    {
        // point lighting
        vector3_s dir_a = shadowlightingdir(l.a.v, lightpos, dirlighting);
        vector3_scale_self(&dir_a, SHADOW_VOLUME_LENGTH);
        vector3_s dir_b = shadowlightingdir(l.b.v, lightpos, dirlighting);
        vector3_scale_self(&dir_b, SHADOW_VOLUME_LENGTH);

        // triangle 1
        pa = m->points + k * 6;
        vector3_s_to_GLfloatv(pa->vertex, l.a);
        pa = m->points + k * 6 + 1;
        vector3_s_to_GLfloatv(pa->vertex, dir_a);
        pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = m->points + k * 6 + 2;
        vector3_s_to_GLfloatv(pa->vertex, l.b);

        // triangle 2
        pa = m->points + k * 6 + 3;
        vector3_s_to_GLfloatv(pa->vertex, dir_a);
        pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = m->points + k * 6 + 4;
        vector3_s_to_GLfloatv(pa->vertex, dir_b);
        pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = m->points + k * 6 + 5;
        vector3_s_to_GLfloatv(pa->vertex, l.b);

        k++;
    }

    // cale top cap of shadow volume
    // using triangles of the mesh faces to lighting source
    m = r->materials + 1;
    newmat(m, tops.size());
    for(k = 0; k < tops.size(); k += 3)
    {
        const vector3_s &first = tops[k];
        const vector3_s &sec = tops[k + 1];
        const vector3_s &third = tops[k + 2];
        pa = m->points + k;
        vector3_s_to_GLfloatv(pa->vertex, first);
        pa = m->points + k + 1;
        vector3_s_to_GLfloatv(pa->vertex, sec);
        pa = m->points + k + 2;
        vector3_s_to_GLfloatv(pa->vertex, third);
    }

    // cale bottom cap of shadow volume
    // using triangles of the mesh not faces to lighting source
    m = r->materials + 2;
    newmat(m, bottoms.size());
    for(k = 0; k < bottoms.size(); k += 3)
    {
        const vector3_s &first = bottoms[k];
        const vector3_s &sec = bottoms[k + 1];
        const vector3_s &third = bottoms[k + 2];

        // point lighting
        vector3_s dir_a = shadowlightingdir(first.v, lightpos, dirlighting);
        vector3_scale_self(&dir_a, SHADOW_VOLUME_LENGTH);
        vector3_s dir_b = shadowlightingdir(sec.v, lightpos, dirlighting);
        vector3_scale_self(&dir_b, SHADOW_VOLUME_LENGTH);
        vector3_s dir_c = shadowlightingdir(third.v, lightpos, dirlighting);
        vector3_scale_self(&dir_c, SHADOW_VOLUME_LENGTH);

        pa = m->points + k;
        vector3_s_to_GLfloatv(pa->vertex, dir_a);
        pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = m->points + k + 1;
        vector3_s_to_GLfloatv(pa->vertex, dir_b);
        pa->vertex[3] = SHADOW_VOLUME_FAR_W;
        pa = m->points + k + 2;
        vector3_s_to_GLfloatv(pa->vertex, dir_c);
        pa->vertex[3] = SHADOW_VOLUME_FAR_W;
    }

#if 0
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDisable(GL_DEPTH_TEST);

    glCullFace(GL_FRONT);
    glLineWidth(4.0);

    glDisable(GL_BLEND);
    glColor4f(1,0,0,1);
    //rendermesh(r);
    rendermat(r->materials);
    glColor4f(0,1,0,1);
    rendermat(r->materials + 1);
    glColor4f(0,0,1,1);
    rendermat(r->materials + 2);
    glColor4f(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

void shadowrvol(const mesh_s *cube, const vector3_s *lpos, bool dirlighting)
{
    mesh_s *vol;

    if(!cube || !lpos)
        return;

    vol = (mesh_s *)calloc(cube->count, sizeof(mesh_s));

    for(int i = 0; i < cube->count; i++)
    {
        shadowcalevol(vol + i, lpos, cube->materials + i, dirlighting);
    }

    // render
    // 1: get depth buffer of scene

    //goto __Exit;

    // 2: Z-Fail
#ifdef GL_ARB_depth_clamp
    glEnable(GL_DEPTH_CLAMP);
#endif
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0U);

    // 2-1: cale front faces of shadow volume
#if !defined(_GLSL)
    glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
    glCullFace(GL_FRONT);
    for(int i = 0; i < cube->count; i++)
    {
        rendermesh(vol + i);
    }

    // 2-1: cale back faces of shadow volume
    glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
    glCullFace(GL_BACK);
    for(int i = 0; i < cube->count; i++)
    {
        rendermesh(vol + i);
    }
#else
    glDisable(GL_CULL_FACE);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    for(int i = 0; i < cube->count; i++)
    {
        rendermesh(vol + i);
    }
    glEnable(GL_CULL_FACE);
#endif

#ifdef GL_ARB_depth_clamp
    glDisable(GL_DEPTH_CLAMP);
#endif

    // 3: final render scene or mask again

    // 4: reset OpenGL state
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDisable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0U);

__Exit:
    for(int i = 0; i < cube->count; i++)
    {
        freemesh(vol + i);
    }
    free(vol);
}

void shadowrmesh(const mesh_s *mesh, const vector3_s *lightpos, bool dirlighting, int start, int count)
{
    mesh_s cube;
    GLmatrix *mat;
    int mesh_count;
    int index;

    if(!mesh || !lightpos)
        return;

    mesh_count = count;
    if(count < 0)
        mesh_count = mesh->count;

    if(mesh_count == 0)
        return;

    index = 0;
    memset(&cube, 0, sizeof(mesh_s));
    newmesh(&cube, mesh_count);

    // make transform matrix
    mat = (GLmatrix *)calloc(mesh_count, sizeof(GLmatrix));

    for(int i = start; i < start + mesh_count; i++)
    {
        Mesa_AllocGLMatrix(mat + index);
        Mesa_glLoadIdentity(mat + index);
        Mesa_glRotate(mat + index, mesh->materials[i].rotation[0], 1, 0, 0);
        Mesa_glRotate(mat + index, mesh->materials[i].rotation[1], 0, 1, 0);
        Mesa_glRotate(mat + index, mesh->materials[i].rotation[2], 0, 0, 1); // ??
        index++;
    }

    // cale mesh new vertex position and normal
    for(int i = 0; i < mesh_count; i++)
    {
        shadowcaletrans(cube.materials + i, mesh->materials + i + 1, mat + i);
    }

    // render shadow volume
    shadowrvol(&cube, lightpos, dirlighting);

__Exit:
    for(int i = 0; i < mesh_count; i++)
    {
        Mesa_FreeGLMatrix(mat + i);
    }
    free(mat);
    freemesh(&cube);
}

void shadowrmask(const GLfloat color[4])
{
    if(!color)
        return;

    GLfloat mask[] = {
        -SHADOW_MASK_W, -SHADOW_MASK_W, SHADOW_MASK_Z,
        SHADOW_MASK_W, -SHADOW_MASK_W, SHADOW_MASK_Z,
        -SHADOW_MASK_W, SHADOW_MASK_W, SHADOW_MASK_Z,
        SHADOW_MASK_W, SHADOW_MASK_W, SHADOW_MASK_Z
    };

    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NOTEQUAL, 0, ~0U);
    glEnable(GL_STENCIL_TEST);
    glColor4fv(color);

    glVertexPointer(3, GL_FLOAT, 0, mask);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glColor4f(1, 1, 1, 1);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0U);
}
