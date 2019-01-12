#ifndef _KARIN_NL_ERRNO_H
#define _KARIN_NL_ERRNO_H

#define NL_NO_ERROR 0
#define NL_MISSING_PARAMTERS -1
#define NL_FILE_CAN_NOT_READ -2
#define NL_FILE_CAN_NOT_WRITE -3
#define NL_READ_ERROR -4
#define NL_WRITE_ERROR -5
#define NL_NOT_A_NLT_FILE -6
#define NL_NOT_A_NLT_STRUCT -7
#define NL_INVALID_MAGIC -8
#define NL_INVALID_WIDTH -9
#define NL_INVALID_HEIGHT -10
#define NL_INDEX_TABLE_IS_NULL -11
#define NL_COLOR_TABLE_IS_NULL -12
#define NL_INVALID_COLOR_TABLE_LENGTH -13

#ifdef __cplusplus
extern "C" {
#endif

typedef int NL_err_t;

extern NL_err_t nl_error;

const char * nlGetErrorString(NL_err_t err);
NL_err_t nlSetErrno(NL_err_t err);

#ifdef __cplusplus
}
#endif

#endif // NL_ERRNO_H
