/* The Amiga console's keys as the BSD card games read them (amiga/keys.h).
 * In raw mode (SetMode 1) the console gives Return as CR, which a Unix tty
 * gives as NL; the cursor keys, function keys, Help and window events as
 * CSI ($9B, or Esc [) sequences ending in a letter, '~' or '|': these are
 * read to their end and dropped (the games use letters). Esc alone is
 * given as Esc; the key after it is kept for the next call. No AmigaOS
 * calls here, so tests/host/test_keys.c runs it on the build machine.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include "keys.h"

#define CSI 0x9b
#define ESC 0x1b

static int held = -2; /* a byte read after Esc, for the next call; -2: none */

int amiga_key_decode(int (*next)(void))
{
	int c, n;

	if (held != -2) {
		c = held;
		held = -2;
	} else {
		c = next();
	}
	if (c == '\r') return '\n';
	if (c != CSI && c != ESC) return c;
	if (c == ESC) {
		n = next();
		if (n != '[') {
			held = n;
			return n == -1 ? (held = -2, ESC) : ESC;
		}
	}
	do {
		n = next();
	} while (n != -1 && !((n >= '@' && n <= '~') || n == '|'));
	return n == -1 ? -1 : 0;
}
