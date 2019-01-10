/********************************************************************************
** Form generated from reading UI file 'karin_settingdialog.ui'
**
** Created: Sat Jan 5 20:41:44 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KARIN_SETTINGDIALOG_H
#define UI_KARIN_SETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_karin_SettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *title;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *group_box;
    QWidget *formLayoutWidget;
    QFormLayout *form_layout;
    QLabel *thread_label;
    QSpinBox *thread;
    QLabel *enable_log_label;
    QCheckBox *enable_log;
    QLabel *log_level_label;
    QComboBox *log_level;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *karin_SettingDialog)
    {
        if (karin_SettingDialog->objectName().isEmpty())
            karin_SettingDialog->setObjectName(QString::fromUtf8("karin_SettingDialog"));
        karin_SettingDialog->resize(983, 529);
        karin_SettingDialog->setModal(true);
        verticalLayout = new QVBoxLayout(karin_SettingDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        title = new QLabel(karin_SettingDialog);
        title->setObjectName(QString::fromUtf8("title"));
        title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(title);

        scrollArea = new QScrollArea(karin_SettingDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 963, 462));
        verticalLayoutWidget = new QWidget(scrollAreaWidgetContents);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 961, 461));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        group_box = new QGroupBox(verticalLayoutWidget);
        group_box->setObjectName(QString::fromUtf8("group_box"));
        formLayoutWidget = new QWidget(group_box);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 20, 961, 441));
        form_layout = new QFormLayout(formLayoutWidget);
        form_layout->setObjectName(QString::fromUtf8("form_layout"));
        form_layout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        form_layout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        form_layout->setVerticalSpacing(20);
        form_layout->setContentsMargins(10, 0, 10, 0);
        thread_label = new QLabel(formLayoutWidget);
        thread_label->setObjectName(QString::fromUtf8("thread_label"));

        form_layout->setWidget(0, QFormLayout::LabelRole, thread_label);

        thread = new QSpinBox(formLayoutWidget);
        thread->setObjectName(QString::fromUtf8("thread"));

        form_layout->setWidget(0, QFormLayout::FieldRole, thread);

        enable_log_label = new QLabel(formLayoutWidget);
        enable_log_label->setObjectName(QString::fromUtf8("enable_log_label"));

        form_layout->setWidget(1, QFormLayout::LabelRole, enable_log_label);

        enable_log = new QCheckBox(formLayoutWidget);
        enable_log->setObjectName(QString::fromUtf8("enable_log"));

        form_layout->setWidget(1, QFormLayout::FieldRole, enable_log);

        log_level_label = new QLabel(formLayoutWidget);
        log_level_label->setObjectName(QString::fromUtf8("log_level_label"));

        form_layout->setWidget(2, QFormLayout::LabelRole, log_level_label);

        log_level = new QComboBox(formLayoutWidget);
        log_level->setObjectName(QString::fromUtf8("log_level"));

        form_layout->setWidget(2, QFormLayout::FieldRole, log_level);


        verticalLayout_2->addWidget(group_box);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        buttonBox = new QDialogButtonBox(karin_SettingDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(karin_SettingDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), karin_SettingDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), karin_SettingDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(karin_SettingDialog);
    } // setupUi

    void retranslateUi(QDialog *karin_SettingDialog)
    {
        karin_SettingDialog->setWindowTitle(QApplication::translate("karin_SettingDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("karin_SettingDialog", "Setting", 0, QApplication::UnicodeUTF8));
        group_box->setTitle(QApplication::translate("karin_SettingDialog", "General", 0, QApplication::UnicodeUTF8));
        thread_label->setText(QApplication::translate("karin_SettingDialog", "Max working thread", 0, QApplication::UnicodeUTF8));
        enable_log_label->setText(QApplication::translate("karin_SettingDialog", "Enable log", 0, QApplication::UnicodeUTF8));
        enable_log->setText(QString());
        log_level_label->setText(QApplication::translate("karin_SettingDialog", "Log level", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class karin_SettingDialog: public Ui_karin_SettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KARIN_SETTINGDIALOG_H
