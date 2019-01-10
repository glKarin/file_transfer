#ifndef _KARIN_GL2_H
#define _KARIN_GL2_H

#include "gl_std.h"

#ifdef __cplusplus
extern "C" {
#endif
enum GL_matrix_e
{
	ModelviewMatrix = 0,
	ProjectionMatrix,
	ModelviewProjectionMatrix,
	NormalMatrix,
	ViewMatrix,
	TotalMatrix
};


#if defined(_GLSL)

enum GL_program_property_type_e
{
	GL_program_attribute = 0,
	GL_program_uniform
};

enum GL_program_property_data_e
{
	GL_program_bool = 0,
	GL_program_int,
	GL_program_float,

	GL_program_vec2,
	GL_program_vec3,
	GL_program_vec4,

	GL_program_ivec2,
	GL_program_ivec3,
	GL_program_ivec4,

	GL_program_bvec2,
	GL_program_bvec3,
	GL_program_bvec4,

	GL_program_mat2,
	GL_program_mat2x3,
	GL_program_mat2x4,

	GL_program_mat3x2,
	GL_program_mat3,
	GL_program_mat3x4,

	GL_program_mat4x2,
	GL_program_mat4x3,
	GL_program_mat4,

	GL_program_sampler1D,
	GL_program_sampler2D,
	GL_program_sampler3D,
};

enum GL_program_attribute_e
{
	a_Vertex = 0,
	a_Normal,
	a_Color,
	a_Texcoord,
	a_Texcoord2,
	a_Texcoord3,
	a_Texcoord4,
	a_Texcoord5,
	a_Texcoord6,
	a_Texcoord7,
	a_Texcoord8,
	a_Fogcoord,
	a_SecondaryColor,

	a_TotalAttribute
};

enum GL_program_uniform_e
{
	// matrix
	u_ModelviewMatrix = 0,
	u_ProjectionMatrix,
	u_ModelviewProjectionMatrix,
	u_NormalMatrix,

	// texture
	u_Color,
	u_Texture2D,
	u_Texture2D2,
	u_Texture2D3,
	u_Texture2D4,
	u_Texture2D5,
	u_Texture2D6,
	u_Texture2D7,
	u_Texture2D8,

	// alpha test
	//u_AlphaTest,
	//u_AlphaFunc,
	//u_AlphaRef,

	// state
	u_PointSize,

	u_TotalUniform
};

struct GL_program_s
{
	GLuint program;
	GLuint vshader;
	GLuint fshader;
	GLint attribute[a_TotalAttribute];
	GLint uniform[u_TotalUniform];
};

extern const struct GL_program_s *GL_currprog;

int GL_NewProgram(struct GL_program_s *r, const char *vs, const char *fs);
int GL_CreateShader(GLenum type, const char *file);
int GL_CreateProgram(GLuint vs, GLuint fs);
void GL_DeleteProgram(struct GL_program_s *p);

void GL_VertexAttribute(const struct GL_program_s *p, unsigned attr, int size, int stride, const void *data);
void GL_Uniform(const struct GL_program_s *p, unsigned unif, int count, const void *data);
int GL_UseProgram(const struct GL_program_s *p);
int GL_EnableVertexAttribute(const struct GL_program_s *p, unsigned attr);
int GL_DisableVertexAttribute(const struct GL_program_s *p, unsigned attr);

#endif

#ifdef __cplusplus
}
#endif

#endif
