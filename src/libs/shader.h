
#ifndef  SHADER_INC
#define  SHADER_INC

struct shader {
	unsigned int id;
};

enum UNIFORM_FORMAT {
	UNIFORM_INVALID = 0,
	UNIFORM_INT1,
	UNIFORM_INT2,
	UNIFORM_INT3,
	UNIFORM_INT4,
	UNIFORM_FLOAT1,
	UNIFORM_FLOAT2,
	UNIFORM_FLOAT3,
	UNIFORM_FLOAT4,
	UNIFORM_FLOAT33,
	UNIFORM_FLOAT44,
};

void shader_create(struct shader *s, const char *fs, const char *vs);
void shader_create_with_file(struct shader *s, const char *fs_file, const char *vs_file);
void shader_use(const struct shader *s);
void shader_set_uniform(struct shader *s, const char *field, enum UNIFORM_FORMAT format, const float *v);
void shader_source(char *filename, char *buf, int sz);


#endif   /* ----- #ifndef SHADER_INC  ----- */
