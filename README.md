# BSD Card Games for GFOS

The AmiCradle Team's port of four games from bsd-games 2.17 (NetBSD's
games): Cribbage, Mille Bornes, Go Fish and Backgammon, to GFOS: AmigaOS
3.2 with GFOpenUp, 68040/68060. Each game runs in its own 80 x 24 console
window: no SDL, no ACGame and no GFOpenGPU. It ships as a release archive
with an Installer and as the GFOpenUp part Parts/BSDCardGames.

Upstream is fetched at build time (package/bsdgames.lock) and patched
(amiga/patches); this repository holds only the Amiga port. See
[AMIGA_PORT.md](AMIGA_PORT.md).

BSD-3-Clause, like the games. Copyright (c) 2026 AmiCradle Team.
