#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

static bool
check_compile_error(GLuint s)  {
	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(s, 512, NULL, infoLog);
		printf("ERROR:shader: vertex compile failed:%s\n", infoLog);
	}
	if (!success) {
		return false;
	}
	return true;
}


void 
shader_use(const struct shader *s) {
	glUseProgram(s->id);
}

void
shader_create(struct shader *s, const char *fs_source, const char *vs_source) {
    GLuint id = glCreateProgram();
    s->id = id;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	check_compile_error(vs);


	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);

	check_compile_error(fs);

	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);

	//TODO check link error
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void 
shader_set_uniform(struct shader *s, int loc, enum UNIFORM_FORMAT format, const float *v) {
	switch(format) {
		case UNIFORM_FLOAT1:
			{
				glUniform1f(loc, v[0]);
				break;
			}
		case UNIFORM_FLOAT2:
			{
				glUniform2f(loc, v[0], v[1]);
				break;
			}
		case UNIFORM_FLOAT3:
			{
				glUniform3f(loc, v[0], v[1], v[2]);
				break;
			}
		case UNIFORM_FLOAT4:
			{
				glUniform4f(loc, v[0], v[1], v[2], v[3]);
				break;
			}
		case UNIFORM_FLOAT33:
			{
				glUniformMatrix3fv(loc, 1, GL_FALSE, v);
				break;
			}
		case UNIFORM_FLOAT44:
			{
				glUniformMatrix3fv(loc, 1, GL_FALSE, v);
				break;
			}
		default:
			assert(0);

	}
}


int 
shader_uniform_location(struct shader *s, const char *name) {
	int loc = glGetUniformLocation(s->id, name);
	return loc;
}
