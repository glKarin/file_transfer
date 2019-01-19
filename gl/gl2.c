#include "gl2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_GLSL)

struct GL_program_property_s
{
	int location;
	enum GL_program_property_type_e type;
	enum GL_program_property_data_e data;
	const char *name;
};

#define GL_ATTR(L, D) \
{ a_##L, GL_program_attribute, GL_program_##D, "a_"#L }
static const struct GL_program_property_s GL_Attributes[] = {
	GL_ATTR(Vertex, vec4),
	GL_ATTR(Normal, vec3),
	GL_ATTR(Color, vec4),
	GL_ATTR(Texcoord, vec2),
	GL_ATTR(Texcoord2, vec2),
	GL_ATTR(Texcoord3, vec2),
	GL_ATTR(Texcoord4, vec2),
	GL_ATTR(Texcoord5, vec2),
	GL_ATTR(Texcoord6, vec2),
	GL_ATTR(Texcoord7, vec2),
	GL_ATTR(Texcoord8, vec2),
	GL_ATTR(Fogcoord, float),
	GL_ATTR(SecondaryColor, vec4),
};

#define GL_UNI1(L, D) \
{ u_##L, GL_program_uniform, GL_program_##D, "u_"#L }
#define GL_UNI2(L, D, N) \
{ u_##L, GL_program_uniform, GL_program_##D, #N }
static const struct GL_program_property_s GL_Uniforms[] = {
	GL_UNI1(ModelviewMatrix, mat4),
	GL_UNI1(ProjectionMatrix, mat4),
	GL_UNI1(ModelviewProjectionMatrix, mat4),
	GL_UNI1(NormalMatrix, mat3),
	GL_UNI1(Color, vec4),
	GL_UNI1(Texture2D, sampler2D),
	GL_UNI1(Texture2D2, sampler2D),
	GL_UNI1(Texture2D3, sampler2D),
	GL_UNI1(Texture2D4, sampler2D),
	GL_UNI1(Texture2D5, sampler2D),
	GL_UNI1(Texture2D6, sampler2D),
	GL_UNI1(Texture2D7, sampler2D),
	GL_UNI1(Texture2D8, sampler2D),
	GL_UNI1(PointSize, float),
};

const struct GL_program_s *GL_currprog;

static char * GL_ShaderSource(const char *file);
static int GL_GetAttributeLocation(GLuint program, GLint r[]);
static int GL_GetUniformLocation(GLuint program, GLint r[]);

int GL_UseProgram(const struct GL_program_s *p)
{
	if(!p || !glIsProgram(p->program))
	{
		glUseProgram(0);
		GL_currprog = 0;
	}
	else
	{
		glUseProgram(p->program);
		GL_currprog = p;
	}

	return GL_currprog ? GL_currprog->program : 0;
}

int GL_EnableVertexAttribute(const struct GL_program_s *p, unsigned attr)
{
	if(!p || attr >= a_TotalAttribute)
		return -1;
	if(p->attribute[attr] == -1)
		return -1;
	glEnableVertexAttribArray(p->attribute[attr]);
	return 0;
}

int GL_DisableVertexAttribute(const struct GL_program_s *p, unsigned attr)
{
	if(!p || attr >= a_TotalAttribute)
		return -1;
	if(p->attribute[attr] == -1)
		return -1;
	glDisableVertexAttribArray(p->attribute[attr]);
	return 0;
}

int GL_NewProgram(struct GL_program_s *r, const char *vs, const char *fs)
{
	GLuint vshader;
	GLuint fshader;
	GLuint program;
	int res;

	if(!r || !vs || !fs)
		return -1;

	vshader = fshader = 0;
	program = 0;

	vshader = GL_CreateShader(GL_VERTEX_SHADER, vs);
	if(vshader == 0)
		goto __Exit;
	fshader = GL_CreateShader(GL_FRAGMENT_SHADER, fs);
	if(fshader == 0)
		goto __Exit;

	program = GL_CreateProgram(vshader, fshader);

__Exit:
	if(program == 0)
	{
		if(glIsShader(vshader))
			glDeleteShader(vshader);
		if(glIsShader(fshader))
			glDeleteShader(fshader);
		res = -1;
	}
	else
	{
		r->program = program;
		r->vshader = vshader;
		r->fshader = fshader;
		GL_GetAttributeLocation(program, r->attribute);
		GL_GetUniformLocation(program, r->uniform);
		res = 0;
	}
	return res;
}

void GL_VertexAttribute(const struct GL_program_s *p, unsigned attr, int size, int stride, const void *data)
{
	enum GL_program_property_data_e data_type;
	GLenum type;

	if(!p || !data || attr >= a_TotalAttribute)
		return;
	if(p->attribute[attr] == -1)
		return;

	data_type = GL_Attributes[attr].data;
	switch(data_type)
	{
		case GL_program_bool:
		case GL_program_bvec2:
		case GL_program_bvec3:
		case GL_program_bvec4:
		case GL_program_int:
		case GL_program_ivec2:
		case GL_program_ivec3:
		case GL_program_ivec4:
			type = GL_INT;
			break;

		case GL_program_float:
		case GL_program_vec2:
		case GL_program_vec3:
		case GL_program_vec4:
			type = GL_FLOAT;
			break;

		default:
			return;
	}

	glVertexAttribPointer(p->attribute[attr], size, type, GL_FALSE, stride, data);
}

void GL_Uniform(const struct GL_program_s *p, unsigned unif, int count, const void *data)
{
	enum GL_program_property_data_e data_type;

	if(!p || !data || unif >= u_TotalUniform)
		return;
	if(p->uniform[unif] == -1)
		return;

	data_type = GL_Uniforms[unif].data;
	switch(data_type)
	{
		case GL_program_bool:
		case GL_program_int:
		case GL_program_sampler1D:
		case GL_program_sampler2D:
		case GL_program_sampler3D:
            glUniform1iv(p->uniform[unif], count, (const GLint *)data);
			break;
		case GL_program_float:
            glUniform1fv(p->uniform[unif], count, (const GLfloat *)data);
			break;
		case GL_program_bvec2:
		case GL_program_ivec2:
            glUniform2iv(p->uniform[unif], count, (const GLint *)data);
			break;
		case GL_program_bvec3:
		case GL_program_ivec3:
            glUniform3iv(p->uniform[unif], count, (const GLint *)data);
			break;
		case GL_program_ivec4:
		case GL_program_bvec4:
            glUniform4iv(p->uniform[unif], count, (const GLint *)data);
			break;

		case GL_program_vec2:
            glUniform2fv(p->uniform[unif], count, (const GLfloat *)data);
			break;
		case GL_program_vec3:
            glUniform3fv(p->uniform[unif], count, (const GLfloat *)data);
			break;
		case GL_program_vec4:
            glUniform4fv(p->uniform[unif], count, (const GLfloat *)data);
			break;

		case GL_program_mat2:
            glUniformMatrix2fv(p->uniform[unif], count, GL_FALSE, (const GLfloat *)data);
			break;
		break;

		case GL_program_mat3:
            glUniformMatrix3fv(p->uniform[unif], count, GL_FALSE, (const GLfloat *)data);
		break;

		case GL_program_mat4:
            glUniformMatrix4fv(p->uniform[unif], count, GL_FALSE, (const GLfloat *)data);
		break;

		case GL_program_mat2x3:
		case GL_program_mat2x4:
		case GL_program_mat3x2:
		case GL_program_mat3x4:
		case GL_program_mat4x2:
		case GL_program_mat4x3:
			return;
		default:
			return;
	}

}

int GL_CreateShader(GLenum type, const char *file)
{
	char *src;
	GLint len;
	GLint state;
	char *log;
	int r;

	if(!file)
		return 0;

	src = GL_ShaderSource(file);
	if(!src)
		return 0;

	len = strlen(src);
	r = glCreateShader(type);
    glShaderSource(r, 1, (const char **)&src, &len);
	glCompileShader(r);

	glGetShaderiv(r, GL_COMPILE_STATUS, &state);
	if(!state)
	{
		glGetShaderiv(r, GL_INFO_LOG_LENGTH, &len);
        log = (char *)calloc(len + 1, sizeof(char));
		glGetShaderInfoLog(r, len, 0, log);
		fprintf(stderr, "[Error]: Compile shader %X -> %s\n", type, log);
		free(log);
		glDeleteShader(r);
		r = 0;
	}

	free(src);

	return r;
}

int GL_CreateProgram(GLuint vs, GLuint fs)
{
	int r;
	GLint state;
	GLint len;
	char *log;

	if(!glIsShader(vs) || !glIsShader(fs))
		return 0;

	r = glCreateProgram();
	glAttachShader(r, vs);
	glAttachShader(r, fs);
	glLinkProgram(r);

	glGetProgramiv(r, GL_LINK_STATUS, &state);
	if(!r)
	{
		glGetProgramiv(r, GL_INFO_LOG_LENGTH, &len);
        log = (char *)calloc(len + 1, sizeof(char));
		glGetProgramInfoLog(r, len, 0, log);
		fprintf(stderr, "[Error]: Link program -> %s\n", log);
		free(log);
		glDeleteProgram(r);
		r = 0;
	}

	return r;
}

void GL_DeleteProgram(struct GL_program_s *p)
{
    if(!p)
        return;
    if(glIsProgram(p->program))
    {
        glDetachShader(p->program, p->vshader);
        glDetachShader(p->program, p->fshader);
        glDeleteShader(p->vshader);
        glDeleteShader(p->fshader);
        glDeleteProgram(p->program);
        p->program = 0;
        p->vshader = 0;
        p->fshader = 0;
    }
}

int GL_GetAttributeLocation(GLuint program, GLint r[])
{
	int i;

	if(!glIsProgram(program) || !r)
		return -1;

	for(i = 0; i < a_TotalAttribute; i++)
	{
		r[i] = glGetAttribLocation(program, GL_Attributes[i].name);
	}
	return 0;
}

int GL_GetUniformLocation(GLuint program, GLint r[])
{
	int i;

	if(!glIsProgram(program) || !r)
		return -1;

	for(i = 0; i < u_TotalUniform; i++)
	{
		r[i] = glGetUniformLocation(program, GL_Uniforms[i].name);
	}
	return 0;
}

char * GL_ShaderSource(const char *file)
{
	FILE *f;
	int size;
	char *r;

	if(!file)
		return 0;

	f = fopen(file, "r");

	if(!f)
		return 0;

	fseek(f, 0, SEEK_END);
	size = ftell(f);
    r = (char *)calloc(size + 1, 1);

	fseek(f, 0, SEEK_SET);
	fread(r, size, 1, f);

	fclose(f);

	return r;
}

#endif
