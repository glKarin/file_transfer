#include "mesa_gl_math.h"

#include "mesa_math/main/imports.h"

#define arrcpy16(n, o) {\
    n[0] = o[0]; \
    n[1] = o[1]; \
    n[2] = o[2]; \
    n[3] = o[3]; \
    n[4] = o[4]; \
    n[5] = o[5]; \
    n[6] = o[6]; \
    n[7] = o[7]; \
    n[8] = o[8]; \
    n[9] = o[9]; \
    n[10] = o[10]; \
    n[11] = o[11]; \
    n[12] = o[12]; \
    n[13] = o[13]; \
    n[14] = o[14]; \
    n[15] = o[15]; \
}


#define IF_NULL_RETURN(p) if(!p) return;

void Mesa_glTranslate(GLmatrix *mat, float x, float y, float z)
{
    IF_NULL_RETURN(mat)
        _math_matrix_translate( mat, x, y, z );
}

void Mesa_glRotate(GLmatrix *mat, float angle, float x, float y, float z)
{
    IF_NULL_RETURN(mat)
        if(angle != 0.0F)
            _math_matrix_rotate( mat, angle, x, y, z );
}

void Mesa_glScale(GLmatrix *mat, float x, float y, float z)
{
    IF_NULL_RETURN(mat)
        _math_matrix_scale( mat, x, y, z );
}

void Mesa_glLoadMatrix(GLmatrix *mat, const float *m)
{
    IF_NULL_RETURN(mat)
    if (!m) return;
    _math_matrix_loadf( mat, m );
}

void Mesa_glLoadTransposeMatrix(GLmatrix *mat, const float *m)
{
    IF_NULL_RETURN(mat)
   if (!m) return;
   float tm[16];
   _math_transposef(tm, m);
   Mesa_glLoadMatrix(mat, tm);
}

void Mesa_glLoadIdentity(GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
        _math_matrix_set_identity( mat );
}

void Mesa_glMultMatrix(GLmatrix *mat, const float *m)
{
   IF_NULL_RETURN(mat)
   if (!m) return;
   _math_matrix_mul_floats( mat, m );
}

void Mesa_glMultTransposeMatrix(GLmatrix *mat, const float *m)
{
    IF_NULL_RETURN(mat)
   if (!m) return;
   float tm[16];
   _math_transposef(tm, m);
   Mesa_glMultMatrix(mat, tm);
}

void Mesa_glFrustum(GLmatrix *mat, float left, float right, float bottom, float top, float nearval, float farval)
{
   IF_NULL_RETURN(mat)
   if (nearval <= 0.0 ||
       farval <= 0.0 ||
       nearval == farval ||
       left == right ||
       top == bottom)
   {
      _mesa_error( NULL,  GL_INVALID_VALUE, "glFrustum" );
      return;
   }

     _math_matrix_frustum( mat,
             left, right,
             bottom, top,
             nearval, farval );
}

void Mesa_glOrtho(GLmatrix *mat, float left, float right, float bottom, float top, float nearval, float farval)
{
    IF_NULL_RETURN(mat)

   if (left == right ||
       bottom == top ||
       nearval == farval)
   {
       _mesa_error( NULL,  GL_INVALID_VALUE, "glOrtho" );
       return;
   }

    _math_matrix_ortho( mat,
            left, right,
            bottom, top,
            nearval, farval );
}

void Mesa_glTransform(float r[3], const float p[3], const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    float v[4] = {p[0], p[1], p[2], 1};
    float u[4];
    _mesa_transform_vector(u, v, mat->m);
    r[0] = u[0] / u[3];
    r[1] = u[1] / u[3];
    r[2] = u[2] / u[3];
}

void Mesa_glTransform4(float r[4], const float p[4], const GLmatrix *mat)
{
    IF_NULL_RETURN(mat)
    _mesa_transform_vector(r, p, mat->m);
}

void Mesa_AllocGLMatrix(GLmatrix *mat)
{
    if(!mat)
        return;
    _math_matrix_ctr(mat);
    _math_matrix_alloc_inv(mat);
}

void Mesa_FreeGLMatrix(GLmatrix *mat)
{
    if(!mat)
        return;
    _math_matrix_dtr(mat);
}

void Mesa_NormalMatrix(GLmatrix *mat, const GLfloat mv[16])
{
    GLmatrix nor;
    GLfloat tmp[16];
    GLfloat dst[16];
    if(!mat || !mv)
    {
        return;
    }

    Mesa_AllocGLMatrix(&nor);
    Mesa_glLoadIdentity(&nor);
    _math_matrix_loadf(&nor, mv);
    _math_matrix_analyse(&nor);
    arrcpy16(tmp, nor.inv);
    _math_transposef(dst, tmp);
    Mesa_FreeGLMatrix(&nor);

    _math_matrix_loadf(mat, dst);
}
