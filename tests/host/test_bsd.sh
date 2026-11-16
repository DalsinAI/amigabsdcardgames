#!/bin/sh
# Host tests of the port's console layer, on this machine: amiga/curses.c,
# amiga/keys.c and amiga/compat.c built with the three games (upstream with
# amiga/patches applied) over a plain terminal (tests/host/console_host.c),
# run with scripted keys; tests/host/vt.py reads back the screen curses
# drew and checks it:
#  - keys: CR is NL, Esc [ A (a cursor key) and CSI sequences are dropped,
#    Esc alone stays Esc;
#  - Cribbage asks for instructions, takes N, and asks Long or Short in its
#    message window (a subwindow of stdscr), with the score board drawn;
#  - Cribbage's instructions come through the port's pager;
#  - Mille Bornes draws its board, score card and command list in three
#    windows; the window toggle (w) and redraw (^L) keep it whole;
#  - Mille Bornes saves a game through the port's writev (s, a file name);
#  - Go Fish asks for instructions and deals (stdio, no curses);
#  - Backgammon (termcap and tty modes, amiga/termcap.c and amiga/tty.c)
#    asks its questions, draws the board with the checkers where they
#    start, and the computer moves; asking for the rules starts
#    Teachgammon (execl), which shows its first page.
# Needs a C compiler and python3; skipped without a compiler.
#   BSD=/path/to/bsd-games (patched) sh tests/host/test_bsd.sh
# Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
BSD=${BSD:?BSD=upstream source}
CC=${HOSTCC:-cc}
command -v "$CC" >/dev/null 2>&1 || { echo "host tests: skipped (no C compiler)"; exit 0; }
T=$(mktemp -d "${TMPDIR:-/tmp}/bsd-host.XXXXXX")
[ -n "${BSD_TEST_KEEP:-}" ] || trap 'rm -rf "$T"' EXIT INT TERM; echo "work: $T"
cd "$ROOT"

PERM=$($CC -fpermissive -x c -E /dev/null >/dev/null 2>&1 && echo -fpermissive || true)
UP="-std=gnu89 $PERM -w -D_XOPEN_SOURCE=500 -D__amigaos__ -include amiga/include/amiga_prelude.h -Iamiga/include -Iamiga"
MINE="-std=gnu99 -Wall -Werror -D_XOPEN_SOURCE=600 -Iamiga/include -Iamiga"
for f in curses keys compat; do $CC $MINE -c -o "$T/$f.o" "amiga/$f.c"; done
$CC $MINE -DGAME_MAIN=fish_main -DGAME_TITLE='"Go Fish"' -c -o "$T/start_fish.o" amiga/game_start.c
$CC $MINE -DGAME_MAIN=bg_main -DGAME_TITLE='"Backgammon"' -c -o "$T/start_bg.o" amiga/game_start.c
$CC $MINE -DGAME_MAIN=teach_main -DGAME_TITLE='"Teachgammon"' -c -o "$T/start_teach.o" amiga/game_start.c
$CC $MINE -c -o "$T/con.o" tests/host/console_host.c
for f in tty termcap ospeed; do $CC $MINE -D_DEFAULT_SOURCE -c -o "$T/$f.o" "amiga/$f.c"; done
PORT="$T/curses.o $T/keys.o $T/compat.o $T/con.o"
# the games' files where the host run finds them (on the Amiga: PROGDIR:)
mkdir -p "$T/inc/cribbage" "$T/inc/fish" "$T/Data"
cp "$BSD/cribbage/cribbage.n" "$BSD/fish/fish.instr" "$T/Data/"
sed "s|PROGDIR:|$T/|" amiga/cribbage/pathnames.h > "$T/inc/cribbage/pathnames.h"
sed "s|PROGDIR:|$T/|" amiga/fish/pathnames.h > "$T/inc/fish/pathnames.h"
for f in cards crib extern instr io score support; do
    $CC $UP -I"$T/inc/cribbage" -I"$BSD/cribbage" -c -o "$T/c_$f.o" "$BSD/cribbage/$f.c"
done
for f in comp end extern init mille misc move print roll save types varpush; do
    $CC $UP -I"$BSD/mille" -c -o "$T/m_$f.o" "$BSD/mille/$f.c"
done
$CC $UP -I"$T/inc/fish" -Dmain=fish_main -c -o "$T/fish.o" "$BSD/fish/fish.c"
BG="-DAMIGA_TTY_GAME -D_DEFAULT_SOURCE -DV7 -I$BSD/backgammon/common_source -DEXEC_PATH=\"$T/backgammon\" -DTEACH_PATH=\"$T/teachgammon\""
for f in allow board check fancy init odds one save subs table; do
    $CC $UP $BG -c -o "$T/bc_$f.o" "$BSD/backgammon/common_source/$f.c"
done
for f in extra main move text version; do
    $CC $UP $BG -Dmain=bg_main -c -o "$T/bg_$f.o" "$BSD/backgammon/backgammon/$f.c"
done
for f in data teach ttext1 ttext2 tutor; do
    $CC $UP $BG -Dmain=teach_main -c -o "$T/bt_$f.o" "$BSD/backgammon/teachgammon/$f.c"
done
# shellcheck disable=SC2086
$CC -o "$T/cribbage" "$T"/c_*.o $PORT
# shellcheck disable=SC2086
$CC -o "$T/mille" "$T"/m_*.o $PORT
# shellcheck disable=SC2086
$CC -o "$T/fish" "$T/fish.o" "$T/start_fish.o" $PORT
TTY="$T/tty.o $T/termcap.o"
# shellcheck disable=SC2086
$CC -o "$T/backgammon" "$T"/bg_*.o "$T"/bc_*.o "$T/start_bg.o" $PORT $TTY
# shellcheck disable=SC2086
$CC -o "$T/teachgammon" "$T"/bt_*.o "$T"/bc_*.o "$T/start_teach.o" "$T/ospeed.o" $PORT $TTY

# the keys, through amiga/keys.c alone
cat > "$T/keys_test.c" <<'C'
#include <stdio.h>
#include "keys.h"
static const unsigned char *p; static int n;
static int next(void) { return n-- > 0 ? *p++ : -1; }
int main(void)
{
	static const unsigned char in[] = { 'a', '\r', 0x1b, '[', 'A', 0x9b, '1', '~', 0x1b, 'x', 'z' };
	int want[] = { 'a', '\n', 0, 0, 0x1b, 'x', 'z', -1 }, bad = 0;
	p = in; n = sizeof in;
	for (int i = 0; i < 8; i++) {
		int c = amiga_key_decode(next);
		if (c != want[i]) { printf("key %d: got %d, want %d\n", i, c, want[i]); bad = 1; }
	}
	return bad;
}
C
fails=0
$CC $MINE -o "$T/keys_test" "$T/keys_test.c" "$T/keys.o"
if "$T/keys_test"; then echo "ok   keys: CR, cursor keys, CSI, Esc"; else echo "FAIL keys"; fails=$((fails + 1)); fi

play() {	# game, keys (printf format), output; the game's exit is not checked (it ends on the input's end)
    (cd "$T" && printf "$2" | timeout 20 "./$1" > "$3" 2>&1) || true
}
want() {	# name, output, texts...
    name=$1; out=$2; shift 2
    n=$#
    for t in "$@"; do set -- "$@" --want "$t"; done
    shift "$n"
    if python3 "$HERE/vt.py" "$out" "$@" > "$T/vt.txt"; then
        echo "ok   $name"
    else
        echo "FAIL $name"; cat "$T/vt.txt"; fails=$((fails + 1))
    fi
}

play cribbage 'n' "$T/crib1.out"
want "cribbage: instructions asked, N taken, Long or Short asked" "$T/crib1.out" \
    "Do you need instructions for cribbage? N" "Long (to 121) or Short (to 61)?"
play cribbage 'y ' "$T/crib2.out"
if grep -q "CRIBBAGE\|cribbage" "$T/crib2.out" && grep -q "More" "$T/crib2.out"; then
    echo "ok   cribbage: the instructions page through the port's pager"
else
    echo "FAIL cribbage: no instructions"; fails=$((fails + 1))
fi
play mille '' "$T/mille1.out"
want "mille: board, score card and commands in three windows" "$T/mille1.out" \
    "--HAND--" "--DECK--" "Milestones Played" "Overall Total" "p: pick" "--MILEAGE--"
play mille 'w' "$T/mille2.out"
want "mille: w swaps the command window, the board stays" "$T/mille2.out" "--HAND--" "Milestones Played" "--MILEAGE--"
play mille '\014' "$T/mille4.out"
want "mille: ^L redraws it whole" "$T/mille4.out" "--HAND--" "Milestones Played" "p: pick"
play mille 'ssaved.mb\n' "$T/mille3.out"
if [ -s "$T/saved.mb" ]; then echo "ok   mille: a game saves (writev)"; else echo "FAIL mille: no saved game"; fails=$((fails + 1)); fi
play fish 'n\n' "$T/fish1.out"
if grep -q "Would you like instructions" "$T/fish1.out" && grep -q "Your hand is" "$T/fish1.out"; then
    echo "ok   fish: asks, deals, shows the hand"
else
    echo "FAIL fish"; cat "$T/fish1.out" | head -20; fails=$((fails + 1))
fi
play backgammon 'nnnr' "$T/bg1.out"
want "backgammon: questions, the board as it starts, a first move" "$T/bg1.out" \
    "|13  14  15  16  17  18 |   |19  20  21  22  23  24 |" \
    "|12  11  10   9   8   7 |   | 6   5   4   3   2   1 |" "Player is red, computer is white." "rolls"
play backgammon 'y' "$T/bg2.out"
want "backgammon: the rules start Teachgammon" "$T/bg2.out" \
    "Do you want the rules of the game?  Yes." "how to play Backgammon" "-->"

if [ "$fails" -ne 0 ]; then echo "host tests: $fails failed"; exit 1; fi
echo "host tests: all passed"
