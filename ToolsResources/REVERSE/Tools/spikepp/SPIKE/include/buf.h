/*
  buf.h

  Buffer manipulation routines.

  Copyright (c) 2000 Dug Song <dugsong@monkey.org>

  $Id: buf.h,v 1.1.1.1 2004/01/15 00:26:48 dave Exp $
*/

#ifndef BUF_H
#define BUF_H

typedef struct buf {
	u_char	*base;
	int	 size;
	int	 offset;
	int	 end;
} *buf_t;

/* Initialize buffer. */
void	buf_init(buf_t buf, u_char *data, int len);

/* Allocate buffer. */
buf_t	buf_new(int size);

/* Free buffer. */
void	buf_free(buf_t buf);

/* Return pointer to buffer data. */
#define buf_ptr(b)	((b)->base + (b)->offset)

/* Return length of buffer data. */
#define buf_len(b)	((b)->end - (b)->offset)

/* Return remaining length of unused buffer. */
#define buf_left(b)	((b)->size - (b)->offset)

/* Return size of buffer. */
#define buf_size(b)	((b)->size)

/* Return offset in buffer. */
#define buf_tell(b)	((b)->offset)

/* Reposition buffer offset. */
int	buf_seek(buf_t buf, int offset, int whence);
#define buf_skip(b, l)	buf_seek(b, l, SEEK_CUR)
#define buf_rewind(b)	buf_seek(b, 0, SEEK_SET)

/* Read from buffer. */
int	buf_get(buf_t buf, void *dst, int len);

/* Write to buffer. */
int	buf_put(buf_t buf, void *src, int len);
int	buf_putf(buf_t buf, const char *fmt, ...);

/* Finalize buffer. */
void	buf_end(buf_t buf);

/* Locate byte string in buffer. */
int	buf_index(buf_t buf, void *ptr, int len);
int	buf_rindex(buf_t buf, void *ptr, int len);

/* Compare buffer to byte string. */
int	buf_cmp(buf_t buf, void *ptr, int len);

/* Tokenize buffer, like strtok(3). */
buf_t	buf_tok(buf_t buf, void *sep, int len);

/* Get a sub-buffer from buffer. */
buf_t	buf_getbuf(buf_t buf, int offset, int len);

/* Get a word from buffer. */
buf_t	buf_getword(buf_t buf, void *sep, int len);

/* Convert buffer to allocated string. */
char *	buf_strdup(buf_t buf);

/* ASCII string test. */
int	buf_isascii(buf_t buf);

#endif /* BUF_H */
