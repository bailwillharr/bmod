#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

extern bool logger_isTimeAvailable;

int log_init(const char *path);
void log_destroy();

void log_log(const char *format, ...);
void log_error(const char *format, ...);
void log_warn(const char *format, ...);

#endif
