#ifndef KARIN_GLWIDGET_H
#define KARIN_GLWIDGET_H

#include "mesa_gl_math.h"
#include "gl/gl_std.h"
#include "gl/gl2.h"
#include "gl/camera.h"
#include "gl/qmath.h"
#include "glut2.h"
#include "gl/mesh.h"

#include <QGLWidget>

class karin_GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit karin_GLWidget(QWidget *parent = 0);
    virtual ~karin_GLWidget();
    
signals:
    
public slots:

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void init();
    virtual void deinit();

    void setcamera();
    void setmvp(const GLmatrix *mat);
    void render2d(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);
    void render3d(GLfloat fovy, GLfloat w, GLfloat h, GLfloat near, GLfloat far);

protected:
#if defined(_GLSL)
        GL_program_s m_defaultprog;
#endif
    GLmatrix m_matrix[TotalMatrix];
    camera_s m_cam;
    
};

#endif // KARIN_GLWIDGET_H
