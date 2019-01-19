#include "karin_glsplash.h"
#include "karin_std.h"
#include "ada/obj.h.c"
#include "shadow.h"

#include <QDebug>
#include <QMouseEvent>
#include <math.h>
#include <time.h>

#define FRUSTUM_Z_NEAR 0.01
#define FRUSTUM_Z_FAR 1000

#define arrcpy3(n, o) {\
    n[0] = o[0]; \
    n[1] = o[1]; \
    n[2] = o[2]; \
    }
#define M_CUBE_ROTATION_SENS 0.2

static unsigned render_type = 1; // for testing mask
static const int Mesh_Count = 3;

#if 0
static mesh_s ada;
#endif

karin_GLSplash::karin_GLSplash(QWidget *parent) :
    karin_GLWidget(parent),
    m_cubeangle(Vector3D(0.0, 0.0, 0.0)),
    m_dirlighting(false),
    m_cubersens(M_CUBE_ROTATION_SENS)
{
    qsrand(time(0));
    vector3_s viewpos = Vector3D(0.395481, 2.8357, 6.89706);
    vector3_s viewangle = Vector3D(30, 344, 0.0);

    setFixedSize(karin_GLSplash::M_Width, karin_GLSplash::M_Width);

    m_count[0] = m_count[1] = m_count[2] = randr(40, 80);
    memset(m_cur, 0, sizeof(int) * 3);
    m_orientation[0] = m_orientation[1] = m_orientation[2] = randr(10, 18);
    initcam(&m_cam, &viewpos, &viewangle);
    m_lightpos << Vector3D(0.0, 2.0, 0.0)
               << Vector3D(-6.0, 6.0, -4.0)
               << Vector3D(-4.0, 4.0, 4.0)
               << Vector3D(2.0, 1.8, 0.5);

    handleidle(true);
}

karin_GLSplash::~karin_GLSplash()
{
}

void karin_GLSplash::init()
{
    karin_GLWidget::init();

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
    m_mesh.materials[0].color[0] = m_mesh.materials[0].color[1] = m_mesh.materials[0].color[2] = 1.0;
    m_mesh.materials[0].color[3] = 1.0;
    m_mesh.materials[0].use_color = true;

    karinCube_MinMax(min, max, m_mesh.materials + 1);
    m_mesh.materials[1].use_color = false;
    karinCube_MinMax(min2, max2, m_mesh.materials + 2);
    m_mesh.materials[2].use_color = false;
    karinCube_MinMax(min3, max3, m_mesh.materials + 3);
    m_mesh.materials[3].use_color = false;
    m_mesh.materials[3].rotation[1] = 59;

#if 0
    ada_parse(&ada, "./ada/AdaORCObj.obj");
#endif

#if !defined(_GLSL)
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
#endif

    glEnable(GL_BLEND);
}

void karin_GLSplash::deinit()
{
    freemesh(&m_mesh);

    karin_GLWidget::deinit();
}

void karin_GLSplash::initializeGL()
{
    karin_GLWidget::initializeGL();
}

void karin_GLSplash::paintGL()
{
    static const QString Fmt("(%1, %2, %3) [%4, %5]");

    GLmatrix mat;
    Mesa_AllocGLMatrix(&mat);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render3d(45, size().width(), size().height(), FRUSTUM_Z_NEAR, FRUSTUM_Z_FAR);
    {
#if defined(_GLSL)
        GL_UseProgram(&m_defaultprog);
        GL_EnableVertexAttribute(&m_defaultprog, a_Vertex);
#else
        glEnableClientState(GL_VERTEX_ARRAY);
#endif

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
        GLfloat lp[] = { 0, 0, 0 };
        GLfloat ps = 10;
        setcamera();
#if defined(_GLSL)
        GL_Uniform(&m_defaultprog, u_Color, 1, White_Color);
        GL_VertexAttribute(&m_defaultprog, a_Vertex, 3, 0, lp);
        GL_Uniform(&m_defaultprog, u_PointSize, 1, &ps);
#else
        GLfloat cps;
        glGetFloatv(GL_POINT_SIZE, &cps);
        glPointSize(ps);
        glColor4fv(White_Color);
        glVertexPointer(3, GL_FLOAT, 0, lp);
#endif
        for(int i = 0; i < m_lightpos.size(); i++)
        {
            Mesa_glLoadIdentity(&mat);
            Mesa_glTranslate(&mat, VECTOR3_X(m_lightpos[i]), VECTOR3_Y(m_lightpos[i]), VECTOR3_Z(m_lightpos[i]));
            setmvp(&mat);
            glDrawArrays(GL_POINTS, 0, 1);
        }
#if !defined(_GLSL)
        glPointSize(cps);
#endif

        //printfcam(m_cam);

#if defined(_GLSL)
        GL_DisableVertexAttribute(&m_defaultprog, a_Vertex);
#else
        glDisableClientState(GL_VERTEX_ARRAY);
#endif
    }

    glFlush();

    Mesa_FreeGLMatrix(&mat);
}

void karin_GLSplash::resizeGL(int w, int h)
{
    karin_GLWidget::resizeGL(w, h);
}

void karin_GLSplash::rendershadowscene()
{
    static const GLfloat Shadow_Mask_Color[] = {
        0.0,
        0.0,
        0.0,
        0.2,
    };
    glClear(GL_STENCIL_BUFFER_BIT);

    renderscene();

    for(int i = 0; i < m_lightpos.size(); i++)
    {
        setcamera();
        setmvp(0);

        shadowrmesh(&m_mesh, &(m_lightpos[i]), m_dirlighting, 1, Mesh_Count);

        Mesa_glLoadIdentity(m_matrix + ViewMatrix);
        Mesa_glLoadIdentity(m_matrix + ModelviewMatrix);
        setmvp(0);
        shadowrmask(Shadow_Mask_Color);
    }

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void karin_GLSplash::renderscene()
{
    GLmatrix mat;
    int i;

    Mesa_AllocGLMatrix(&mat);

    setmvp(0);
    glDisable(GL_BLEND);
    rendermat(m_mesh.materials);
    glEnable(GL_BLEND);

#if defined(_GLSL)
        GL_EnableVertexAttribute(GL_currprog, a_Color);
#else
        glEnableClientState(GL_COLOR_ARRAY);
#endif
    for(i = 1; i < m_mesh.count; i++)
    {
        Mesa_glLoadIdentity(&mat);
        Mesa_glRotate(&mat, m_mesh.materials[i].rotation[0], 1, 0, 0);
        Mesa_glRotate(&mat, m_mesh.materials[i].rotation[1], 0, 1, 0);
        Mesa_glRotate(&mat, m_mesh.materials[i].rotation[2], 0, 0, 1); // ??

        Mesa_glTranspose(&mat);

        setmvp(&mat);
        rendermat(m_mesh.materials + i);
    }
#if defined(_GLSL)
        GL_DisableVertexAttribute(GL_currprog, a_Color);
#else
        glDisableClientState(GL_COLOR_ARRAY);
#endif

__Exit:
    Mesa_FreeGLMatrix(&mat);
}

void karin_GLSplash::mousePressEvent(QMouseEvent* event)
{
    karin_GLWidget::mousePressEvent(event);
}

void karin_GLSplash::mouseMoveEvent(QMouseEvent* event)
{
    vector3_s v;
    bool r;

    r = false;
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
                r = true;
                updateGL();
            }
        }
    }
    if(!r)
        karin_GLWidget::mouseMoveEvent(event);
}

void karin_GLSplash::mouseReleaseEvent(QMouseEvent* event)
{
    karin_GLWidget::mouseReleaseEvent(event);
}

void karin_GLSplash::keyPressEvent(QKeyEvent *event)
{
    if(!keyev(event, true))
        karin_GLWidget::keyPressEvent(event);
}

void karin_GLSplash::keyReleaseEvent(QKeyEvent *event)
{
    if(!keyev(event, false))
        karin_GLWidget::keyReleaseEvent(event);
}

bool karin_GLSplash::keyev(QKeyEvent *event, bool pressed)
{
    bool r;

    r = false;

    if(event->modifiers() == Qt::NoModifier)
    {
        switch(event->key())
        {
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
        default:
            goto __Exit;
            break;
        }
        r = true;
    }

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
