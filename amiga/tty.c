/* The tty modes Backgammon sets (amiga/include/termios.h), as the console
 * window's modes: without ICANON a key at a time (raw), else line mode.
 * Also its reads and writes: fd 0 is the window's keys (acurses_read: CR
 * comes as NL, cursor keys dropped), fds 1 and 2 go through stdio, which
 * console_amiga.c opened on the window; other fds are files.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "termios.h"

void acurses_console_mode(int raw);
int acurses_read(int fd, void *buf, unsigned int n);

/* the window's line editing: Backspace erases, ^U (as on Unix) kills the line */
static struct termios now = { 0, ONLCR, 0, ECHO | ICANON,
	{ [VERASE] = '\b', [VKILL] = CTRL('U') }, B9600, B9600 };

int amiga_tcgetattr(int fd, struct termios *t)
{
	(void)fd;
	*t = now;
	return 0;
}

int amiga_tcsetattr(int fd, int when, const struct termios *t)
{
	(void)fd; (void)when;
	/* ONLCR: the console's LF mode (CSI 20 h: LF returns the carriage too,
	 * its default; CSI 20 l: LF only goes down, as Backgammon's board
	 * drawing wants) */
	if ((t->c_oflag & ONLCR) != (now.c_oflag & ONLCR))
		fputs(t->c_oflag & ONLCR ? "\033[20h" : "\033[20l", stdout);
	fflush(stdout);
	now = *t;
	acurses_console_mode(!(t->c_lflag & ICANON));
	return 0;
}

speed_t amiga_cfgetospeed(const struct termios *t) { return t->c_ospeed; }

ssize_t amiga_read(int fd, void *buf, size_t n)
{
	if (fd == 0) { fflush(stdout); return acurses_read(0, buf, (unsigned)n); }
	return read(fd, buf, n);
}

ssize_t amiga_write(int fd, const void *buf, size_t n)
{
	if (fd == 1 || fd == 2) {
		FILE *f = fd == 1 ? stdout : stderr;
		size_t w = fwrite(buf, 1, n, f);
		fflush(f);
		return (ssize_t)w;
	}
	return write(fd, buf, n);
}

unsigned int amiga_alarm(unsigned int s) { (void)s; return 0; }
