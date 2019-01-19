#include "nl_std.h"

#include <stdlib.h>

#define NL_COLOR_R(x) (x)[0]
#define NL_COLOR_G(x) (x)[1]
#define NL_COLOR_B(x) (x)[2]
#define NL_COLOR_A(x) (x)[3]

#define NL_CMP3(a, b) \
    ((a)[0] == (b)[0] && (a)[1] == (b)[1] && (a)[2] == (b)[2])
#define NL_CMP4(a, b) \
    ((a)[0] == (b)[0] && (a)[1] == (b)[1] && (a)[2] == (b)[2] && (a)[3] == (b)[3])

#define NL_CPY3(a, b) \
{\
    (a)[0] == (b)[0]; \
    (a)[1] == (b)[1]; \
    (a)[2] == (b)[2]; \
}

#define NL_CPY4(a, b) \
{\
    (a)[0] = (b)[0]; \
    (a)[1] = (b)[1]; \
    (a)[2] = (b)[2]; \
    (a)[3] = (b)[3]; \
}

int nlMakeNLTexture(NL_texture_s *r, int w, int h, int alpha, const GLubyte *data)
{
    int size;
    int i, j;
    GLubyte *colors;
    const GLubyte *c;
    const GLubyte *c8;
    const uint8_t *cstd8;
    uint32_t count, index;
    int has;
    int s;

    if(!r || !data)
        return NL_MISSING_PARAMTERS;

    if(w <= 0)
        return NL_INVALID_WIDTH;
    if(h <= 0)
        return NL_INVALID_HEIGHT;

    count = 0;
    index = 0;
    s = alpha ? 4 : 3;

    r->flag[0] = FILE_FLAG_CHARS[0];
    r->flag[1] = FILE_FLAG_CHARS[1];
    r->flag[2] = FILE_FLAG_CHARS[2];

    r->version = NL_VERSION;
    r->magic = NL_MAGIC;

    r->alpha = alpha ? 1 : 0;
    r->width = w;
    r->height = h;

    size = r->width * r->height;
    colors = (GLubyte *)calloc(size * s, sizeof(GLubyte));

    r->color_table = (uint8_t *)calloc(size * s, 1);

    memcpy(r->color_table, data, size * s);

    return NL_NO_ERROR;
}

// GL_RGBA GL_UNSIGNED_BYTE
int nlMakeGLTexture(GLubyte **r, const NL_texture_s *nl, int *w, int *h)
{
    GLubyte *data;
    int size;
    int i;
    uint32_t index;
    GLubyte *c8;
    const uint8_t *cstd8;
    int s;

    if(!r || !nl)
        return NL_MISSING_PARAMTERS;
    if(!nlIsNLTextureStruct(nl))
        return NL_NOT_A_NLT_STRUCT;

    size = nl->width * nl->height;
    s = nl->alpha ? 4 : 3;
    data = (GLubyte *)calloc(size * s, sizeof(GLubyte));

    memcpy(data, nl->color_table, size * s);

    *r = data;

    return NL_NO_ERROR;
}
