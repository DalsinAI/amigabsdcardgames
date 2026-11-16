/* readv/writev for Mille Bornes' saved games (mille/varpush.c): libnix has
 * neither, so the port's own (amiga/compat.c), one read or write a part.
 * Copyright (c) 2026 AmiCradle Team. BSD-3-Clause, like the games. */
#ifndef AMIGA_SYS_UIO_H
#define AMIGA_SYS_UIO_H
#include <sys/types.h>
#ifndef __iovec_defined
#define __iovec_defined 1
struct iovec {
	void *iov_base;
	size_t iov_len;
};
#endif
#define readv amiga_readv
#define writev amiga_writev
ssize_t amiga_readv(int fd, const struct iovec *v, int n);
ssize_t amiga_writev(int fd, const struct iovec *v, int n);
#endif
