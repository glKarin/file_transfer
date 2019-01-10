/********************************************************************************
** Form generated from reading UI file 'karin_progressdialog.ui'
**
** Created: Sat Jan 5 20:41:43 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KARIN_PROGRESSDIALOG_H
#define UI_KARIN_PROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_karin_ProgressDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *main_layout;
    QLabel *title;
    QStackedWidget *stacked_layout;
    QWidget *work_page;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *work_layout;
    QSpacerItem *verticalSpacer;
    QLabel *work_label;
    QSpacerItem *verticalSpacer_4;
    QLabel *work_desc;
    QProgressBar *work_progressbar;
    QSpacerItem *verticalSpacer_2;
    QWidget *result_page;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *result_layout;
    QLabel *result_label;
    QDialogButtonBox *tools;

    void setupUi(QDialog *karin_ProgressDialog)
    {
        if (karin_ProgressDialog->objectName().isEmpty())
            karin_ProgressDialog->setObjectName(QString::fromUtf8("karin_ProgressDialog"));
        karin_ProgressDialog->setWindowModality(Qt::ApplicationModal);
        karin_ProgressDialog->resize(640, 360);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(karin_ProgressDialog->sizePolicy().hasHeightForWidth());
        karin_ProgressDialog->setSizePolicy(sizePolicy);
        karin_ProgressDialog->setModal(true);
        verticalLayoutWidget = new QWidget(karin_ProgressDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 640, 360));
        main_layout = new QVBoxLayout(verticalLayoutWidget);
        main_layout->setObjectName(QString::fromUtf8("main_layout"));
        main_layout->setContentsMargins(0, 0, 0, 0);
        title = new QLabel(verticalLayoutWidget);
        title->setObjectName(QString::fromUtf8("title"));
        title->setAlignment(Qt::AlignCenter);

        main_layout->addWidget(title);

        stacked_layout = new QStackedWidget(verticalLayoutWidget);
        stacked_layout->setObjectName(QString::fromUtf8("stacked_layout"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stacked_layout->sizePolicy().hasHeightForWidth());
        stacked_layout->setSizePolicy(sizePolicy1);
        work_page = new QWidget();
        work_page->setObjectName(QString::fromUtf8("work_page"));
        sizePolicy.setHeightForWidth(work_page->sizePolicy().hasHeightForWidth());
        work_page->setSizePolicy(sizePolicy);
        verticalLayoutWidget_2 = new QWidget(work_page);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(0, 0, 640, 360));
        work_layout = new QVBoxLayout(verticalLayoutWidget_2);
        work_layout->setObjectName(QString::fromUtf8("work_layout"));
        work_layout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        work_layout->addItem(verticalSpacer);

        work_label = new QLabel(verticalLayoutWidget_2);
        work_label->setObjectName(QString::fromUtf8("work_label"));
        work_label->setAlignment(Qt::AlignCenter);

        work_layout->addWidget(work_label);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        work_layout->addItem(verticalSpacer_4);

        work_desc = new QLabel(verticalLayoutWidget_2);
        work_desc->setObjectName(QString::fromUtf8("work_desc"));
        work_desc->setWordWrap(true);

        work_layout->addWidget(work_desc);

        work_progressbar = new QProgressBar(verticalLayoutWidget_2);
        work_progressbar->setObjectName(QString::fromUtf8("work_progressbar"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(work_progressbar->sizePolicy().hasHeightForWidth());
        work_progressbar->setSizePolicy(sizePolicy2);
        work_progressbar->setValue(100);
        work_progressbar->setAlignment(Qt::AlignCenter);

        work_layout->addWidget(work_progressbar);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        work_layout->addItem(verticalSpacer_2);

        stacked_layout->addWidget(work_page);
        result_page = new QWidget();
        result_page->setObjectName(QString::fromUtf8("result_page"));
        verticalLayoutWidget_3 = new QWidget(result_page);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(0, 0, 791, 391));
        result_layout = new QVBoxLayout(verticalLayoutWidget_3);
        result_layout->setObjectName(QString::fromUtf8("result_layout"));
        result_layout->setContentsMargins(0, 0, 0, 0);
        result_label = new QLabel(verticalLayoutWidget_3);
        result_label->setObjectName(QString::fromUtf8("result_label"));

        result_layout->addWidget(result_label);

        stacked_layout->addWidget(result_page);

        main_layout->addWidget(stacked_layout);

        tools = new QDialogButtonBox(verticalLayoutWidget);
        tools->setObjectName(QString::fromUtf8("tools"));
        tools->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tools->setCenterButtons(true);

        main_layout->addWidget(tools);


        retranslateUi(karin_ProgressDialog);

        QMetaObject::connectSlotsByName(karin_ProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *karin_ProgressDialog)
    {
        karin_ProgressDialog->setWindowTitle(QApplication::translate("karin_ProgressDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        title->setText(QString());
        work_label->setText(QString());
        work_desc->setText(QString());
        result_label->setText(QApplication::translate("karin_ProgressDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class karin_ProgressDialog: public Ui_karin_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KARIN_PROGRESSDIALOG_H
