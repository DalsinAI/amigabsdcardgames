/* Go Fish's start on the Amiga: its own console window (from a Shell as
 * from Workbench), then upstream's main, built as fish_main. Go Fish reads
 * whole lines, so the window stays in its normal (line) mode.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
int amiga_console_window(const char *title);
int fish_main(int argc, char **argv);

int main(int argc, char **argv)
{
	if (amiga_console_window("Go Fish")) return 20;
	return fish_main(argc, argv);
}
