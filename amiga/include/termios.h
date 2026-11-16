/* What of termios the BSD games use: CTRL() (^L redraws), and for
 * Backgammon the tty modes it switches between (line mode, line mode
 * without echo, a key at a time). amiga/tty.c maps them onto the console
 * window's modes: a key at a time is raw mode (no echo either), the rest
 * line mode (the console echoes; it has no line mode without echo).
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#ifndef AMIGA_TERMIOS_H
#define AMIGA_TERMIOS_H
#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

typedef unsigned int tcflag_t;
typedef unsigned char cc_t;
typedef unsigned int speed_t;
#define NCCS 20
#define VERASE 3
#define VKILL  5
struct termios {
	tcflag_t c_iflag, c_oflag, c_cflag, c_lflag;
	cc_t c_cc[NCCS];
	speed_t c_ispeed, c_ospeed;
};
#define ECHO    0x0008
#define ICANON  0x0100
#define ONLCR   0x0002
#define OXTABS  0x0004
#define TCSANOW   0
#define TCSADRAIN 1
#define TCSAFLUSH 2
#define B9600   9600

#define tcgetattr   amiga_tcgetattr
#define tcsetattr   amiga_tcsetattr
#define cfgetospeed amiga_cfgetospeed
int amiga_tcgetattr(int fd, struct termios *t);
int amiga_tcsetattr(int fd, int when, const struct termios *t);
speed_t amiga_cfgetospeed(const struct termios *t);
#endif
