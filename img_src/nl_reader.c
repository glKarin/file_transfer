#include "nl_std.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int nlReadFile(NL_texture_s *r, const char *file)
{
#define NL_FREAD(ptr, size, count) \
    if((n = fread((ptr), (size), (count), (f))) != (count)) \
    { \
       res = NL_READ_ERROR; \
       goto __Exit; \
    }

    FILE *f;
    char head[FILE_FLAG_LEN] = { 0 };
    int i;
    int isnlt;
    int res;
    int n;
    int size;
    int s;

    res = NL_NO_ERROR;
    f = NULL;
    isnlt = 1;

    if(!r || !file)
    {
        res = NL_MISSING_PARAMTERS;
        goto __Exit;
    }

    f = fopen(file, "rb");

    if(!f)
    {
        res = NL_FILE_CAN_NOT_READ;
        goto __Exit;
    }

    //memset(r, 0, sizeof(NL_texture_s));
    NL_FREAD(head, 1, 3)

    for(i = 0; i < FILE_FLAG_LEN; i++)
    {
        if(head[i] != FILE_FLAG_CHARS[i])
        {
            isnlt = 0;
            break;
        }
    }
    if(!isnlt)
    {
        res = NL_NOT_A_NLT_FILE;
        goto __Exit;
    }
    memcpy(r->flag, head, FILE_FLAG_LEN);

    NL_FREAD(&r->version, 4, 1)

    NL_FREAD(&r->magic, 4, 1) // check magic

    NL_FREAD(&r->alpha, 1, 1)
    s = r->alpha ? 4 : 3; // RGBA or RGB

    NL_FREAD(&r->width, 4, 1)
    NL_FREAD(&r->height, 4, 1)

    size = r->width * r->height;

    r->color_table = (uint8_t *)calloc(size * s, 1);
    NL_FREAD(r->color_table, 1, size * s)

__Exit:
    if(f)
        fclose(f);
    if(res != NL_NO_ERROR)
        nlSetErrno(res);

    return res;

#undef NL_FREAD
}

int nlWriteFile(const NL_texture_s *r, const char *file)
{
#define NL_FWRITE(ptr, size, count) \
    if((n = fwrite((ptr), (size), (count), (f))) != (count)) \
    { \
       res = NL_READ_ERROR; \
       goto __Exit; \
    }

    FILE *f;
    int res;
    int n;
    int s;

    f = NULL;

    res = NL_NO_ERROR;

    if(!r || !file)
    {
        res = NL_MISSING_PARAMTERS;
        goto __Exit;
    }

    f = fopen(file, "wb");

    if(!f)
    {
        res = NL_FILE_CAN_NOT_WRITE;
        goto __Exit;
    }

    res = nlIsNLTextureStruct(r);
    if(res != NL_NO_ERROR)
    {
        goto __Exit;
    }

    NL_FWRITE(r->flag, 1, FILE_FLAG_LEN)

    NL_FWRITE(&r->version, 4, 1)

    NL_FWRITE(&r->magic, 4, 1)

    NL_FWRITE(&r->alpha, 1, 1)
    s = r->alpha ? 4 : 3; // RGBA or RGB
    NL_FWRITE(&r->width, 4, 1)
    NL_FWRITE(&r->height, 4, 1)

    NL_FWRITE(r->color_table, 1, r->width * r->height * s)

__Exit:
    if(f)
        fclose(f);
    if(res != NL_NO_ERROR)
        nlSetErrno(res);

    return res;

#undef NL_FWRITE
}

int nlIsNLTextureFile(const char *file)
{
    FILE *f;
    char head[FILE_FLAG_LEN] = { 0 };
    int i;
    int isnlt;
    int res;
    int err;

    res = -1;
    err = NL_NO_ERROR;
    f = NULL;
    isnlt = 1;

    if(!file)
    {
        err = NL_MISSING_PARAMTERS;
        goto __Exit;
    }

    f = fopen(file, "rb");

    if(!f)
    {
        err = NL_FILE_CAN_NOT_READ;
        goto __Exit;
    }

    //memset(r, 0, sizeof(NL_texture_s));
    if(fread(head, 1, FILE_FLAG_LEN, f) != FILE_FLAG_LEN)
    {
        err = NL_NOT_A_NLT_FILE;
        goto __Exit;
    }

    for(i = 0; i < FILE_FLAG_LEN; i++)
    {
        if(head[i] != FILE_FLAG_CHARS[i])
        {
            isnlt = 0;
            break;
        }
    }
    if(!isnlt)
    {
        err = NL_NOT_A_NLT_FILE;
        goto __Exit;
    }

    res = 0;

__Exit:
    if(f)
        fclose(f);
    return res;
}
