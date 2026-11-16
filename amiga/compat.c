/* What the BSD card games ask of NetBSD that the port answers itself, on
 * the Amiga and in the host tests alike (amiga/include/amiga_prelude.h):
 * the player's name and the instructions' pager.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curses.h"

/* nothing to give up: the games run as the one user there is */
int amiga_setregid(int rgid, int egid)
{
	(void)rgid; (void)egid;
	return 0;
}

/* the player is named by the USER variable, or "Player" */
char *amiga_getlogin(void)
{
	static char name[40];
	const char *u = getenv("USER");

	strncpy(name, u && *u ? u : "Player", sizeof name - 1);
	return name;
}

/* The instructions, 22 lines at a time: Space or Return for more, q to
 * stop. 0 when the file was there. */
int amiga_page(const char *path)
{
	FILE *f = fopen(path, "r");
	char line[256];
	int n = 0;

	if (!f) return -1;
	while (fgets(line, sizeof line, f)) {
		/* upstream's files are troff (.n) or plain: drop troff's requests */
		if (line[0] == '.' && line[1] != '\n') continue;
		fputs(line, stdout);
		if (++n == 22) {
			unsigned char c = 0;
			fputs("--More-- (Space: more, q: done)", stdout);
			fflush(stdout);
			if (acurses_read(0, &c, 1) != 1 || c == 'q' || c == 'Q') { putchar('\n'); break; }
			fputs("\r                                \r", stdout);
			n = 0;
		}
	}
	fflush(stdout);
	fclose(f);
	return 0;
}

/* readv/writev (amiga/include/sys/uio.h), for Mille Bornes' saved games */
#include <unistd.h>
#include "sys/uio.h"

ssize_t amiga_readv(int fd, const struct iovec *v, int n)
{
	ssize_t total = 0;

	for (int i = 0; i < n; i++) {
		ssize_t r = read(fd, v[i].iov_base, v[i].iov_len);
		if (r < 0) return -1;
		total += r;
		if ((size_t)r < v[i].iov_len) break;
	}
	return total;
}

ssize_t amiga_writev(int fd, const struct iovec *v, int n)
{
	ssize_t total = 0;

	for (int i = 0; i < n; i++) {
		ssize_t r = write(fd, v[i].iov_base, v[i].iov_len);
		if (r < 0) return -1;
		total += r;
	}
	return total;
}
