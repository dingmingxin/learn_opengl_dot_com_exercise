
#ifndef  SHADER_INC
#define  SHADER_INC

struct shader {
	unsigned int id;
};

enum UNIFORM_FORMAT {
	UNIFORM_INVALID = 0,
	UNIFORM_FLOAT1,
	UNIFORM_FLOAT2,
	UNIFORM_FLOAT3,
	UNIFORM_FLOAT4,
	UNIFORM_FLOAT33,
	UNIFORM_FLOAT44,
};

void shader_create(struct shader *s, const char *fs, const char *vs);
void shader_use(const struct shader *s);
void shader_uniform_property(struct shader *s, int loc, enum UNIFORM_FORMAT format, const float *v);
int shader_uniform_location(struct shader *s, const char *name);


#endif   /* ----- #ifndef SHADER_INC  ----- */
