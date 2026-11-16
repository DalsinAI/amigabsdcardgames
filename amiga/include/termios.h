/* The one thing of termios the BSD card games use: CTRL() (^L redraws). */
#ifndef AMIGA_TERMIOS_H
#define AMIGA_TERMIOS_H
#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif
#endif
