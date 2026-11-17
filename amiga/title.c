/* A game's console window title, set before the window opens: GFOpenCurses
 * titles the window acurses_title (the program's name otherwise). Built once
 * a game, with its title: -DGAME_TITLE='"Mille Bornes"'.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#include "curses.h"

__attribute__((constructor)) static void game_title(void)
{
	acurses_title = GAME_TITLE;
}
