#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "board/board.h"

#ifndef S_IFCHR
#define S_IFCHR 0020000
#endif

extern char _end;
extern char _stack_bottom;

static char *heap_end;

void *_sbrk(ptrdiff_t incr)
{
    if (heap_end == 0) {
        heap_end = &_end;
    }

    char *prev = heap_end;
    char *next = heap_end + incr;

    if (next >= &_stack_bottom) {
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end = next;
    return prev;
}

int _write(int fd, const void *buf, size_t count)
{
    (void)fd;
    const char *p = (const char *)buf;
    for (size_t i = 0; i < count; ++i) {
        board_putc(p[i]);
    }
    return (int)count;
}

int _read(int fd, void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;
    errno = EAGAIN;
    return -1;
}

int _close(int fd)
{
    (void)fd;
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    (void)fd;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    (void)fd;
    return 1;
}

off_t _lseek(int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    return 0;
}

void _exit(int status)
{
    (void)status;
    while (1) {
        __asm__ volatile("wfi");
    }
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}
