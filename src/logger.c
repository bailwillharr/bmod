#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "logger.h"

static FILE *logger_out = NULL;

static void log_msg(const char * type, const char *format, va_list old_args)
{

	va_list args;

	if (logger_isTimeAvailable == true) {
		double time = glfwGetTime();
		fprintf(logger_out, "[%lf] [%s] ", time, type);
		fprintf(stderr, "[%lf] [%s] ", time, type);
	}

	if (logger_out != NULL) {
		va_copy(args, old_args);
		vfprintf(logger_out, format, args); // write to file
	}
	va_copy(args, old_args);
	vfprintf(stderr, format, args); // write to console

}



bool logger_isTimeAvailable = false;

void log_init(const char *path)
{
	if (logger_out != NULL) {
		fclose(logger_out);
	}
	logger_out = fopen(path, "w");
	if (logger_out == NULL) {
		fprintf(stderr, "unable to open log file for writing\n");
	}
}

void log_destroy()
{
	fclose(logger_out);
	logger_out = NULL;
}

void log_log(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_msg("INFO", format, args);
	va_end(args);
}

void log_error(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_msg("ERROR", format, args);
	va_end(args);
}

void log_warn(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_msg("WARN", format, args);
	va_end(args);
}
