#-------------------------------------------------
#
# Project created by QtCreator 2018-12-24T11:50:28
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = file_transformer
TEMPLATE = app

DEFINES += _APP_NAME=\\\"FileTransfer\\\" \
_APP_DEV=\\\"karin\\\" \
_APP_VER=\\\"0.0.1harmattan1\\\" \
_APP_CODE=\\\"katyusha\\\" \
_APP_RELEASE=20181224 \
_APP_PKG=\\\"filetransfer\\\"


SOURCES += main.cpp\
        mainwindow.cpp \
    karin_filewindow.cpp \
    karin_ut.cpp \
    karin_fsmodel.cpp \
    karin_fileengine.cpp \
    karin_progressdialog.cpp \
    karin_fileview.cpp \
    karin_fileengine_p.cpp \
    karin_std.cpp \
    karin_settingdialog.cpp \
    karin_glsplash.cpp \
    mesa_gl_math.c \
    glut2.cpp

HEADERS  += mainwindow.h \
    karin_filewindow.h \
    karin_ut.h \
    karin_fsmodel.h \
    karin_std.h \
    karin_fileengine.h \
    karin_progressdialog.h \
    karin_fileview.h \
    karin_fileengine_p.h \
    karin_settingdialog.h \
    karin_glsplash.h \
    mesa_gl_math.h \
    glut2.h

HEADERS += \
    gl/vector3.h \
    gl/camera.h \
    gl/mesh.h \
    gl/qmath.h \
    gl/gl_std.h

SOURCES += \
    gl/vector3.c \
    gl/camera.c \
    gl/mesh.c

FORMS    += mainwindow.ui \
    karin_filewindow.ui \
    karin_progressdialog.ui \
    karin_settingdialog.ui

RESOURCES += \
    karin.qrc

OTHER_FILES += \
    debian/changelog

# Mesa GL math
HEADERS += \
 mesa_math/m_clip_tmp.h \
           mesa_math/m_copy_tmp.h \
           mesa_math/m_debug.h \
           mesa_math/m_debug_util.h \
           mesa_math/m_dotprod_tmp.h \
           mesa_math/m_eval.h \
           mesa_math/m_matrix.h \
           mesa_math/m_norm_tmp.h \
           mesa_math/m_trans_tmp.h \
           mesa_math/m_translate.h \
           mesa_math/m_vector.h \
           mesa_math/m_xform.h \
           mesa_math/m_xform_tmp.h \
           mesa_math/main/compiler.h \
           mesa_math/main/config.h \
           mesa_math/main/glheader.h \
           mesa_math/main/imports.h \
           mesa_math/main/macros.h
SOURCES += \
 mesa_math/m_debug_clip.c \
           mesa_math/m_debug_norm.c \
           mesa_math/m_debug_xform.c \
           mesa_math/m_eval.c \
           mesa_math/m_matrix.c \
           mesa_math/m_translate.c \
           mesa_math/m_vector.c \
           mesa_math/m_xform.c \
           mesa_math/main/imports.c
