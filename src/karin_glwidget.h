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

class QTimer;

class karin_GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit karin_GLWidget(QWidget *parent = 0);
    virtual ~karin_GLWidget();
    
signals:
    void sizechanged(const QSize &s);
    
public slots:
    void handleidle(bool b);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void init();
    virtual void deinit();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void setcamera();
    void setmvp(const GLmatrix *mat);
    void render2d(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);
    void render3d(GLfloat fovy, GLfloat w, GLfloat h, GLfloat near, GLfloat far);

protected Q_SLOTS:
    virtual void idle() {}

private:
    bool keyev(QKeyEvent *event, bool pressed);

private Q_SLOTS:
    void transform();

protected:
#if defined(_GLSL)
        GL_program_s m_defaultprog;
#endif
    GLmatrix m_matrix[TotalMatrix];
    camera_s m_cam;
    QTimer *m_timer;
    GLfloat m_movesens;
    GLfloat m_turnsens;
    bool m_pressed;
    QPoint m_lastpos;

private:
    bool m_direction[Direction_Total];
    bool m_rotation[Rotation_Total];

    static const int M_Timer_Interval = 100;
    static const int M_Move_Unit = 10;
    
};

#endif // KARIN_GLWIDGET_H
