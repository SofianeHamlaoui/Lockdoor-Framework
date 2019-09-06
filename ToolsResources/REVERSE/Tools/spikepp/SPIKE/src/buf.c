/*
  buf.c

  Copyright (c) 2000 Dug Song <dugsong@monkey.org>

  Modifed 2001 Dave Aitel as was under BSD license

  $Id: buf.c,v 1.1.1.1 2004/01/15 00:26:13 dave Exp $
*/


#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <err.h>
#include "buf.h"

void
buf_init(buf_t buf, u_char *data, int len)
{
	buf->base = data;
	buf->end = buf->size = len;
	buf->offset = 0;
}

buf_t
buf_new(int size)
{
	buf_t buf;
	
	if ((buf = malloc(sizeof(*buf))) == NULL)
		return (NULL);
	
	if ((buf->base = malloc(size)) == NULL) {
		free(buf);
		return (NULL);
	}
	buf->size = size;
	buf->offset = 0;
	buf->end = 0;

	return (buf);
}

void
buf_free(buf_t buf)
{
	if (buf->base != NULL) {
		free(buf->base);
	}
	free(buf);
}

int
buf_seek(buf_t buf, int off, int whence)
{
	if ((whence == SEEK_SET) && (off <= buf->size)) {
		buf->offset = off;
	}
	else if ((whence == SEEK_CUR) && ((buf->offset + off) <= buf->size)) {
		buf->offset += off;
	}
	else return (-1);
	
	return (buf->offset);
}

int
buf_get(buf_t buf, void *dst, int len)
{
	u_char *p;
	int i;

	p = dst;
	
	if (buf_len(buf) < len) {
		return (-1);
	}
	for (i = 0; i < len; i++)
		p[i] = buf->base[buf->offset + i];
	
	buf->offset += len;
	
	return (len);
}

int
buf_put(buf_t buf, void *src, int len)
{
	if (buf->offset + len > buf->size) {
		return (-1);
	}
	memcpy(buf_ptr(buf), src, len);
	
	buf->offset += len;

	return (len);
}

int
buf_putf(buf_t buf, const char *fmt, ...)
{
	va_list ap;
	int i;

	va_start(ap, fmt);
	i = vsnprintf(buf_ptr(buf), buf_len(buf), fmt, ap);
	va_end(ap);

	buf_skip(buf, i);
	
	return (i);
}

void
buf_end(buf_t buf)
{
	buf->end = buf->offset;
	buf->offset = 0;
}

int
buf_index(buf_t buf, void *ptr, int len)
{
	u_char *p, *q;

	p = buf_ptr(buf);
	q = buf->base + buf->end;
	
	for (; q - p >= len; p++) {
		if (memcmp(p, ptr, len) == 0)
			return (p - buf_ptr(buf));
	}
	return (-1);
}

int
buf_rindex(buf_t buf, void *ptr, int len)
{
	u_char *p, *q;

	p = buf->base + buf->end - len;
	q = buf_ptr(buf);
	
	for (; p > q; p--) {
		if (memcmp(p, ptr, len) == 0)
			return (p - q);
	}
	return (-1);
}

int
buf_cmp(buf_t buf, void *ptr, int len)
{
	if (buf_len(buf) < len)
		return (-1);

	return (memcmp(buf_ptr(buf), ptr, len));
}

buf_t
buf_tok(buf_t buf, void *sep, int len)
{
	static struct buf *savebuf, tokbuf;
	int off;

	if (buf != NULL)
		savebuf = buf;

	if (sep == NULL && buf_len(savebuf) >= len) {
		tokbuf.base = buf_ptr(savebuf);
		tokbuf.offset = 0;
		tokbuf.size = tokbuf.end = len;
		buf_skip(savebuf, len);
	}
	else if ((off = buf_index(savebuf, sep, len)) != -1) {
		tokbuf.base = buf_ptr(savebuf);
		tokbuf.offset = 0;
		tokbuf.size = tokbuf.end = off;
		buf_skip(savebuf, off + len);
	}
	else if (buf_len(savebuf) > 0) {
		tokbuf.base = buf_ptr(savebuf);
		tokbuf.offset = 0;
		tokbuf.size = tokbuf.end = buf_len(savebuf);
		savebuf->offset = savebuf->end;
	}
	else return (NULL);
	
	return (&tokbuf);
}

buf_t
buf_getbuf(buf_t buf, int offset, int len)
{
	buf_t b;
	
	if (buf->offset + offset + len > buf->end)
		return (NULL);

	buf_skip(buf, offset);
	
	if ((b = buf_new(len)) != NULL) {
		buf_put(b, buf_ptr(buf), len);
		buf_end(b);
	}
	buf_skip(buf, len);
	
	return (b);
}

buf_t
buf_getword(buf_t buf, void *sep, int len)
{
	buf_t b;
	int off;
	
	if ((off = buf_index(buf, sep, len)) < 0)
		return (NULL);

	if ((b = buf_new(off)) != NULL) {
		buf_put(b, buf_ptr(buf), off);
		buf_end(b);
		buf_skip(buf, off + len);
	}
	return (b);
}

char *
buf_strdup(buf_t buf)
{
	char *p;
	int i;

	i = buf_len(buf);
	
	if ((p = malloc(i + 1)) == NULL)
		err(1, "malloc");

	memcpy(p, buf_ptr(buf), i);
	
	p[i] = '\0';
	
	return (p);
}

int
buf_isascii(buf_t buf)
{
	u_char *p, *q;

	p = buf_ptr(buf);
	q = buf->base + buf->end;
	
	for (; p < q; p++)
		if (!isascii(*p)) return (0);
	
	return (1);
}
