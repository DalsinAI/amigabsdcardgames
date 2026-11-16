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
/* the instructions, a screen at a time; 0 when the file was there */
int amiga_page(const char *path);
#endif
