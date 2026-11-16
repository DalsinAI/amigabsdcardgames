/* execl() for Backgammon and Teachgammon, which start each other (the
 * lessons from the game, the game after the lessons): this window closes,
 * the other program runs in its own window (dos.library SystemTags), and
 * this one ends with its return code when it is done, as an exec would
 * have ended it. "PROGDIR:Name" is the drawer this program is in.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dos/dos.h>
#include <dos/dostags.h>
#include <proto/dos.h>

void acurses_console_close(void);

int amiga_execl(const char *path, const char *arg0, ...)
{
	char cmd[512];
	size_t len;
	const char *a;
	va_list ap;
	LONG rc;

	cmd[0] = '"';
	cmd[1] = 0;
	if (!strncmp(path, "PROGDIR:", 8)) {
		if (!NameFromLock(GetProgramDir(), (STRPTR)cmd + 1, sizeof cmd - 64)) return -1;
		AddPart((STRPTR)cmd + 1, (CONST_STRPTR)path + 8, sizeof cmd - 64);
	} else {
		strncat(cmd, path, sizeof cmd - 64);
	}
	strcat(cmd, "\"");
	(void)arg0;
	va_start(ap, arg0);
	while ((a = va_arg(ap, const char *)) != NULL) {
		len = strlen(cmd);
		if (len + strlen(a) + 4 >= sizeof cmd) break;
		snprintf(cmd + len, sizeof cmd - len, " \"%s\"", a);
	}
	va_end(ap);

	fflush(stdout);
	acurses_console_close();
	rc = SystemTags((CONST_STRPTR)cmd, SYS_Input, Open((CONST_STRPTR)"NIL:", MODE_OLDFILE),
	                SYS_Output, Open((CONST_STRPTR)"NIL:", MODE_NEWFILE), TAG_DONE);
	exit(rc < 0 ? 20 : (int)rc);
}
