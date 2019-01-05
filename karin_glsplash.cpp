#include "karin_glsplash.h"
#include "karin_std.h"

#include <GL/glu.h>
#include <GL/glext.h>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <math.h>

#ifdef GL_ARB_depth_clamp
#define SHADOW_VOLUME_FAR_W 0
#define SHADOW_VOLUME_LENGTH 10
#else
#define SHADOW_VOLUME_FAR_W 1
#define SHADOW_VOLUME_LENGTH 100
#endif

#define arrcpy3(n, o) {\
    n[0] = o[0]; \
    n[1] = o[1]; \
    n[2] = o[2]; \
}
#define M_CUBE_ROTATION_SENS 0.2

static unsigned render_type = 1; // for testing mask
static const int Mesh_Count = 3;

karin_GLSplash::karin_GLSplash(QWidget *parent) :
    QGLWidget(parent),
    m_cubeangle(Vector3D(0.0, 0.0, 0.0)),
    m_dirlighting(false),
    m_pressed(false),
    m_cubersens(M_CUBE_ROTATION_SENS),
    m_movesens(0.1),
    m_turnsens(1),
    m_timer(0)
{
    vector3_s viewpos = Vector3D(0.395481, 2.8357, 6.89706);
    vector3_s viewangle = Vector3D(30, 344, 0.0);

    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
    setFixedSize(karin_GLSplash::M_Width, karin_GLSplash::M_Width);

    m_count[0] = m_count[1] = m_count[2] = randr(40, 80);
    memset(m_cur, 0, sizeof(int) * 3);
    m_orientation[0] = m_orientation[1] = m_orientation[2] = randr(10, 18);
    initcam(&m_cam, &viewpos, &viewangle);
    m_lightpos << Vector3D(0.0, 2.0, 0.0)
        << Vector3D(-6.0, 6.0, -4.0)
        << Vector3D(-4.0, 4.0, 4.0)
        << Vector3D(2.0, 1.8, 0.5);

    if(!m_timer)
    {
        m_timer = new QTimer(this);
        m_timer->setInterval(karin_GLSplash::M_Timer_Interval);
        m_timer->setSingleShot(false);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(idle()));
    }

    m_timer->start();
}

karin_GLSplash::~karin_GLSplash()
{
    deinit();
}

void karin_GLSplash::init()
{
    GLfloat min[] = {-0.5, -0.5, -0.5};
    GLfloat max[] = {0.5, 0.5, 0.5};

    GLfloat min2[] = {-4.0, -2.0, -0.5};
    GLfloat max2[] = {-3.0, -1.0, 0.5};
    GLfloat min3[] = {-4.0, -2.0, 0.0};
    GLfloat max3[] = {-3.0, -1.0, 1.0};

    GLfloat minp[] = {-12.0, -2.0, 12.0};
    GLfloat maxp[] = {12.0, -2.0, -12.0};

    memset(&m_mesh, 0, sizeof(mesh_s));
    newmesh(&m_mesh, 4);

    karinPlane_MinMax(minp, maxp, m_mesh.materials);
    m_mesh.materials[0].color[0] = m_mesh.materials[0].color[1] = m_mesh.materials[0].color[2] = 0.1;
    m_mesh.materials[0].color[3] = 1.0;
    m_mesh.materials[0].use_color = true;

    karinCube_MinMax(min, max, m_mesh.materials + 1);
    m_mesh.materials[1].use_color = false;
    karinCube_MinMax(min2, max2, m_mesh.materials + 2);
    m_mesh.materials[2].use_color = false;
    karinCube_MinMax(min3, max3, m_mesh.materials + 3);
    m_mesh.materials[3].use_color = false;
    m_mesh.materials[3].rotation[1] = 59;
}

void karin_GLSplash::deinit()
{
    freemesh(&m_mesh);
}

void karin_GLSplash::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glPointSize(10);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    glStencilMask(~0);

    init();

    printfgl();
}

void karin_GLSplash::paintGL()
{
	static const QString Fmt("(%1, %2, %3) [%4, %5]");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();
    {
        glLoadIdentity();

        setcamera();

        // render scene
        rendershadowscene();


#if 0
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glDisable(GL_DEPTH_TEST);
        GLfloat vd[] = { 0, 0, 0,
                         VECTOR3_X(m_cam.direction) * 50, VECTOR3_Y(m_cam.direction) * 50, VECTOR3_Z(m_cam.direction) * 50 };
        glVertexPointer(3, GL_FLOAT, 0, vd);
        glDrawArrays(GL_LINES, 0, 2);
        glColor4f(0.0, 1.0, 0.0, 1.0);
        glDrawArrays(GL_POINTS, 0, 1);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glDrawArrays(GL_POINTS, 1, 2);
        glEnable(GL_DEPTH_TEST);
#endif

        // lighting source
        glColor4f(1.0, 1.0, 1.0, 1.0);
        GLfloat lp[] = { 0, 0, 0 };
        glVertexPointer(3, GL_FLOAT, 0, lp);
        for(int i = 0; i < m_lightpos.size(); i++)
        {
            glPushMatrix();
            {
                glTranslatef(VECTOR3_X(m_lightpos[i]), VECTOR3_Y(m_lightpos[i]), VECTOR3_Z(m_lightpos[i]));
                glDrawArrays(GL_POINTS, 0, 1);
            }
            glPopMatrix();
        }

    }
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);

    //printfcam(m_cam);

    glFlush();
}

void karin_GLSplash::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)h / (GLfloat)h, 0.01, 10000);
    glMatrixMode(GL_MODELVIEW);
}

void karin_GLSplash::setcamera() const
{
    camtrans_gl1(&m_cam);
}

vector3_s karin_GLSplash::lightingdir(const GLfloat v[3], const vector3_s *lightpos, bool dirlight) const
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

void karin_GLSplash::caletrans(material_s *r, const material_s *src, const GLmatrix *mat) const
{
    GLmatrix nor_mat;

    if(!r || !src || !mat)
        return;

    newmat(r, src->count);
    memcpy(r->points, src->points, sizeof(point_s) * src->count);
    r->use_color = src->use_color;
    r->texture = src->texture;
    memcpy(r->color, src->color, sizeof(GLfloat) * 4);
    for(int i = 0; i < src->count; i++)
    {
        Mesa_glTransform4(r->points[i].vertex, src->points[i].vertex, mat);
    }

    Mesa_AllocGLMatrix(&nor_mat);
    Mesa_NormalMatrix(&nor_mat, mat->m);

    for(int i = 0; i < src->count; i++)
    {
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
void karin_GLSplash::shadowvol(mesh_s *r, const vector3_s *lightpos, const material_s *mat) const
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
        vector3_s p2l = lightingdir(v, lightpos, m_dirlighting);
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
        vector3_s dir_a = lightingdir(l.a.v, lightpos, m_dirlighting);
        vector3_scale_self(&dir_a, SHADOW_VOLUME_LENGTH);
        vector3_s dir_b = lightingdir(l.b.v, lightpos, m_dirlighting);
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
        vector3_s dir_a = lightingdir(first.v, lightpos, m_dirlighting);
        vector3_scale_self(&dir_a, SHADOW_VOLUME_LENGTH);
        vector3_s dir_b = lightingdir(sec.v, lightpos, m_dirlighting);
        vector3_scale_self(&dir_b, SHADOW_VOLUME_LENGTH);
        vector3_s dir_c = lightingdir(third.v, lightpos, m_dirlighting);
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

void karin_GLSplash::rendershadow(const mesh_s *cube, const vector3_s *lpos, bool firstrender) const
{
    mesh_s *vol;

    if(!cube || !lpos)
        return;

    vol = (mesh_s *)calloc(Mesh_Count, sizeof(mesh_s));

    for(int i = 0; i < Mesh_Count; i++)
    {
        shadowvol(vol + i, lpos, cube->materials + i);
    }

    // render
    // 1: get depth buffer of scene
    glClear(GL_STENCIL_BUFFER_BIT);
    if(firstrender)
    {
        glDisable(GL_BLEND);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        drawscene(cube, true);
    }
    //goto __Exit;

    // 2: Z-Fail
#ifdef GL_ARB_depth_clamp
    glEnable(GL_DEPTH_CLAMP);
#endif
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0);

    // 2-1: cale front faces of shadow volume
    glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
    glCullFace(GL_FRONT);
    for(int i = 0; i < Mesh_Count; i++)
    {
        rendermesh(vol + i);
    }

    // 2-1: cale back faces of shadow volume
    glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
    glCullFace(GL_BACK);
    for(int i = 0; i < Mesh_Count; i++)
    {
        rendermesh(vol + i);
    }

#ifdef GL_ARB_depth_clamp
    glDisable(GL_DEPTH_CLAMP);
#endif

    // 3: final render scene again
    glCullFace(GL_BACK);
    glDepthFunc(GL_EQUAL); // GL_LESS will not pass depth-testing
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 0, ~0);
    glBlendFunc(GL_ONE, GL_ONE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_BLEND);

    drawscene(cube, true);

    // 4: reset OpenGL state
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);

__Exit:
    for(int i = 0; i < Mesh_Count; i++)
    {
        freemesh(vol + i);
    }
    free(vol);
}

void karin_GLSplash::rendershadowscene() const
{
    mesh_s cube;
    GLmatrix *mat;

    memset(&cube, 0, sizeof(mesh_s));
    newmesh(&cube,Mesh_Count);

    mat = (GLmatrix *)calloc(Mesh_Count, sizeof(GLmatrix));


    for(int i = 0; i < Mesh_Count; i++)
    {
        Mesa_AllocGLMatrix(mat + i);
        Mesa_glLoadIdentity(mat + i);
        Mesa_glRotate(mat + i, m_mesh.materials[i + 1].rotation[0], 1, 0, 0);
        Mesa_glRotate(mat + i, m_mesh.materials[i + 1].rotation[1], 0, 1, 0);
        Mesa_glRotate(mat + i, m_mesh.materials[i + 1].rotation[2], 0, 0, 1); // ??
    }

    for(int i = 0; i < Mesh_Count; i++)
    {
        caletrans(cube.materials + i, m_mesh.materials + i + 1, mat + i);
    }

    for(int i = 0; i < m_lightpos.size(); i++)
    {
        rendershadow(&cube, &m_lightpos[i], i == 0);
    }

__Exit:
    for(int i = 0; i < Mesh_Count; i++)
    {
        Mesa_FreeGLMatrix(mat + i);
    }
    free(mat);
    freemesh(&cube);
}

void karin_GLSplash::drawscene(const mesh_s *cube, bool plane) const
{
    if(plane)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        rendermat(m_mesh.materials);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if(cube)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        rendermesh(cube);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void karin_GLSplash::mousePressEvent(QMouseEvent* event)
{
    m_lastpos = event->pos();
    m_pressed = true;
    if(event->modifiers() & Qt::ControlModifier)
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    event->accept();

    QGLWidget::mousePressEvent(event);
}

void karin_GLSplash::mouseMoveEvent(QMouseEvent* event)
{
	vector3_s v;
	
    if(m_pressed)
    {
        if(event->modifiers() & Qt::ControlModifier)
        {
            QPoint delta = (event->pos() - m_lastpos) * m_cubersens;
            m_lastpos = event->pos();
            if(!delta.isNull())
            {
                v = Vector3D(delta.y(), delta.x(), 0);
                m_cubeangle = vector3_add(&m_cubeangle, &v);
                VECTOR3_X(m_cubeangle) = clampangle(VECTOR3_X(m_cubeangle));
                VECTOR3_Y(m_cubeangle) = clampangle(VECTOR3_Y(m_cubeangle));
                updateGL();
            }
        }
        else
        {
            QPoint center = mapToGlobal(QPoint(width() / 2, height() / 2));
            QPoint delta = (QCursor::pos() - center) * m_turnsens;
            if(!delta.isNull())
            {
                v = Vector3D(delta.y(), delta.x(), 0);
                camrot(&m_cam, &v);
                updateGL();
            }
            QCursor::setPos(center);
        }
    }
    QGLWidget::mouseMoveEvent(event);
}

void karin_GLSplash::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressed = false;
    m_lastpos.setX(0);
    m_lastpos.setY(0);
    QGLWidget::mouseReleaseEvent(event);
}

void karin_GLSplash::keyPressEvent(QKeyEvent *event)
{
    if(!keyev(event, true))
        QGLWidget::keyPressEvent(event);
}

void karin_GLSplash::keyReleaseEvent(QKeyEvent *event)
{
    if(!keyev(event, false))
        QGLWidget::keyReleaseEvent(event);
}

void karin_GLSplash::transform()
{
    vector3_s unit;

    VECTOR3_X(unit) = VECTOR3_Y(unit) = VECTOR3_Z(unit) = 0;

    if(m_direction[Direction_Forward])
        VECTOR3_Z(unit) = m_movesens;
    else if(m_direction[Direction_Backward])
        VECTOR3_Z(unit) = -m_movesens;

    if(m_direction[Direction_Left])
        VECTOR3_X(unit) = -m_movesens;
    else if(m_direction[Direction_Right])
        VECTOR3_X(unit) = m_movesens;

    if(m_direction[Direction_Down])
        VECTOR3_Y(unit) = -m_movesens;
    else if(m_direction[Direction_Up])
        VECTOR3_Y(unit) = m_movesens;

    cammove(&m_cam, &unit);
    //m_lightpos[0] += unit;
    //qDebug()<<m_lightpos[0];
    updateGL();
}

bool karin_GLSplash::keyev(QKeyEvent *event, bool pressed)
{
    int i;
    bool r;

    r = false;

    if(event->modifiers() == Qt::NoModifier)
    {
        i = -1;
        switch(event->key())
        {
        case Qt::Key_W:
            i = Direction_Forward;
            break;
        case Qt::Key_S:
            i = Direction_Backward;
            break;
        case Qt::Key_A:
            i = Direction_Left;
            break;
        case Qt::Key_D:
            i = Direction_Right;
            break;
        case Qt::Key_Q:
            i = Direction_Down;
            break;
        case Qt::Key_E:
            i = Direction_Up;
            break;

        case Qt::Key_M:
            if(pressed)
            {
                render_type ^= 1;
                updateGL();
            }
            break;
        case Qt::Key_L:
            if(pressed)
            {
                m_dirlighting ^= 1;
                updateGL();
            }
            break;
        case Qt::Key_C:
            if(pressed)
            {
                if(glIsEnabled(GL_CULL_FACE))
                    glDisable(GL_CULL_FACE);
                else
                    glEnable(GL_CULL_FACE);
                updateGL();
            }
            break;
        case Qt::Key_P:
            if(pressed)
            {
                if(m_timer->isActive())
                    m_timer->stop();
                else
                    m_timer->start();
                updateGL();
            }
            break;
        default:
            break;
        }
        if(i != -1)
        {
            m_direction[i] = pressed;
            r = true;
            transform();
        }
        goto __Exit;
    }
    else if(event->modifiers() & Qt::ControlModifier)
    {
        goto __Exit;
    }
    else
        goto __Exit;

__Exit:
    return r;
}

void karin_GLSplash::idle()
{
    for(int i = 0; i < 3; i++)
    {
        if(m_cur[i] > m_count[i])
        {
            m_count[i] = randr(40, 80);
            m_orientation[i] = randr(10, 18);
            m_cur[i] = 0;
        }
        else
        {
            m_cur[i]++;
        }
    }

    VECTOR3_X(m_cubeangle) = clampangle(VECTOR3_X(m_cubeangle) + m_orientation[0] * m_cubersens);
    VECTOR3_Y(m_cubeangle) = clampangle(VECTOR3_Y(m_cubeangle) + m_orientation[1] * m_cubersens);
    VECTOR3_Z(m_cubeangle) = clampangle(VECTOR3_Z(m_cubeangle) + m_orientation[2] * m_cubersens);


    m_mesh.materials[1].rotation[0] = VECTOR3_X(m_cubeangle);
    m_mesh.materials[1].rotation[1] = VECTOR3_Y(m_cubeangle);
    m_mesh.materials[1].rotation[2] = VECTOR3_Z(m_cubeangle);

    updateGL();
}
