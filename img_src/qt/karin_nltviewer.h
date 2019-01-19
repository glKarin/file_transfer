#ifndef KARIN_NLTVIEWER_H
#define KARIN_NLTVIEWER_H

#include "../src/karin_glwidget.h"

class karin_NLTViewer : public karin_GLWidget
{
    Q_OBJECT
public:
    explicit karin_NLTViewer(QWidget *parent = 0);
    virtual ~karin_NLTViewer();
    
signals:
    
public slots:
    void open(const QString &qfile);
    void tonlt(const QString &dst, const QString &src);

protected:
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void init();
    virtual void deinit();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    void rs();
    
private:
    mesh_s m_mesh;
    float m_scale;
};

#endif // KARIN_NLTVIEWER_H
