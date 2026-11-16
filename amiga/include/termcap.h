/* termcap for Backgammon: one terminal, the console window, which reads
 * ANSI (ECMA-48) sequences. amiga/termcap.c.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#ifndef AMIGA_TERMCAP_H
#define AMIGA_TERMCAP_H
extern char PC, *BC, *UP;
extern short ospeed;
int tgetent(char *buf, const char *name);
int tgetnum(const char *id);
int tgetflag(const char *id);
char *tgetstr(const char *id, char **area);
char *tgoto(const char *cm, int col, int row);
int tputs(const char *str, int affcnt, int (*outc)(int));
#endif
