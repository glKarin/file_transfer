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
    virtual void init();
    virtual void deinit();
    
private:
    mesh_s m_mesh;
};

#endif // KARIN_NLTVIEWER_H
