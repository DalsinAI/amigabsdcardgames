/* The part of curses that the BSD card games (Cribbage, Mille Bornes) use,
 * for the Amiga port: windows kept in memory, and a refresh that sends the
 * console only what changed, as ANSI (ECMA-48) sequences, which the Amiga's
 * console.device and the host tests' terminal both read. amiga/curses.c.
 *
 * The screen is LINES x COLS (24 x 80): the console window
 * amiga/console_amiga.c opens that size. stdscr is a variable (Cribbage and
 * Mille point it at their own windows for a while); curscr is what the
 * console shows.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#ifndef ACURSES_H
#define ACURSES_H

#include <stdio.h>
#include <stdarg.h>

#define TRUE  1
#define FALSE 0
#define ERR   (-1)
#define OK    0

/* curses' own bool (Mille Bornes uses it), unless C99's is there */
#if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
typedef char bool;
#endif
typedef unsigned short chtype;
#define A_STANDOUT 0x100
#define A_CHARTEXT 0xff

typedef struct acurses_win {
	int begy, begx;          /* where it is on the screen */
	int maxy, maxx;          /* its lines and columns */
	int cury, curx;
	chtype **line;           /* its lines; a subwindow's point into its parent's */
	chtype attr;             /* A_STANDOUT while standout() */
	int clear_next;          /* clearok: the next refresh redraws everything */
	int scroll_ok;
	int own;                 /* 1: line[] and its text are this window's */
} WINDOW;

extern WINDOW *stdscr, *curscr;
extern int LINES, COLS;

WINDOW *initscr(void);
int endwin(void);
int isendwin(void);
WINDOW *newwin(int lines, int cols, int y, int x);
WINDOW *subwin(WINDOW *parent, int lines, int cols, int y, int x);
int delwin(WINDOW *w);

int wmove(WINDOW *w, int y, int x);
int waddch(WINDOW *w, chtype c);
int waddstr(WINDOW *w, const char *s);
int waddnstr(WINDOW *w, const char *s, int n);
int wprintw(WINDOW *w, const char *fmt, ...);
int vwprintw(WINDOW *w, const char *fmt, va_list ap);
int mvwprintw(WINDOW *w, int y, int x, const char *fmt, ...);
int mvprintw(int y, int x, const char *fmt, ...);
int printw(const char *fmt, ...);
int werase(WINDOW *w);
int wclear(WINDOW *w);
int wclrtoeol(WINDOW *w);
int wclrtobot(WINDOW *w);
int wrefresh(WINDOW *w);
int wnoutrefresh(WINDOW *w);
int doupdate(void);
chtype winch(WINDOW *w);
int wstandout(WINDOW *w);
int wstandend(WINDOW *w);
int touchwin(WINDOW *w);
int clearok(WINDOW *w, int on);
int leaveok(WINDOW *w, int on);
int scrollok(WINDOW *w, int on);
int idlok(WINDOW *w, int on);
int keypad(WINDOW *w, int on);
int mvcur(int oldy, int oldx, int y, int x);
int wgetch(WINDOW *w);

int cbreak(void);
int nocbreak(void);
int noecho(void);
int echo(void);
int raw(void);
int noraw(void);
int nl(void);
int nonl(void);
int beep(void);
int flushinp(void);
char erasechar(void);
char killchar(void);
const char *unctrl(chtype c);
int baudrate(void);

#define crmode()           cbreak()
#define nocrmode()         nocbreak()
#define getyx(w, y, x)     ((y) = (w)->cury, (x) = (w)->curx)
#define getmaxyx(w, y, x)  ((y) = (w)->maxy, (x) = (w)->maxx)
#define getbegyx(w, y, x)  ((y) = (w)->begy, (x) = (w)->begx)

#define addch(c)           waddch(stdscr, (c))
#define addstr(s)          waddstr(stdscr, (s))
#define move(y, x)         wmove(stdscr, (y), (x))
#define clear()            wclear(stdscr)
#define erase()            werase(stdscr)
#define clrtoeol()         wclrtoeol(stdscr)
#define clrtobot()         wclrtobot(stdscr)
#define refresh()          wrefresh(stdscr)
#define inch()             winch(stdscr)
#define standout()         wstandout(stdscr)
#define standend()         wstandend(stdscr)
#define getch()            wgetch(stdscr)
#define mvaddch(y, x, c)   (wmove(stdscr, (y), (x)) == ERR ? ERR : waddch(stdscr, (c)))
#define mvaddstr(y, x, s)  (wmove(stdscr, (y), (x)) == ERR ? ERR : waddstr(stdscr, (s)))
#define mvwaddch(w, y, x, c)  (wmove((w), (y), (x)) == ERR ? ERR : waddch((w), (c)))
#define mvwaddstr(w, y, x, s) (wmove((w), (y), (x)) == ERR ? ERR : waddstr((w), (s)))
#define mvinch(y, x)       (wmove(stdscr, (y), (x)) == ERR ? (chtype)ERR : winch(stdscr))
#define mvwinch(w, y, x)   (wmove((w), (y), (x)) == ERR ? (chtype)ERR : winch(w))

/* What the console port adds: the window's input (stdin) read a byte at a
 * time, Return given as NL and the cursor keys' sequences dropped
 * (amiga/console_amiga.c on the Amiga, the plain read on the host). */
int acurses_read(int fd, void *buf, unsigned int n);

#endif
