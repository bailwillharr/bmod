#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#include "../glad/glad.h"

GLuint shader_create(const char *name);
void shader_destroy(GLuint handle);

#endif