#-------------------------------------------------
#
# Project created by QtCreator 2018-12-24T11:50:28
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = file_transfer
TEMPLATE = app

DEFINES += _APP_NAME=\\\"FileTransfer\\\" \
_APP_DEV=\\\"karin\\\" \
_APP_VER=\\\"0.0.1harmattan1\\\" \
_APP_CODE=\\\"katyusha\\\" \
_APP_RELEASE=20181224 \
_APP_PKG=\\\"$${TARGET}\\\" \
_APP_EMAIL=\\\"beyondk2000@gmail.com\\\" \
_APP_GITHUB=\\\"https://github.com/glKarin/file_transfer\\\"

INCLUDEPATH += gl src img_src SOIL

#INCLUDEPATH += glew\\include
#LIBS += "E:\\pro\\qt\\file_transfer\\glew\\lib\\Release\\Win32\\glew32s.lib"

contains(MEEGO_EDITION, harmattan){
DEFINES += _HARMATTAN
DEFINES += _GLSL
}
DEFINES += _OS_WIN32
#DEFINES += _GLSL


# file
SOURCES += main.cpp\
        src/mainwindow.cpp \
    src/karin_filewindow.cpp \
    src/karin_ut.cpp \
    src/karin_fsmodel.cpp \
    src/karin_fileengine.cpp \
    src/karin_progressdialog.cpp \
    src/karin_fileview.cpp \
    src/karin_fileengine_p.cpp \
    src/karin_std.cpp \
    src/karin_settingdialog.cpp \
    src/karin_glsplash.cpp \
    img_src/nl_std.c \
    img_src/nl_gl.c \
    img_src/qt/karin_nltviewer.cpp \
    src/karin_glwidget.cpp \
    img_src/qt/karin_nltmainwindow.cpp \
    img_src/nl_errno.c \
    img_src/nl_reader.c

HEADERS  += src/mainwindow.h \
    src/karin_filewindow.h \
    src/karin_ut.h \
    src/karin_fsmodel.h \
    src/karin_std.h \
    src/karin_fileengine.h \
    src/karin_progressdialog.h \
    src/karin_fileview.h \
    src/karin_fileengine_p.h \
    src/karin_settingdialog.h \
    src/karin_glsplash.h \
    img_src/nl_errno.h \
    img_src/nl_std.h \
    img_src/qt/karin_nltviewer.h \
    src/karin_glwidget.h \
    img_src/qt/karin_nltmainwindow.h

# gl
HEADERS += \
    gl/vector3.h \
    gl/camera.h \
    gl/mesh.h \
    gl/qmath.h \
    gl/gl2.h \
    gl/gl_std.h \
    gl/mesa_gl_math.h \
    gl/glut2.h

SOURCES += \
    gl/vector3.c \
    gl/camera.c \
    gl/mesh.c \
    gl/gl2.c \
    gl/gl_std.c \
    gl/mesa_gl_math.c \
    gl/glut2.cpp

#SOIL
HEADERS += SOIL/image_DXT.h \
           SOIL/image_helper.h \
           SOIL/SOIL.h \
           SOIL/stb_image_aug.h \
           SOIL/stbi_DDS_aug.h \
           SOIL/stbi_DDS_aug_c.h

SOURCES += SOIL/image_DXT.c \
           SOIL/image_helper.c \
           SOIL/SOIL.c \
           SOIL/stb_image_aug.c


FORMS    += mainwindow.ui \
    karin_filewindow.ui \
    karin_progressdialog.ui \
    karin_settingdialog.ui \
    img_src/qt/karin_nltmainwindow.ui

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

glsl.files = \
    glsl/default.vert \
    glsl/default.frag
glsl.path = ./glsl

i18n.files = \
    i18n/$${TARGET}.zh_CN.ts \
    i18n/$${TARGET}.zh_CN.qm
i18n.path = ./i18n

INSTALLS        += i18n glsl
