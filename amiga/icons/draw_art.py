#!/usr/bin/env python3
"""The three games' pictures for their GlowIcons (art/*.png), drawn here:
bsd-games has no pictures of its own. 64 x 64, RGBA, solid or see-through.
    python3 amiga/icons/draw_art.py amiga/icons/art
Pure Python. MIT, Copyright (c) 2026 AmiCradle Team."""
import struct, sys, zlib
from pathlib import Path

W = H = 64
INK = (16, 18, 22, 255)


class Pic:
    def __init__(self):
        self.px = [[(0, 0, 0, 0)] * W for _ in range(H)]

    def rect(self, x0, y0, x1, y1, c):
        for y in range(max(0, y0), min(H, y1)):
            for x in range(max(0, x0), min(W, x1)):
                self.px[y][x] = c

    def box(self, x0, y0, x1, y1, fill, edge=INK):
        self.rect(x0, y0, x1, y1, edge)
        self.rect(x0 + 1, y0 + 1, x1 - 1, y1 - 1, fill)

    def disc(self, cx, cy, r, c):
        for y in range(H):
            for x in range(W):
                if (x - cx) ** 2 + (y - cy) ** 2 <= r * r:
                    self.px[y][x] = c

    def ellipse(self, cx, cy, rx, ry, c):
        for y in range(H):
            for x in range(W):
                if ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1:
                    self.px[y][x] = c

    def tri(self, pts, c):
        (ax, ay), (bx, by), (qx, qy) = pts
        def side(px, py, x0, y0, x1, y1):
            return (px - x1) * (y0 - y1) - (x0 - x1) * (py - y1)
        for y in range(H):
            for x in range(W):
                d1 = side(x, y, ax, ay, bx, by); d2 = side(x, y, bx, by, qx, qy); d3 = side(x, y, qx, qy, ax, ay)
                if not ((d1 < 0 or d2 < 0 or d3 < 0) and (d1 > 0 or d2 > 0 or d3 > 0)):
                    self.px[y][x] = c

    def png(self, path):
        raw = b"".join(b"\0" + bytes(v for p in row for v in p) for row in self.px)
        def chunk(t, d):
            return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xffffffff)
        Path(path).write_bytes(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 6, 0, 0, 0))
                               + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


WHITE, RED, BLACK = (250, 250, 246, 255), (200, 30, 36, 255), (30, 30, 34, 255)


def card(p, x, y, pip):
    p.box(x, y, x + 22, y + 30, WHITE)
    pip(p, x + 11, y + 15)


def heart(p, cx, cy):
    p.disc(cx - 3, cy - 2, 3, RED); p.disc(cx + 3, cy - 2, 3, RED); p.tri([(cx - 6, cy - 1), (cx + 6, cy - 1), (cx, cy + 6)], RED)


def spade(p, cx, cy):
    p.disc(cx - 3, cy + 1, 3, BLACK); p.disc(cx + 3, cy + 1, 3, BLACK); p.tri([(cx - 6, cy), (cx + 6, cy), (cx, cy - 7)], BLACK)
    p.rect(cx - 1, cy + 2, cx + 1, cy + 7, BLACK)


def cribbage():
    p = Pic()
    wood, dark = (176, 112, 52, 255), (120, 72, 30, 255)
    p.box(4, 36, 60, 56, wood)
    for row, col in ((40, (220, 40, 40, 255)), (46, (40, 90, 210, 255))):
        for i in range(9):
            p.rect(8 + i * 6, row, 10 + i * 6, row + 2, dark)
    p.rect(26, 38, 28, 42, (220, 40, 40, 255)); p.rect(38, 44, 40, 48, (40, 90, 210, 255))
    card(p, 10, 6, heart); card(p, 30, 4, spade)
    return p


def mille():
    p = Pic()
    stone, top = (236, 232, 220, 255), (220, 40, 36, 255)
    p.rect(14, 52, 50, 58, (110, 110, 104, 255))
    p.box(18, 14, 46, 54, stone)
    p.ellipse(32, 16, 14, 10, INK); p.ellipse(32, 16, 13, 9, top); p.rect(19, 16, 45, 26, stone)
    p.rect(19, 25, 45, 26, INK)
    for i, x in enumerate((23, 29, 37)):  # 100 (the "1" and two "0"s)
        if i == 0:
            p.rect(x + 1, 32, x + 3, 44, INK)
        else:
            p.box(x - 1, 32, x + 5, 44, stone)
    return p


def fish():
    p = Pic()
    blue, fin = (40, 130, 210, 255), (24, 90, 170, 255)
    card(p, 34, 4, heart)
    p.ellipse(28, 42, 20, 11, INK); p.ellipse(28, 42, 19, 10, blue)
    p.tri([(44, 42), (60, 30), (60, 54)], INK); p.tri([(46, 42), (59, 32), (59, 52)], fin)
    p.disc(17, 39, 3, WHITE); p.disc(17, 39, 1, INK)
    p.tri([(24, 33), (34, 26), (36, 34)], fin)
    return p


if __name__ == "__main__":
    out = Path(sys.argv[1] if len(sys.argv) > 1 else "art")
    out.mkdir(parents=True, exist_ok=True)
    for name, f in (("cribbage", cribbage), ("millebornes", mille), ("gofish", fish)):
        f().png(out / f"{name}.png")
