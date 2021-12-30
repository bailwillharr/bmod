#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../logger.h"

#include "shader.h"

static GLuint compile(const char *path, GLenum type)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        log_log("Error opening shader file: %s\n", path);
		exit(EXIT_FAILURE);
    }

    // copy file into buffer
    fseek(fp, 0, SEEK_END);
    GLint len = (GLint)ftell(fp);
    char *src = calloc(1, len + 1);
    if (src == NULL) {
        log_log("Error allocating memory for shader src\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_SET);
    fread(src, 1, len, fp);
    fclose(fp);

    // compile shader
    GLuint handle = glCreateShader(type);
    glShaderSource(handle, 1, (const GLchar **)&src, (const GLint *)&len);
    glCompileShader(handle);
    free(src);

    // check for compilation error
    GLint compiled;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        GLint log_len;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_len);
        GLchar *log_msg = calloc(1, log_len);
        if (log_msg == NULL) {
            log_log("Error allocating memory for shader compilation error log\n");
            exit(EXIT_FAILURE);
        }
        glGetShaderInfoLog(handle, log_len, NULL, log_msg);
        log_log("Shader compilation error in %s\nLog:\n%s\n", path, log_msg);
        free(log_msg);
		exit(EXIT_FAILURE);
    }

    return handle;
}

GLuint shader_create(const char *name)
{
	char path_vs[128] = { 0 };
	char path_fs[128] = { 0 };
	snprintf(path_vs, 127, "shaders/%s.vert", name);
	snprintf(path_fs, 127, "shaders/%s.frag", name);

    GLuint vs = compile(path_vs, GL_VERTEX_SHADER);
    GLuint fs = compile(path_fs, GL_FRAGMENT_SHADER);

    GLuint handle = glCreateProgram();

    glAttachShader(handle, vs);
    glAttachShader(handle, fs);
    glLinkProgram(handle);
    glValidateProgram(handle);

    // flag shader objects for deletion, this does not take effect until the program is deleted
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint linked, validated;
    glGetProgramiv(handle, GL_LINK_STATUS, &linked);
    glGetProgramiv(handle, GL_VALIDATE_STATUS, &validated);
    if (linked == 0 || validated == 0) {
        GLint log_len;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_len);
        GLchar *log_msg = calloc(1, log_len);
        if (log_msg == NULL) {
            log_log("Error allocating memory for program linking error log\n");
            exit(EXIT_FAILURE);
        }
        glGetProgramInfoLog(handle, log_len, NULL, log_msg);
        log_log("Program linking error with %s and %s\nLog:\n%s\n", path_vs, path_fs, log_msg);
        free(log_msg);
        exit(EXIT_FAILURE);
    }

    return handle;
}

void shader_destroy(GLuint handle)
{
    glDeleteProgram(handle);
}
