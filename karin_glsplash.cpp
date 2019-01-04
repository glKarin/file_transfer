#include "karin_glsplash.h"
#include "karin_std.h"

#include <GL/glu.h>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <math.h>

#define arrcpy3(n, o) {\
    n[0] = o[0]; \
    n[1] = o[1]; \
    n[2] = o[2]; \
}
#define M_CUBE_ROTATION_SENS 0.2

static unsigned render_type = 1;

karin_GLSplash::karin_GLSplash(QWidget *parent) :
    QGLWidget(parent),
    m_viewpos(QVector3D(7.45058e-08, 2.8, 6.4)),
    m_scale(QVector3D(1.0, 1.0, 1.0)),
    m_viewangle(QVector3D(30, 343, 0.0)),
    m_cubeangle(QVector3D(0.0, 0.0, 0.0)),
    m_lightpos(QVector3D(2.0, 1.8, 0.5)),
   // m_lightpos(QVector3D(-2.9, 1.8, 0.1)),
    m_pressed(false),
    m_cubersens(M_CUBE_ROTATION_SENS),
    m_movesens(0.1),
    m_turnsens(1),
    m_timer(0)
{
    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
    setFixedSize(karin_GLSplash::M_Width, karin_GLSplash::M_Width);

    m_count[0] = m_count[1] = m_count[2] = randr(40, 80);
    memset(m_cur, 0, sizeof(int) * 3);
    m_orientation[0] = m_orientation[1] = m_orientation[2] = randr(10, 18);

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

    GLfloat minp[] = {-8.0, -2.0, 8.0};
    GLfloat maxp[] = {8.0, -2.0, -8.0};

    memset(&m_mesh, 0, sizeof(mesh_s));
    newmesh(&m_mesh, 3);
    karinCube_MinMax(min, max, m_mesh.materials);
    m_mesh.materials[0].use_color = false;

    karinPlane_MinMax(minp, maxp, m_mesh.materials + 1);
    m_mesh.materials[1].color[0] = m_mesh.materials[1].color[1] = m_mesh.materials[1].color[2] = 0.5;
    m_mesh.materials[1].color[3] = 1.0;
    m_mesh.materials[1].use_color = true;

    karinCube_MinMax(min2, max2, m_mesh.materials + 2);
    m_mesh.materials[2].use_color = false;
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
}

void karin_GLSplash::paintGL()
{
	static const QString Fmt("(%1, %2, %3) [%4, %5]");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glLoadIdentity();
    glPushMatrix();
    {
        glRotatef(m_viewangle.x(), 1, 0, 0);
        glRotatef(m_viewangle.y(), 0, 1, 0);
        glTranslatef(-m_viewpos.x(), -m_viewpos.y(), -m_viewpos.z());

        glEnableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
        {
            glTranslatef(m_lightpos.x(), m_lightpos.y(), m_lightpos.z());
            glColor4f(1.0, 1.0, 1.0, 1.0);
            GLfloat lp[] = {0, 0, 0};
            glVertexPointer(3, GL_FLOAT, 0, lp);
            glDrawArrays(GL_POINTS, 0, 1);
        }
        glPopMatrix();

        calevol();

        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glPopMatrix();
	//qDebug()<< (Fmt.arg(m_viewpos.x()).arg(m_viewpos.y()).arg(m_viewpos.z()).arg(m_viewangle.x()).arg(m_viewangle.y()));
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

typedef struct _line_s
{
    QVector3D a;
    QVector3D b;
    bool operator==(const _line_s &o)
    {
        return((a == o.a && b == o.b)
               || (a == o.b && b == o.a));
    }
    template <int I> void set(const GLfloat v[3])
    {
        if(!v)
            return;
        QVector3D &p = I == 0 ? a : b;
        p.setX(v[0]);
        p.setY(v[1]);
        p.setZ(v[2]);
    }
    template <int I> void get(GLfloat v[3]) const
    {
        if(!v)
            return;
        const QVector3D &p = I == 0 ? a : b;
        v[0] = (p.x());
        v[1] = (p.y());
        v[2] = (p.z());
    }
} line_s;

void karin_GLSplash::caletrans(material_s *r, const material_s *src, const GLmatrix *mat)
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
        QVector3D n(r->points[i].normal[0], r->points[i].normal[1], r->points[i].normal[2]);
        n.normalize();
        r->points[i].normal[0] = n.x();
        r->points[i].normal[1] = n.y();
        r->points[i].normal[2] = n.z();
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

void karin_GLSplash::shadowvol(mesh_s *r, const QVector3D &lightpos, const material_s *mat)
{
    bool has;
    line_s lp;
    int k;
    point_s *pa;
    material_s *m;
    QList<line_s> lines;
    const GLfloat vol_len = 10;
    QList<QVector3D> tops, bottoms;
    GLfloat *v;

    if(!r || !mat || !mat->count)
        return;

    newmesh(r, 3); // top bottom caps, and vol side
    for(int i = 0; i < mat->count / 3; i++) // cale triangle
    {
        pa = mat->points + (i * 3);
        v = pa->vertex;
        QVector3D nor(pa->normal[0], pa->normal[1], pa->normal[2]);
        QVector3D p(v[0], v[1], v[2]);
        QVector3D p2l = QVector3D(lightpos - p).normalized();
        float dot_p = QVector3D::dotProduct(p2l, nor);
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
                    if(lp == lines.at(m)) // if exists, continue
                    {
                        //qDebug()<<"exist";
                        lines.removeAt(m);
                        has = true;
                        break;
                    }
                }
                if(!has) // if not exists, add line to list
                    lines.push_back(lp);
            }
            tops << fv4toqvec3(pa[0].vertex)
             << fv4toqvec3(pa[1].vertex)
              << fv4toqvec3(pa[2].vertex);
        }
        else
        {
            bottoms << fv4toqvec3(pa[0].vertex)
             << fv4toqvec3(pa[1].vertex)
              << fv4toqvec3(pa[2].vertex);
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

    m = r->materials;
    newmat(m, lines.size() * 6);
    k = 0;
    Q_FOREACH(const line_s &l, lines)
    {
        QVector3D dir_a = QVector3D(l.a) - m_lightpos;
        dir_a.normalize();
        dir_a *= vol_len;
        QVector3D dir_b = QVector3D(l.b) - m_lightpos;
        dir_b.normalize();
        dir_b *= vol_len;

        pa = m->points + k * 6;
        pa->vertex[0] = l.a.x();
        pa->vertex[1] = l.a.y();
        pa->vertex[2] = l.a.z();
        pa->vertex[3] = 1;
        pa = m->points + k * 6 + 1;
        pa->vertex[0] = dir_a.x();
        pa->vertex[1] = dir_a.y();
        pa->vertex[2] = dir_a.z();
        pa->vertex[3] = 1;
        pa = m->points + k * 6 + 2;
        pa->vertex[0] = l.b.x();
        pa->vertex[1] = l.b.y();
        pa->vertex[2] = l.b.z();
        pa->vertex[3] = 1;

        pa = m->points + k * 6 + 3;
        pa->vertex[0] = dir_a.x();
        pa->vertex[1] = dir_a.y();
        pa->vertex[2] = dir_a.z();
        pa->vertex[3] = 1;
        pa = m->points + k * 6 + 4;
        pa->vertex[0] = dir_b.x();
        pa->vertex[1] = dir_b.y();
        pa->vertex[2] = dir_b.z();
        pa->vertex[3] = 1;
        pa = m->points + k * 6 + 5;
        pa->vertex[0] = l.b.x();
        pa->vertex[1] = l.b.y();
        pa->vertex[2] = l.b.z();
        pa->vertex[3] = 1;

        k++;
    }

    m = r->materials + 1;
    newmat(m, tops.size());
    for(k = 0; k < tops.size(); k += 3)
    {
        const QVector3D &first = tops[k];
        const QVector3D &sec = tops[k + 1];
        const QVector3D &third = tops[k + 2];
        pa = m->points + k;
        pa->vertex[0] = first.x();
        pa->vertex[1] = first.y();
        pa->vertex[2] = first.z();
        pa->vertex[3] = 1;
        pa = m->points + k + 1;
        pa->vertex[0] = sec.x();
        pa->vertex[1] = sec.y();
        pa->vertex[2] = sec.z();
        pa->vertex[3] = 1;
        pa = m->points + k + 2;
        pa->vertex[0] = third.x();
        pa->vertex[1] = third.y();
        pa->vertex[2] = third.z();
        pa->vertex[3] = 1;
    }

    m = r->materials + 2;
    newmat(m, bottoms.size());
    for(k = 0; k < bottoms.size(); k += 3)
    {
        const QVector3D &first = bottoms[k];
        const QVector3D &sec = bottoms[k + 1];
        const QVector3D &third = bottoms[k + 2];
        QVector3D dir_a = first - m_lightpos;
        dir_a.normalize();
        dir_a *= vol_len;
        QVector3D dir_b = sec - m_lightpos;
        dir_b.normalize();
        dir_b *= vol_len;
        QVector3D dir_c = third - m_lightpos;
        dir_c.normalize();
        dir_c *= vol_len;

        pa = m->points + k;
        pa->vertex[0] = dir_a.x();
        pa->vertex[1] = dir_a.y();
        pa->vertex[2] = dir_a.z();
        pa->vertex[3] = 1;
        pa = m->points + k + 1;
        pa->vertex[0] = dir_b.x();
        pa->vertex[1] = dir_b.y();
        pa->vertex[2] = dir_b.z();
        pa->vertex[3] = 1;
        pa = m->points + k + 2;
        pa->vertex[0] = dir_c.x();
        pa->vertex[1] = dir_c.y();
        pa->vertex[2] = dir_c.z();
        pa->vertex[3] = 1;
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

void karin_GLSplash::calevol()
{
    GLmatrix mat;
    QVector3D light_dir(0, 1, 0);
    mesh_s vol, vol2;
    material_s cube, cube2;

    memset(&vol, 0, sizeof(mesh_s));
    memset(&cube, 0, sizeof(material_s));
    memset(&vol2, 0, sizeof(mesh_s));
    memset(&cube2, 0, sizeof(material_s));

    Mesa_AllocGLMatrix(&mat);
    Mesa_glLoadIdentity(&mat);
    Mesa_glRotate(&mat, m_cubeangle.x(), 1, 0, 0);
    Mesa_glRotate(&mat, m_cubeangle.y(), 0, 1, 0);
    Mesa_glRotate(&mat, m_cubeangle.z(), 0, 0, 1); // ??

    caletrans(&cube, m_mesh.materials, &mat);
    shadowvol(&vol, m_lightpos, &cube);

    Mesa_glLoadIdentity(&mat);
    //Mesa_glRotate(&mat, 45, 0, 1, 0);

    caletrans(&cube2, m_mesh.materials + 2, &mat);
    shadowvol(&vol2, m_lightpos, &cube2);

    glDisable(GL_BLEND);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnableClientState(GL_NORMAL_ARRAY);
    drawscene(false, true);
    glEnableClientState(GL_COLOR_ARRAY);
    rendermat(&cube);
    rendermat(&cube2);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    //goto __Exit;

    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0);

    glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
    glCullFace(GL_FRONT);
    rendermesh(&vol);
    rendermesh(&vol2);

    glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
    glCullFace(GL_BACK);
    rendermesh(&vol);
    rendermesh(&vol2);

    glCullFace(GL_BACK);
    glDepthFunc(GL_EQUAL);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 0, ~0);
    glBlendFunc(GL_ONE, GL_ONE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_BLEND);

    glEnableClientState(GL_NORMAL_ARRAY);
    drawscene(false, true);
    glEnableClientState(GL_COLOR_ARRAY);
    rendermat(&cube);
    rendermat(&cube2);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);

__Exit:
    freemat(&cube);
    freemesh(&vol);
    freemat(&cube2);
    freemesh(&vol2);
    Mesa_FreeGLMatrix(&mat);
}

void karin_GLSplash::drawscene(bool cube, bool plane)
{
    // cube
    if(cube)
    {
        rendermat(m_mesh.materials);
    }
	
    // plane
    if(plane)
    {
        rendermat(m_mesh.materials + 1);
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
    if(m_pressed)
    {
        if(event->modifiers() & Qt::ControlModifier)
        {
            QPoint center = mapToGlobal(QPoint(width() / 2, height() / 2));
            QPoint delta = (QCursor::pos() - center) * m_turnsens;
            if(!delta.isNull())
            {
                m_viewangle += QVector3D(delta.y(), delta.x(), 0);
                m_viewangle.setX(clampangle(m_viewangle.x()));
                m_viewangle.setY(clampangle(m_viewangle.y()));
                updateGL();
            }
            QCursor::setPos(center);
        }
        else
        {
            QPoint delta = (event->pos() - m_lastpos) * m_cubersens;
            m_lastpos = event->pos();
            if(!delta.isNull())
            {
                m_cubeangle += QVector3D(delta.y(), delta.x(), 0);
                m_cubeangle.setX(clampangle(m_cubeangle.x()));
                m_cubeangle.setY(clampangle(m_cubeangle.y()));
                updateGL();
            }
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
    QVector3D unit;

    if(m_direction[Direction_Forward])
        unit.setZ(-m_movesens);
    else if(m_direction[Direction_Backward])
        unit.setZ(m_movesens);

    if(m_direction[Direction_Left])
        unit.setX(-m_movesens);
    else if(m_direction[Direction_Right])
        unit.setX(m_movesens);

    if(m_direction[Direction_Down])
        unit.setY(-m_movesens);
    else if(m_direction[Direction_Up])
        unit.setY(m_movesens);

    m_viewpos += unit;
    //m_lightpos += unit;
    //qDebug()<<m_lightpos;
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

        case Qt::Key_P:
            if(pressed)
            {
                render_type ^= 1;
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

    m_cubeangle.setX(clampangle(m_cubeangle.x() + m_orientation[0] * m_cubersens));
    m_cubeangle.setY(clampangle(m_cubeangle.y() + m_orientation[1] * m_cubersens));
    m_cubeangle.setZ(clampangle(m_cubeangle.z() + m_orientation[2] * m_cubersens));

    updateGL();
}
