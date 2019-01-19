#ifndef KARIN_GLSPLASH_H
#define KARIN_GLSPLASH_H

#include "karin_glwidget.h"

class QTimer;

class karin_GLSplash : public karin_GLWidget
{
    Q_OBJECT
public:
    explicit karin_GLSplash(QWidget *parent = 0);
    virtual ~karin_GLSplash();
    
signals:
    
public slots:

protected:
    void init();
    void deinit();
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    void rendershadow(const mesh_s *cube, const vector3_s *lpos, int render_count);
    void rendershadowscene();
    void renderscene();
    void starttimer();
    bool keyev(QKeyEvent *event, bool pressed);
    vector3_s lightingdir(const GLfloat v[3], const vector3_s *lightpos, bool dirlight = false) const;
    void shadowvol(mesh_s *r, const vector3_s *lightpos, const material_s *mat) const;
    void caletrans(material_s *r, const material_s *src, const GLmatrix *mat) const;

protected Q_SLOTS:
    virtual void idle();

private:
    mesh_s m_mesh;
    vector3_s m_cubeangle;
    QList<vector3_s> m_lightpos;
    bool m_dirlighting;

    GLfloat m_cubersens;
    int m_count[3];
    int m_cur[3];
    int m_orientation[3];

    static const int M_Width = 512;
    
};

#endif // KARIN_GLSPLASH_H
