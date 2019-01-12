#include "karin_glwidget.h"
#include "gl/gl_std.h"
#include "gl/qmath.h"

karin_GLWidget::karin_GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
#if defined(_GLSL)
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
#if defined(_GLSL)
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
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_matrix[ProjectionMatrix].m);
#else
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
}

void karin_GLWidget::render3d(GLfloat fovy, GLfloat w, GLfloat h, GLfloat near, GLfloat far)
{
    GLfloat th;

    th = h != 0.0 ? h : 1;
#if 1
    Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    Mesa_gluPerspective(m_matrix + ProjectionMatrix, fovy, w / th, 0.01, 10000);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_matrix[ProjectionMatrix].m);
#else
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.01, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
}
