#include "karin_glwidget.h"
#include "gl/gl_std.h"
#include "gl/qmath.h"

#include <QTimer>
#include <QDebug>
#include <QMouseEvent>

karin_GLWidget::karin_GLWidget(QWidget *parent) :
    QGLWidget(parent),
    m_timer(0),
    m_movesens(0.1),
    m_turnsens(1),
    m_pressed(false)
{
#if defined(_GLSL) && defined(_OS_WIN32)
    QGLFormat format;
    format.setVersion(3, 3);
    format.setProfile(QGLFormat::CoreProfile);
    setFormat(format);
#endif

    newcam(&m_cam);

    for(int i = 0; i < TotalMatrix; i++)
    {
        Mesa_AllocGLMatrix(m_matrix + i);
        Mesa_glLoadIdentity(m_matrix + i);
    }

    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
}

karin_GLWidget::~karin_GLWidget()
{
    deinit();
}

void karin_GLWidget::deinit()
{
#if defined(_GLSL)
    GL_DeleteProgram(&m_defaultprog);
#endif
    for(int i = 0; i < TotalMatrix; i++)
    {
        Mesa_FreeGLMatrix(m_matrix + i);
    }
}

void karin_GLWidget::init()
{
#if defined(_GLSL)
    GL_NewProgram(&m_defaultprog, "./glsl/default.vert", "./glsl/default.frag");
#endif
}

void karin_GLWidget::initializeGL()
{
    QGLWidget::makeCurrent();
#if defined(_GLSL) && defined(_OS_WIN32)
    getgl2func();
#endif
    printfgl();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glStencilMask(~0);

    init();
}

void karin_GLWidget::paintGL()
{
    //glFlush();
}

void karin_GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void karin_GLWidget::setmvp(const GLmatrix *mat)
{
    Mesa_glLoadIdentity(m_matrix + ModelviewMatrix);
    Mesa_glLoadMatrix(m_matrix + ModelviewMatrix, m_matrix[ViewMatrix].m);

    if(mat)
    {
        Mesa_glMultMatrix(m_matrix + ModelviewMatrix, mat->m);
    }

    Mesa_glLoadMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ProjectionMatrix].m);
    Mesa_glMultMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ModelviewMatrix].m);

#if defined(_GLSL)
    GL_Uniform(&m_defaultprog, u_ModelviewMatrix, 1, m_matrix[ModelviewMatrix].m);
    GL_Uniform(&m_defaultprog, u_ProjectionMatrix, 1, m_matrix[ProjectionMatrix].m);
    GL_Uniform(&m_defaultprog, u_ModelviewProjectionMatrix, 1, m_matrix[ModelviewProjectionMatrix].m);
#else // not use OpenGL matrix stack, like glPushMatrix()/glPopMatrix for GLSL render
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_matrix[ModelviewMatrix].m);
#endif
}

void karin_GLWidget::setcamera()
{
    Mesa_glLoadIdentity(m_matrix + ViewMatrix);
    camtrans_gl(m_matrix + ViewMatrix, &m_cam);
#if 0
    glRotatef(VECTOR3_X(m_cam.rotation), 1, 0, 0);
    glRotatef(VECTOR3_Y(m_cam.rotation), 0, 1, 0);
    glTranslatef(
                -VECTOR3_X(m_cam.position),
                -VECTOR3_Y(m_cam.position),
                -VECTOR3_Z(m_cam.position)
                );
#endif
}

void karin_GLWidget::render2d(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
#if 1
    Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    Mesa_gluOrtho2D(m_matrix + ProjectionMatrix, left, right, bottom, top);
#if !defined(_GLSL)
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_matrix[ProjectionMatrix].m);
#endif
#else
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
}

void karin_GLWidget::render3d(GLfloat fovy, GLfloat w, GLfloat h, GLfloat n, GLfloat f)
{
    GLfloat th;

    th = h != 0.0 ? h : 1;
#if 1
    Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    Mesa_gluPerspective(m_matrix + ProjectionMatrix, fovy, w / th, n, f);
#if !defined(_GLSL)
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_matrix[ProjectionMatrix].m);
#endif
#else
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.01, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
}

void karin_GLWidget::mousePressEvent(QMouseEvent* event)
{
    m_lastpos = event->pos();
    m_pressed = true;
    if(event->modifiers() & Qt::ControlModifier)
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    event->accept();

    QGLWidget::mousePressEvent(event);
}

void karin_GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    vector3_s v;

    if(m_pressed)
    {
        if((event->modifiers() & Qt::ControlModifier) == 0)
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

void karin_GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressed = false;
    m_lastpos.setX(0);
    m_lastpos.setY(0);
    QGLWidget::mouseReleaseEvent(event);
}

void karin_GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(!keyev(event, true))
        QGLWidget::keyPressEvent(event);
}

void karin_GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(!keyev(event, false))
        QGLWidget::keyReleaseEvent(event);
}

void karin_GLWidget::transform()
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
    //QTimer::singleShot(50, this, SLOT(transform()));
}

bool karin_GLWidget::keyev(QKeyEvent *event, bool pressed)
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
            transform();
            r = true;
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

void karin_GLWidget::handleidle(bool b)
{
    if(b)
    {
        if(!m_timer)
        {
            m_timer = new QTimer(this);
            m_timer->setInterval(karin_GLWidget::M_Timer_Interval);
            m_timer->setSingleShot(false);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(idle()));
        }

        m_timer->start();
    }
    else
    {
        if(m_timer)
            m_timer->stop();
    }
}

void karin_GLWidget::resizeEvent(QResizeEvent *event)
{
    QGLWidget::resizeEvent(event);
    emit sizechanged(event->size());
}
