#ifndef GETLINE_H
#define GETLINE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"

#define DO_DEBUG 0

#if DO_DEBUG
#define DEBUG(x) (x)
#else
#define DEBUG(x) ((void)0)
#endif

/**
 * struct fd - holds an open file descriptor buffer
 * @fd: the integer file descriptor
 * @buf: pointer to the char buffer
 * @i: current index in the buf
 * @len: current length of the buf
 * @next: next node in linked list
 */
typedef struct fd
{
	int fd;
	char *buf;
	size_t i;
	size_t len;
	struct fd *next;
} FdBuf;

#define READ_SIZE 1024

char *__getline(const int fd);
char *__read_buf(FdBuf *fb);
FdBuf *get_fdbuf(FdBuf *head, const int fd);
char *__strchr(char *s, char c, ssize_t size);

#endif
