/* The start on the Amiga of a game that is not curses (Go Fish,
 * Backgammon, Teachgammon): its own console window (from a Shell as from
 * Workbench), then upstream's main, built as GAME_MAIN. The window stays
 * in line mode unless the game sets a tty mode (amiga/tty.c).
 *   -DGAME_MAIN=fish_main -DGAME_TITLE='"Go Fish"'
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
int amiga_console_window(const char *title);
int GAME_MAIN(int argc, char **argv);

int main(int argc, char **argv)
{
	if (amiga_console_window(GAME_TITLE)) return 20;
	return GAME_MAIN(argc, argv);
}
