/********************************************************************************
** Form generated from reading UI file 'karin_filewindow.ui'
**
** Created: Sat Jan 5 20:41:43 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KARIN_FILEWINDOW_H
#define UI_KARIN_FILEWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_karin_FileWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *main_layout;
    QComboBox *dir_input;

    void setupUi(QWidget *karin_FileWindow)
    {
        if (karin_FileWindow->objectName().isEmpty())
            karin_FileWindow->setObjectName(QString::fromUtf8("karin_FileWindow"));
        karin_FileWindow->resize(640, 360);
        verticalLayoutWidget = new QWidget(karin_FileWindow);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(-1, -1, 971, 431));
        main_layout = new QVBoxLayout(verticalLayoutWidget);
        main_layout->setObjectName(QString::fromUtf8("main_layout"));
        main_layout->setContentsMargins(0, 0, 0, 0);
        dir_input = new QComboBox(verticalLayoutWidget);
        dir_input->setObjectName(QString::fromUtf8("dir_input"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dir_input->sizePolicy().hasHeightForWidth());
        dir_input->setSizePolicy(sizePolicy);
        dir_input->setEditable(true);
        dir_input->setMaxCount(20);
        dir_input->setInsertPolicy(QComboBox::InsertAtTop);

        main_layout->addWidget(dir_input);


        retranslateUi(karin_FileWindow);

        QMetaObject::connectSlotsByName(karin_FileWindow);
    } // setupUi

    void retranslateUi(QWidget *karin_FileWindow)
    {
        karin_FileWindow->setWindowTitle(QApplication::translate("karin_FileWindow", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class karin_FileWindow: public Ui_karin_FileWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KARIN_FILEWINDOW_H
