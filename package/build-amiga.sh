#!/bin/sh
# The release archive and the GFOpenUp part, for Cradle's stove_build
# (target package):
#  1. upstream bsd-games, the commit package/bsdgames.lock names (fetched,
#     never kept in this repository), with amiga/patches;
#  2. the host tests (tests/host/test_bsd.sh), when the machine has a C
#     compiler and python3;
#  3. the games built by amiga/Makefile.amigaos;
#  4. the drawer, its icon and the Installer script beside it packed as
#     NAME-VERSION-68040.lha (tools/pack_dist.py), with its .sha256 and a
#     short .readme;
#  5. OUT/Parts/BSDCardGames, a GFOpenUp part: its Part file and the
#     drawer's files for SYS:Games/BSDCardGames, installed with
#       GFOpenUpTool INSTALL Parts/BSDCardGames
# Nothing is published from here.
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu

HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/.." && pwd)
. "$HERE/package.env"
. "$HERE/bsdgames.lock"
OUT=${OUT:-"$ROOT/build-amiga-package"}
mkdir -p "$OUT"
OUT=$(CDPATH= cd -- "$OUT" && pwd)
STOVE=${STOVE:-"$HOME/AmiCradle/stoves/os32-gcc16-open6"}
# outside the checkout: Cradle cleans it (git clean -ffdx) before every job
WORK=${WORK:-"$HOME/.cache/amigabsdcardgames"}
mkdir -p "$WORK"
cd "$ROOT"
t0=$(date +%s)
if [ -x "$STOVE/prefix/bin/m68k-amigaos-gcc" ]; then PREFIX="$STOVE/prefix"; else PREFIX="$STOVE"; fi

# 1. upstream at the locked commit, patched afresh every time
SRC="$WORK/bsd-games-git"
if [ "$(git -C "$SRC" rev-parse HEAD 2>/dev/null)" != "$BSD_COMMIT" ]; then
    rm -rf "$SRC"; mkdir -p "$SRC"
    git -C "$SRC" init -q
    git -C "$SRC" fetch -q --depth 1 "$BSD_URL" "$BSD_COMMIT"
    git -C "$SRC" checkout -q FETCH_HEAD
fi
BSD="$WORK/bsd-games-$BSD_VERSION"
rm -rf "$BSD"; mkdir -p "$BSD"
git -C "$SRC" archive HEAD | tar x -C "$BSD"
for p in "$ROOT"/amiga/patches/*.patch; do
    patch -s -d "$BSD" -p1 < "$p"
done

# 2. the host tests
if command -v cc >/dev/null 2>&1 && command -v python3 >/dev/null 2>&1; then
    BSD="$BSD" sh tests/host/test_bsd.sh
fi

# 3. the games
make -f amiga/Makefile.amigaos BSD="$BSD" STOVE="$STOVE" PREFIX="$PREFIX" -j4 $TARGET \
    > "$OUT/build-output.txt" 2>&1 || { tail -n 60 "$OUT/build-output.txt"; exit 1; }

# 4. the archive
python3 "$ROOT/tools/pack_dist.py" --dist "$DIST" --name "$NAME" --out "$OUT"

# 5. the GFOpenUp part
sh "$ROOT/package/write-part.sh" "$DIST/$NAME" "$OUT/Parts/$NAME" "$NAME" "$PORT_VERSION" \
    "BSD card games: Cribbage, Mille Bornes and Go Fish, each in its own console window"
echo "part: $(grep -c '^copy' "$OUT/Parts/$NAME/Part") files, $(du -sk "$OUT/Parts/$NAME" | cut -f1) KB"
ls -l "$DIST/$NAME"
echo "total: $(( $(date +%s) - t0 )) s"
