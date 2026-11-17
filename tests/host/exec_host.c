/* The host's execl for the host tests, for Backgammon and Teachgammon
 * starting each other (amiga/exec_amiga.c on the Amiga).
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>

int amiga_execl(const char *path, const char *arg0, ...)
{
	char *argv[8];
	int n = 0;
	va_list ap;
	argv[n++] = (char *)arg0;
	va_start(ap, arg0);
	while (n < 7 && (argv[n] = va_arg(ap, char *)) != NULL) n++;
	va_end(ap);
	argv[n] = NULL;
	return execv(path, argv);
}
