/* Included before every file of the games (-include): what NetBSD has that
 * the Amiga's libnix names otherwise or not at all. amiga/compat.c.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#ifndef AMIGA_PRELUDE_H
#define AMIGA_PRELUDE_H
#define __RCSID(x)
#define __COPYRIGHT(x)
#define __SCCSID(x)
#ifndef __dead
#define __dead
#endif
/* one user, no set-group-ID games: renamed, so a system header's
 * declaration of them declares the port's (amiga/compat.c) */
#define setregid amiga_setregid
#define getlogin amiga_getlogin
#define _PATH_DEVNULL "NIL:"
/* Backgammon (-DAMIGA_TTY_GAME): reads and writes of fds 0-2 through the
 * window (amiga/tty.c); no alarm (the 10 s password is untimed); execl of
 * the other program (Teachgammon, Backgammon) runs it in its own window
 * and ends this one (amiga/compat_amiga.c) */
#ifdef AMIGA_TTY_GAME
#include <stddef.h>
#include <sys/types.h>
#define read  amiga_read
#define write amiga_write
#define alarm amiga_alarm
#define execl amiga_execl
ssize_t amiga_read(int fd, void *buf, size_t n);
ssize_t amiga_write(int fd, const void *buf, size_t n);
unsigned int amiga_alarm(unsigned int s);
int amiga_execl(const char *path, const char *arg0, ...);
#ifndef SIGALRM
#define SIGALRM 14
#endif
#endif
/* the instructions, a screen at a time; 0 when the file was there */
int amiga_page(const char *path);
#endif
