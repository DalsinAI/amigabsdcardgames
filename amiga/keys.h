/* The Amiga console's keys as the BSD card games read them (amiga/keys.c).
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#ifndef AMIGA_KEYS_H
#define AMIGA_KEYS_H
/* The next key from NEXT (a byte, or -1 at the end): -1 at the end of the
 * input, 0 for a sequence with no key in it, else the key as a Unix tty in
 * cbreak mode gives it. */
int amiga_key_decode(int (*next)(void));
#endif
