/* err(3) for the Amiga port (amiga/compat_amiga.c; the host has its own). */
#ifndef AMIGA_ERR_H
#define AMIGA_ERR_H
#include <stdarg.h>
void err(int code, const char *fmt, ...) __attribute__((__noreturn__));
void errx(int code, const char *fmt, ...) __attribute__((__noreturn__));
void warn(const char *fmt, ...);
void warnx(const char *fmt, ...);
void verr(int code, const char *fmt, va_list ap) __attribute__((__noreturn__));
void verrx(int code, const char *fmt, va_list ap) __attribute__((__noreturn__));
void vwarn(const char *fmt, va_list ap);
void vwarnx(const char *fmt, va_list ap);
#endif
