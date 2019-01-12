#include "nl_errno.h"

NL_err_t nl_error = NL_NO_ERROR;

const char * nlGetErrorString(NL_err_t err)
{
    static const char *NL_Error_Str[] = {
        "No error",
        "Missing paramters",
        "File can not read",
        "File can not write",
        "File read error",
        "File write error",
    };
    int e;

    e = -err;

    if(e >= 0 || e < sizeof(NL_Error_Str) / sizeof(NL_Error_Str[0]))
        return NL_Error_Str[e];
    return 0;
}

NL_err_t nlSetErrno(NL_err_t err)
{
    NL_err_t last;

    last = nl_error;

    nl_error = err;

    return last;
}
