#include "string.h"
#include <stdarg.h>

const char *
sprintf_alloc(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int cap = 64;
	char *s = malloc(cap);
	while (1) {
		int written = vsnprintf(s, cap, fmt, args);
		if (written >= cap) {
			cap *= 2;
			s = realloc(s, cap);
		} else {
			break;
		}
	}
	va_end(args);
	return s;
}
