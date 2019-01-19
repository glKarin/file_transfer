#include "nl_std.h"

#include <stdlib.h>

int nlIsNLTextureStruct(const NL_texture_s *nl)
{
#define NL_RET(err) \
    { \
        res = err; \
        goto __Exit; \
    }

    int i;
    int isnlt;
    int res;

    res = NL_NO_ERROR;

    if(!nl)
    {
        NL_RET(NL_MISSING_PARAMTERS)
    }

    // check head
    isnlt = 1;
    for(i = 0; i < FILE_FLAG_LEN; i++)
    {
        if(nl->flag[i] != FILE_FLAG_CHARS[i])
        {
            isnlt = 0;
            break;
        }
    }
    if(!isnlt)
    {
        NL_RET(NL_NOT_A_NLT_FILE)
    }

    if(nl->magic != NL_MAGIC)
    {
        NL_RET(NL_INVALID_MAGIC)
    }

    if(nl->width == 0)
    {
        NL_RET(NL_INVALID_WIDTH)
    }
    if(nl->height == 0)
    {
        NL_RET(NL_INVALID_HEIGHT)
    }

    if(!nl->color_table)
    {
        NL_RET(NL_COLOR_TABLE_IS_NULL)
    }

__Exit:
    return res;
}

int nlFreeNLTexture(NL_texture_s *r)
{
    int res;

    res = NL_NO_ERROR;

    if(!r)
    {
        res = NL_MISSING_PARAMTERS;
        goto __Exit;
    }

    if(r->color_table)
        free(r->color_table);

__Exit:
    if(res != NL_NO_ERROR)
        nlSetErrno(res);
    return res;
}
