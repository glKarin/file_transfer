#ifndef _KARIN_GL_STD_H
#define _KARIN_GL_STD_H

#ifdef _OS_WIN32
#ifdef _GLSL
#define GLEW_STATIC
#include "GL/glew.h"
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
#else // win32 glew
#ifndef WINGDIAPI
#define GLEW_WINGDIAPI_DEFINED
#define WINGDIAPI __declspec(dllimport)
#endif

#ifdef APIENTRY
#  ifndef GLAPIENTRY
#    define GLAPIENTRY APIENTRY
#  endif
#  ifndef GLEWAPIENTRY
#    define GLEWAPIENTRY APIENTRY
#  endif
#else
#define GLEW_APIENTRY_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__) || (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#    define APIENTRY __stdcall
#    ifndef GLAPIENTRY
#      define GLAPIENTRY __stdcall
#    endif
#    ifndef GLEWAPIENTRY
#      define GLEWAPIENTRY __stdcall
#    endif
#  else
#    define APIENTRY
#  endif
#endif

#ifndef CALLBACK
#define GLEW_CALLBACK_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define CALLBACK __attribute__ ((__stdcall__))
#  elif (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#  else
#    define CALLBACK
#  endif
#endif

#if (defined(_MSC_VER) || defined(__BORLANDC__)) && !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#  define _WCHAR_T_DEFINED
#endif
#if !defined(_W64)
#  if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && defined(_MSC_VER) && _MSC_VER >= 1300
#    define _W64 __w64
#  else
#    define _W64
#  endif
#endif
#if !defined(_PTRDIFF_T_DEFINED) && !defined(_PTRDIFF_T_) && !defined(__MINGW64__)
#  ifdef _WIN64
typedef __int64 ptrdiff_t;
#  else
typedef _W64 int ptrdiff_t;
#  endif
#  define _PTRDIFF_T_DEFINED
#  define _PTRDIFF_T_
#endif

#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  else
#    define GLAPI WINGDIAPI
#  endif
#endif


#include <GL/gl.h>
#include <GL/glu.h>
#endif // gl vs2010
#else // win32

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

typedef GLfloat GLdouble;
#define GL_DOUBLE GL_FLOAT
#endif // gles 2.0

#ifdef __cplusplus
extern "C" {
#endif

#define RESTRICT

extern const GLfloat White_Color[];

#if defined(_GLSL) && defined(_OS_WIN32)
void getgl2func(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // GL_STD_H
