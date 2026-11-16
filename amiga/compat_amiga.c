/* What libnix has not got that the BSD card games call: err(3) and its
 * kin, and random(3). (The host's C library has them; this file is only in
 * the Amiga build.)
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "err.h"

static void report(int with_errno, const char *fmt, va_list ap)
{
	int e = errno;

	fputs("\n", stderr);
	if (fmt) vfprintf(stderr, fmt, ap);
	if (with_errno) fprintf(stderr, "%s%s", fmt ? ": " : "", strerror(e));
	fputs("\n", stderr);
}

void verr(int code, const char *fmt, va_list ap) { report(1, fmt, ap); exit(code); }
void verrx(int code, const char *fmt, va_list ap) { report(0, fmt, ap); exit(code); }
void vwarn(const char *fmt, va_list ap) { report(1, fmt, ap); }
void vwarnx(const char *fmt, va_list ap) { report(0, fmt, ap); }
void err(int code, const char *fmt, ...) { va_list ap; va_start(ap, fmt); verr(code, fmt, ap); }
void errx(int code, const char *fmt, ...) { va_list ap; va_start(ap, fmt); verrx(code, fmt, ap); }
void warn(const char *fmt, ...) { va_list ap; va_start(ap, fmt); vwarn(fmt, ap); va_end(ap); }
void warnx(const char *fmt, ...) { va_list ap; va_start(ap, fmt); vwarnx(fmt, ap); va_end(ap); }

/* random(3): xorshift32, enough for dealing cards */
static unsigned long seed = 2463534242UL;

void srandom(unsigned int s) { seed = s ? s : 2463534242UL; }

long random(void)
{
	seed ^= (seed << 13) & 0xffffffffUL;
	seed ^= seed >> 17;
	seed ^= (seed << 5) & 0xffffffffUL;
	return (long)(seed & 0x7fffffffUL);
}
