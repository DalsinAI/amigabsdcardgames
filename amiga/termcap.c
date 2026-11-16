/* termcap with one entry: the console window of amiga/console_amiga.c,
 * 80 x 24, which reads ANSI sequences (console.device on the Amiga, the
 * terminal tests/host/vt.py reads in the host tests). Whatever TERM says,
 * this is the terminal. Backgammon asks it for li, co and the strings
 * below; tgoto ignores the cm it is given and writes ESC [ row ; col H.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <stdio.h>
#include <string.h>

#include "termcap.h"

char PC, *BC, *UP;

static const struct { const char *id, *s; } caps[] = {
	{ "cl", "\033[H\033[J" }, { "cd", "\033[J" }, { "ce", "\033[K" },
	{ "cm", "\033[%i%d;%dH" }, { "ho", "\033[H" }, { "up", "\033[A" },
	{ "nd", "\033[C" }, { "bc", "\b" },
};

int tgetent(char *buf, const char *name) { (void)name; if (buf) buf[0] = 0; return 1; }

int tgetnum(const char *id)
{
	if (!strcmp(id, "li")) return 24;
	if (!strcmp(id, "co")) return 80;
	return -1;
}

int tgetflag(const char *id) { (void)id; return 0; }

char *tgetstr(const char *id, char **area)
{
	for (unsigned i = 0; i < sizeof caps / sizeof caps[0]; i++)
		if (!strcmp(id, caps[i].id)) {
			char *s = *area;
			strcpy(s, caps[i].s);
			*area += strlen(s) + 1;
			return s;
		}
	return NULL;
}

char *tgoto(const char *cm, int col, int row)
{
	static char out[32];
	(void)cm;
	snprintf(out, sizeof out, "\033[%d;%dH", row + 1, col + 1);
	return out;
}

int tputs(const char *str, int affcnt, int (*outc)(int))
{
	(void)affcnt;
	if (!str) return -1;
	while (*str) outc((unsigned char)*str++);
	return 0;
}
