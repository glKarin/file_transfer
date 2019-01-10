#include "gl_std.h"

#include <stdio.h>

const GLfloat White_Color[] = { 1, 1, 1, 1 };

#if defined(_GLSL) && defined(_OS_WIN32)
static GLboolean gl_func_init = GL_FALSE;

void getgl2func(void)
{
#ifdef _OS_WIN32
    GLenum glewerr;

    glewerr = glewInit();
    if(glewerr != GLEW_OK)
    {
        fprintf(stderr, "[error]: glewInit return %X (%s), OpenGL init fail. exit...\n", glewerr, glewGetErrorString(glewerr));
        exit(1);
    }
    gl_func_init = (glewerr == GLEW_OK);
#else
   gl_func_init = GL_TRUE;
#endif
}
#endif
