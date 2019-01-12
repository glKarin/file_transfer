#ifndef _KARIN_NL_STD_H
#define _KARIN_NL_STD_H

#include "nl_errno.h"
#include "../gl/gl_std.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FILE_FLAG_LEN 3
#define FILE_FLAG_CHARS "nlt"
#define NL_MAGIC 2009

#define NL_VERSION_1_0_0_1 0x01000001
#define NL_VERSION NL_VERSION_1_0_0_1

#define NL_UNUSED(x) (void)(x)

typedef struct _NL_texture_s
{
    int8_t flag[FILE_FLAG_LEN];
    uint32_t version;
    uint32_t magic;

    uint8_t alpha;
    uint16_t width;
    uint16_t height;

    uint32_t *index_table;

    uint32_t color_count;
    uint8_t *color_table;
} NL_texture_s;

int nlReadFile(NL_texture_s *r, const char *file);
int nlWriteFile(const NL_texture_s *r, const char *file);
int nlIsNLTextureFile(const char *file);
int nlFreeNLTexture(NL_texture_s *r);
int nlIsNLTextureStruct(const NL_texture_s *nl);
int nlMakeNLTexture(NL_texture_s *r, int w, int h, int alpha, const GLubyte *data);
int nlMakeGLTexture(GLubyte **r, const NL_texture_s *nl, int *w, int *h);

#ifdef __cplusplus
}
#endif

#endif // IMG_STRUCT_H
