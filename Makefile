#############################################################################
# Makefile for building: file_transformer
# Generated by qmake (2.01a) (Qt 4.7.4) on: ?? 1? 6 12:46:17 2019
# Project:  file_transformer.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile file_transformer.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -D_APP_NAME=\"FileTransfer\" -D_APP_DEV=\"karin\" -D_APP_VER=\"0.0.1harmattan1\" -D_APP_CODE=\"katyusha\" -D_APP_RELEASE=20181224 -D_APP_PKG=\"filetransfer\" -D_APP_EMAIL=\"beyondk2000@gmail.com\" -D_APP_GITHUB=\"https://github.com/glKarin/file_transfer\" -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -g -Wno-psabi -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -g -Wno-psabi -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-maemo -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -I/usr/X11R6/include -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib -L/usr/X11R6/lib -lQtOpenGL -lQtGui -lQtCore -lGLESv2 -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
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
		glut2.cpp \
		gl/vector3.c \
		gl/camera.c \
		gl/mesh.c \
		gl/gl2.c \
		mesa_math/m_debug_clip.c \
		mesa_math/m_debug_norm.c \
		mesa_math/m_debug_xform.c \
		mesa_math/m_eval.c \
		mesa_math/m_matrix.c \
		mesa_math/m_translate.c \
		mesa_math/m_vector.c \
		mesa_math/m_xform.c \
		mesa_math/main/imports.c moc_mainwindow.cpp \
		moc_karin_filewindow.cpp \
		moc_karin_fsmodel.cpp \
		moc_karin_fileengine.cpp \
		moc_karin_progressdialog.cpp \
		moc_karin_fileview.cpp \
		moc_karin_fileengine_p.cpp \
		moc_karin_settingdialog.cpp \
		moc_karin_glsplash.cpp \
		qrc_karin.cpp
OBJECTS       = main.o \
		mainwindow.o \
		karin_filewindow.o \
		karin_ut.o \
		karin_fsmodel.o \
		karin_fileengine.o \
		karin_progressdialog.o \
		karin_fileview.o \
		karin_fileengine_p.o \
		karin_std.o \
		karin_settingdialog.o \
		karin_glsplash.o \
		mesa_gl_math.o \
		glut2.o \
		vector3.o \
		camera.o \
		mesh.o \
		gl2.o \
		m_debug_clip.o \
		m_debug_norm.o \
		m_debug_xform.o \
		m_eval.o \
		m_matrix.o \
		m_translate.o \
		m_vector.o \
		m_xform.o \
		imports.o \
		moc_mainwindow.o \
		moc_karin_filewindow.o \
		moc_karin_fsmodel.o \
		moc_karin_fileengine.o \
		moc_karin_progressdialog.o \
		moc_karin_fileview.o \
		moc_karin_fileengine_p.o \
		moc_karin_settingdialog.o \
		moc_karin_glsplash.o \
		qrc_karin.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		file_transformer.pro
QMAKE_TARGET  = file_transformer
DESTDIR       = 
TARGET        = file_transformer

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_mainwindow.h ui_karin_filewindow.h ui_karin_progressdialog.h ui_karin_settingdialog.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: file_transformer.pro  /usr/share/qt4/mkspecs/linux-g++-maemo/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtOpenGL.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -o Makefile file_transformer.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtOpenGL.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile file_transformer.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/file_transformer1.0.0 || $(MKDIR) .tmp/file_transformer1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/file_transformer1.0.0/ && $(COPY_FILE) --parents mainwindow.h karin_filewindow.h karin_ut.h karin_fsmodel.h karin_std.h karin_fileengine.h karin_progressdialog.h karin_fileview.h karin_fileengine_p.h karin_settingdialog.h karin_glsplash.h mesa_gl_math.h glut2.h gl/vector3.h gl/camera.h gl/mesh.h gl/qmath.h gl/gl2.h gl/gl_std.h mesa_math/m_clip_tmp.h mesa_math/m_copy_tmp.h mesa_math/m_debug.h mesa_math/m_debug_util.h mesa_math/m_dotprod_tmp.h mesa_math/m_eval.h mesa_math/m_matrix.h mesa_math/m_norm_tmp.h mesa_math/m_trans_tmp.h mesa_math/m_translate.h mesa_math/m_vector.h mesa_math/m_xform.h mesa_math/m_xform_tmp.h mesa_math/main/compiler.h mesa_math/main/config.h mesa_math/main/glheader.h mesa_math/main/imports.h mesa_math/main/macros.h .tmp/file_transformer1.0.0/ && $(COPY_FILE) --parents karin.qrc .tmp/file_transformer1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp karin_filewindow.cpp karin_ut.cpp karin_fsmodel.cpp karin_fileengine.cpp karin_progressdialog.cpp karin_fileview.cpp karin_fileengine_p.cpp karin_std.cpp karin_settingdialog.cpp karin_glsplash.cpp mesa_gl_math.c glut2.cpp gl/vector3.c gl/camera.c gl/mesh.c gl/gl2.c mesa_math/m_debug_clip.c mesa_math/m_debug_norm.c mesa_math/m_debug_xform.c mesa_math/m_eval.c mesa_math/m_matrix.c mesa_math/m_translate.c mesa_math/m_vector.c mesa_math/m_xform.c mesa_math/main/imports.c .tmp/file_transformer1.0.0/ && $(COPY_FILE) --parents mainwindow.ui karin_filewindow.ui karin_progressdialog.ui karin_settingdialog.ui .tmp/file_transformer1.0.0/ && (cd `dirname .tmp/file_transformer1.0.0` && $(TAR) file_transformer1.0.0.tar file_transformer1.0.0 && $(COMPRESS) file_transformer1.0.0.tar) && $(MOVE) `dirname .tmp/file_transformer1.0.0`/file_transformer1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/file_transformer1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_karin_filewindow.cpp moc_karin_fsmodel.cpp moc_karin_fileengine.cpp moc_karin_progressdialog.cpp moc_karin_fileview.cpp moc_karin_fileengine_p.cpp moc_karin_settingdialog.cpp moc_karin_glsplash.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_karin_filewindow.cpp moc_karin_fsmodel.cpp moc_karin_fileengine.cpp moc_karin_progressdialog.cpp moc_karin_fileview.cpp moc_karin_fileengine_p.cpp moc_karin_settingdialog.cpp moc_karin_glsplash.cpp
moc_mainwindow.cpp: mainwindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_karin_filewindow.cpp: karin_filewindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_filewindow.h -o moc_karin_filewindow.cpp

moc_karin_fsmodel.cpp: karin_fsmodel.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_fsmodel.h -o moc_karin_fsmodel.cpp

moc_karin_fileengine.cpp: karin_fileengine_p.h \
		karin_fileengine.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_fileengine.h -o moc_karin_fileengine.cpp

moc_karin_progressdialog.cpp: karin_progressdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_progressdialog.h -o moc_karin_progressdialog.cpp

moc_karin_fileview.cpp: karin_fileview.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_fileview.h -o moc_karin_fileview.cpp

moc_karin_fileengine_p.cpp: karin_fileengine_p.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_fileengine_p.h -o moc_karin_fileengine_p.cpp

moc_karin_settingdialog.cpp: karin_settingdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_settingdialog.h -o moc_karin_settingdialog.cpp

moc_karin_glsplash.cpp: glut2.h \
		gl/mesh.h \
		gl/gl_std.h \
		gl/camera.h \
		gl/vector3.h \
		mesa_gl_math.h \
		mesa_math/m_matrix.h \
		mesa_math/main/glheader.h \
		gl/qmath.h \
		karin_glsplash.h
	/usr/bin/moc $(DEFINES) $(INCPATH) karin_glsplash.h -o moc_karin_glsplash.cpp

compiler_rcc_make_all: qrc_karin.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_karin.cpp
qrc_karin.cpp: karin.qrc \
		resc/image/icon-l-notes.png \
		resc/image/icon-l-folder-empty.png \
		resc/image/icon-m-content-url.png \
		resc/image/icon-m-email-combined-mailbox.png \
		debian/changelog
	/usr/bin/rcc -name karin karin.qrc -o qrc_karin.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h ui_karin_filewindow.h ui_karin_progressdialog.h ui_karin_settingdialog.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h ui_karin_filewindow.h ui_karin_progressdialog.h ui_karin_settingdialog.h
ui_mainwindow.h: mainwindow.ui
	/usr/bin/uic mainwindow.ui -o ui_mainwindow.h

ui_karin_filewindow.h: karin_filewindow.ui
	/usr/bin/uic karin_filewindow.ui -o ui_karin_filewindow.h

ui_karin_progressdialog.h: karin_progressdialog.ui
	/usr/bin/uic karin_progressdialog.ui -o ui_karin_progressdialog.h

ui_karin_settingdialog.h: karin_settingdialog.ui
	/usr/bin/uic karin_settingdialog.ui -o ui_karin_settingdialog.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		karin_std.h \
		karin_ut.h \
		karin_fileengine_p.h \
		karin_glsplash.h \
		glut2.h \
		gl/mesh.h \
		gl/gl_std.h \
		gl/camera.h \
		gl/vector3.h \
		mesa_gl_math.h \
		mesa_math/m_matrix.h \
		mesa_math/main/glheader.h \
		gl/qmath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		ui_mainwindow.h \
		karin_filewindow.h \
		karin_fileengine.h \
		karin_fileengine_p.h \
		karin_progressdialog.h \
		karin_ut.h \
		karin_std.h \
		karin_settingdialog.h \
		karin_glsplash.h \
		glut2.h \
		gl/mesh.h \
		gl/gl_std.h \
		gl/camera.h \
		gl/vector3.h \
		mesa_gl_math.h \
		mesa_math/m_matrix.h \
		mesa_math/main/glheader.h \
		gl/qmath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

karin_filewindow.o: karin_filewindow.cpp karin_filewindow.h \
		ui_karin_filewindow.h \
		karin_fsmodel.h \
		karin_fileview.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_filewindow.o karin_filewindow.cpp

karin_ut.o: karin_ut.cpp karin_ut.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_ut.o karin_ut.cpp

karin_fsmodel.o: karin_fsmodel.cpp karin_fsmodel.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_fsmodel.o karin_fsmodel.cpp

karin_fileengine.o: karin_fileengine.cpp karin_fileengine.h \
		karin_fileengine_p.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_fileengine.o karin_fileengine.cpp

karin_progressdialog.o: karin_progressdialog.cpp karin_progressdialog.h \
		ui_karin_progressdialog.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_progressdialog.o karin_progressdialog.cpp

karin_fileview.o: karin_fileview.cpp karin_fileview.h \
		karin_fsmodel.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_fileview.o karin_fileview.cpp

karin_fileengine_p.o: karin_fileengine_p.cpp karin_fileengine_p.h \
		karin_fileengine.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_fileengine_p.o karin_fileengine_p.cpp

karin_std.o: karin_std.cpp karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_std.o karin_std.cpp

karin_settingdialog.o: karin_settingdialog.cpp karin_settingdialog.h \
		ui_karin_settingdialog.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_settingdialog.o karin_settingdialog.cpp

karin_glsplash.o: karin_glsplash.cpp karin_glsplash.h \
		glut2.h \
		gl/mesh.h \
		gl/gl_std.h \
		gl/camera.h \
		gl/vector3.h \
		mesa_gl_math.h \
		mesa_math/m_matrix.h \
		mesa_math/main/glheader.h \
		gl/qmath.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o karin_glsplash.o karin_glsplash.cpp

mesa_gl_math.o: mesa_gl_math.c mesa_gl_math.h \
		mesa_math/m_matrix.h \
		mesa_math/main/glheader.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o mesa_gl_math.o mesa_gl_math.c

glut2.o: glut2.cpp glut2.h \
		gl/mesh.h \
		gl/gl_std.h \
		gl/camera.h \
		gl/vector3.h \
		karin_std.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o glut2.o glut2.cpp

vector3.o: gl/vector3.c gl/vector3.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o vector3.o gl/vector3.c

camera.o: gl/camera.c gl/camera.h \
		gl/vector3.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o camera.o gl/camera.c

mesh.o: gl/mesh.c gl/mesh.h \
		gl/gl_std.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o mesh.o gl/mesh.c

gl2.o: gl/gl2.c gl/gl2.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o gl2.o gl/gl2.c

m_debug_clip.o: mesa_math/m_debug_clip.c mesa_math/main/glheader.h \
		mesa_math/main/macros.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/m_matrix.h \
		mesa_math/m_xform.h \
		mesa_math/m_vector.h \
		mesa_math/m_debug.h \
		mesa_math/m_debug_util.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_debug_clip.o mesa_math/m_debug_clip.c

m_debug_norm.o: mesa_math/m_debug_norm.c mesa_math/main/glheader.h \
		mesa_math/main/macros.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/m_matrix.h \
		mesa_math/m_xform.h \
		mesa_math/m_vector.h \
		mesa_math/m_debug.h \
		mesa_math/m_debug_util.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_debug_norm.o mesa_math/m_debug_norm.c

m_debug_xform.o: mesa_math/m_debug_xform.c mesa_math/main/glheader.h \
		mesa_math/main/macros.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/m_matrix.h \
		mesa_math/m_xform.h \
		mesa_math/m_vector.h \
		mesa_math/m_debug.h \
		mesa_math/m_debug_util.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_debug_xform.o mesa_math/m_debug_xform.c

m_eval.o: mesa_math/m_eval.c mesa_math/main/glheader.h \
		mesa_math/main/config.h \
		mesa_math/m_eval.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_eval.o mesa_math/m_eval.c

m_matrix.o: mesa_math/m_matrix.c mesa_math/main/glheader.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/main/macros.h \
		mesa_math/m_matrix.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_matrix.o mesa_math/m_matrix.c

m_translate.o: mesa_math/m_translate.c mesa_math/main/glheader.h \
		mesa_math/main/macros.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/m_translate.h \
		mesa_math/m_trans_tmp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_translate.o mesa_math/m_translate.c

m_vector.o: mesa_math/m_vector.c mesa_math/main/glheader.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/main/macros.h \
		mesa_math/m_vector.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_vector.o mesa_math/m_vector.c

m_xform.o: mesa_math/m_xform.c mesa_math/main/glheader.h \
		mesa_math/main/macros.h \
		mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/m_eval.h \
		mesa_math/m_matrix.h \
		mesa_math/m_translate.h \
		mesa_math/m_xform.h \
		mesa_math/m_vector.h \
		mesa_math/m_debug.h \
		mesa_math/m_xform_tmp.h \
		mesa_math/m_clip_tmp.h \
		mesa_math/m_norm_tmp.h \
		mesa_math/m_dotprod_tmp.h \
		mesa_math/m_copy_tmp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o m_xform.o mesa_math/m_xform.c

imports.o: mesa_math/main/imports.c mesa_math/main/imports.h \
		mesa_math/main/compiler.h \
		mesa_math/main/glheader.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o imports.o mesa_math/main/imports.c

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_karin_filewindow.o: moc_karin_filewindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_filewindow.o moc_karin_filewindow.cpp

moc_karin_fsmodel.o: moc_karin_fsmodel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_fsmodel.o moc_karin_fsmodel.cpp

moc_karin_fileengine.o: moc_karin_fileengine.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_fileengine.o moc_karin_fileengine.cpp

moc_karin_progressdialog.o: moc_karin_progressdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_progressdialog.o moc_karin_progressdialog.cpp

moc_karin_fileview.o: moc_karin_fileview.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_fileview.o moc_karin_fileview.cpp

moc_karin_fileengine_p.o: moc_karin_fileengine_p.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_fileengine_p.o moc_karin_fileengine_p.cpp

moc_karin_settingdialog.o: moc_karin_settingdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_settingdialog.o moc_karin_settingdialog.cpp

moc_karin_glsplash.o: moc_karin_glsplash.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_karin_glsplash.o moc_karin_glsplash.cpp

qrc_karin.o: qrc_karin.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_karin.o qrc_karin.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

