#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"


static bool
check_compile_error(GLuint s, const char *type)  {
	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(s, 512, NULL, infoLog);
		printf("ERROR: %s shader: compile failed:%s\n", type, infoLog);
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
shader_create_with_file(struct shader *s, const char *fs_file, const char *vs_file) {
	int sz = 1024;
	char fragmentShaderSource[1024]="";
	char vertexShaderSource[1024]="";
	shader_source(fs_file, fragmentShaderSource, sz);
	shader_source(vs_file, vertexShaderSource, sz);
	shader_create(s, fragmentShaderSource, vertexShaderSource);
}

void
shader_create(struct shader *s, const char *fs_source, const char *vs_source) {
    GLuint id = glCreateProgram();
    s->id = id;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
//	GLint vs_len = strlen(vs_source);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	check_compile_error(vs, "vs");


	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
//	GLint fs_len = strlen(fs_source);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);

	check_compile_error(fs, "fs");

	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);

	//TODO check link error
    glDeleteShader(vs);
    glDeleteShader(fs);
}

static int 
shader_uniform_location(struct shader *s, const char *name) {
	int loc = glGetUniformLocation(s->id, name);
	return loc;
}

void 
shader_set_uniform(struct shader *s, const char *field, enum UNIFORM_FORMAT format, const float *v) {
	int loc = glGetUniformLocation(s->id, field);
	if (format > UNIFORM_INVALID && format <= UNIFORM_INT4) {
		switch(format) {
			case UNIFORM_INT1:
				glUniform1i(loc, v[0]);
				break;
			case UNIFORM_INT2:
				glUniform2i(loc, v[0], v[1]);
				break;
			case UNIFORM_INT3:
				glUniform3i(loc, v[0], v[1], v[2]);
				break;
			case UNIFORM_INT4:
				glUniform4i(loc, v[0], v[1], v[2], v[3]);
				break;
			default:
				assert(0);
		}
	} else {
		switch(format) {
			case UNIFORM_FLOAT1:
				glUniform1f(loc, v[0]);
				break;
			case UNIFORM_FLOAT2:
				glUniform2f(loc, v[0], v[1]);
				break;
			case UNIFORM_FLOAT3:
				glUniform3f(loc, v[0], v[1], v[2]);
				break;
			case UNIFORM_FLOAT4:
				glUniform4f(loc, v[0], v[1], v[2], v[3]);
				break;
			case UNIFORM_FLOAT33:
				glUniformMatrix3fv(loc, 1, GL_FALSE, v);
				break;
			case UNIFORM_FLOAT44:
				glUniformMatrix4fv(loc, 1, GL_FALSE, v);
				break;
			default:
				assert(0);
		}
	}
}



void 
shader_source(const char *name, char *buf, int sz) {
	FILE *fp = NULL;
#ifdef BIN_PATH
	char fname[1024] = "";
	strcat(fname, BIN_PATH);
	strcat(fname, name);
	fp = fopen(fname, "r");
#else
	fp = fopen(name, "r");
#endif 
	if (!fp) {
		printf("open file error\n");
		return;
	}
	fread(buf, sizeof *buf, sz, fp);
	fclose(fp);
}
