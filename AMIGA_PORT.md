BSD Card Games 2.17 for GFOS
============================

Four games from bsd-games, each its own program in the
BSDCardGames drawer and each in its own console window.

  Cribbage     the two-player pegging game against the computer, to 121
               (long) or 61 (short); it scores your hands with you
  MilleBornes  the French road race card game against the computer
  GoFish       ask the computer for ranks and make books of four
  Backgammon   the dice and doubling game against the computer (or watch
               it play itself)
  Teachgammon  Backgammon's rules and a practice game, a lesson at a time

Cribbage asks for cards by name (AH is the ace of hearts, 10S the ten of
spades). Mille Bornes lists its keys in the window (P picks, D discards,
U uses a card, O sorts your hand, R redraws, W changes the window's lists, S saves the game,
Q quits). Go Fish asks you to type a rank. Backgammon takes moves as
"from-to" or "from/pips" (8-5,6-5); R redraws, Q quits, S saves. Each game offers its
instructions when it starts; Return or Space pages through them.
The close gadget ends a game.

What it needs
-------------
GFOS (AmigaOS 3.2 with GFOpenUp) on a 68040 or 68060; the FPU is used
when there is one, and not needed. The games run in console windows
on any screen: no SDL, no ACGame, no GFOpenGPU.

Cribbage keeps its log in Cribbage.log beside itself; an update leaves it.
Mille Bornes and Backgammon save games where you name them. Asking
Backgammon for its rules starts Teachgammon in its own window, which
starts Backgammon again when the lessons end.

How it was ported
-----------------
The games are built from upstream as they are. amiga/curses.c is the part
of curses Cribbage and Mille Bornes use: their windows are kept in memory
and a refresh sends the console only what changed, as ANSI sequences,
which console.device reads. amiga/console_amiga.c opens the window, puts
it in raw mode for the curses games and decodes the keys (amiga/keys.c:
Return as NL, cursor keys dropped). Backgammon draws with
termcap and sets tty modes: amiga/termcap.c is one termcap entry, the
console window's ANSI, and amiga/tty.c makes its modes the window's (a key
at a time is raw mode; its LF-only output is the console's CSI 20 l).
amiga/compat*.c and amiga/include/amiga_prelude.h answer what NetBSD has and libnix has not
(err/warn, random, getlogin, readv/writev). The one patch,
amiga/patches/0001, reads keys through the port, pages the instructions
through amiga_page and leaves out Cribbage's close-on-exec call.

Tests: tests/host/test_bsd.sh builds the five programs on the build machine
over a plain terminal and plays them with scripted keys, reading back the
screen curses drew (the instructions, Long or Short, Mille Bornes' board
and its redraws, a saved game, Go Fish's deal, Backgammon's board and
first move, its rules starting Teachgammon), and checks the key decoder.

Credits
-------
bsd-games 2.17: Cribbage, Mille Bornes, Go Fish and Backgammon
(c) The Regents of the University of California, BSD licence (COPYING.txt).
Amiga port: Designed, tested and delivered by The AmiCradle Team.
Copyright (c) 2026 AmiCradle Team, BSD-3-Clause like the games.
