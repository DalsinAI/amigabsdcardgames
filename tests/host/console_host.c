/* The console under amiga/curses.c for the host tests: stdout is the
 * terminal (tests/host/vt.py reads it back), stdin the keys, decoded by
 * amiga/keys.c as on the Amiga (so CR comes as NL and Esc [ sequences are
 * dropped).
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <unistd.h>

#include "keys.h"

int acurses_console_open(int lines, int cols) { (void)lines; (void)cols; return 0; }
void acurses_console_mode(int raw) { (void)raw; }
void acurses_console_close(void) {}
int amiga_console_window(const char *title) { (void)title; return 0; }

static int key_next(void)
{
	unsigned char c;
	return read(0, &c, 1) == 1 ? c : -1;
}

int acurses_read(int fd, void *buf, unsigned int n)
{
	if (fd != 0 || n == 0) return (int)read(fd, buf, n);
	for (;;) {
		int c = amiga_key_decode(key_next);
		if (c < 0) return 0;
		if (c == 0) continue;
		*(unsigned char *)buf = (unsigned char)c;
		return 1;
	}
}
