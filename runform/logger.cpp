#include <cstdarg>
#include <stdio.h>
#include "runform.h"

void Logger::logf(char *format, ...) {
va_list args;
int n;
va_start (args, format);
vfprintf(stderr, format, args);
va_end (args);
}

