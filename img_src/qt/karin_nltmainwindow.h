#ifndef KARIN_NLTMAINWINDOW_H
#define KARIN_NLTMAINWINDOW_H

#include <QMainWindow>

class karin_NLTViewer;

namespace Ui {
class karin_NLTMainWindow;
}

class karin_NLTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit karin_NLTMainWindow(QWidget *parent = 0);
    ~karin_NLTMainWindow();

private:
    void init();

private Q_SLOTS:
    void open();
    void save();
    void view();
    
private:
    Ui::karin_NLTMainWindow *ui;
    karin_NLTViewer *m_viewer;
    QString m_filepath;
};

#endif // KARIN_NLTMAINWINDOW_H
