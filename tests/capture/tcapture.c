#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

#define BIG_READ 8192
#define MID_READ 5000
#define STDIN_BIG_READ 100000

static int init_errors(void)
{
	t_capture cap;

	memset(&cap, 0xFF, sizeof(cap));
	if (ct_capture_init(-1, &cap) != false)
		return (1);
	if (ct_capture_init(STDOUT_FILENO, NULL) != false)
		return (2);
	if (ct_capture_create(-1) != NULL)
		return (3);
	// a failed create must not close the descriptors it never owned
	if (fcntl(STDIN_FILENO, F_GETFD) == -1)
		return (7);
	ct_capture_destroy(NULL);
	if (ct_capture_release(NULL) != false)
		return (4);
	if (ct_capture_get(NULL) != NULL)
		return (5);
	memset(&cap, 0xFF, sizeof(cap));
	if (ct_capture_init(999999, &cap) != false)
		return (6);
	return (EXIT_SUCCESS);
}

static int compare_errors(void)
{
	t_capture *c;
	int		   pf[2];
	t_capture  bad;
	char	  *s;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	// nothing was written: an empty expectation matches
	if (ct_capture_compare(c, "") != 0)
		return (ct_capture_destroy(c), 2);
	if (ct_capture_compare(NULL, "x") != -1)
		return (ct_capture_destroy(c), 3);
	write(STDOUT_FILENO, "0123456789", 10);
	if (ct_capture_compare(c, "012") != -1)
		return (ct_capture_destroy(c), 4);
	ct_capture_destroy(c);
	if (pipe(pf) == -1)
		return (5);
	memset(&bad, 0, sizeof(bad));
	bad.pipe_read = pf[0];
	close(pf[0]);
	close(pf[1]);
	s = ct_capture_get(&bad);
	if (s != NULL)
		return (safefree(s), 6);
	if (ct_capture_compare(&bad, "x") != -1)
		return (7);
	return (EXIT_SUCCESS);
}

static int big_compare(void)
{
	t_capture *c;
	char	  *exp;
	char	  *written;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	exp = safealloc(BIG_READ + 1);
	written = safealloc(BIG_READ);
	if (!exp || !written)
		return (safefree(exp), safefree(written), ct_capture_destroy(c), 2);
	memset(exp, 'a', BIG_READ);
	exp[BIG_READ] = 0;
	memset(written, 'a', BIG_READ);
	write(STDOUT_FILENO, written, BIG_READ);
	if (ct_capture_compare(c, exp) != 0)
		return (safefree(exp), safefree(written), ct_capture_destroy(c), 3);
	return (safefree(exp), safefree(written), ct_capture_destroy(c),
			EXIT_SUCCESS);
}

static int big_get(void)
{
	t_capture *c;
	char	  *in;
	char	  *out;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	in = safealloc(MID_READ);
	if (!in)
		return (ct_capture_destroy(c), 2);
	memset(in, 'b', MID_READ);
	write(STDOUT_FILENO, in, MID_READ);
	out = ct_capture_get(c);
	if (!out || strlen(out) != MID_READ)
		return (safefree(in), safefree(out), ct_capture_destroy(c), 3);
	safefree(in);
	safefree(out);
	ct_capture_destroy(c);
	return (EXIT_SUCCESS);
}

// an exact multiple of the read buffer used to make get wait forever
static int exact_buffer_get(void)
{
	t_capture *c;
	char	  *in;
	char	  *out;
	size_t	   size;

	size = CT_RBUFFSIZE * 2;
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	in = safealloc(size);
	if (!in)
		return (ct_capture_destroy(c), 2);
	memset(in, 'c', size);
	write(STDOUT_FILENO, in, size);
	out = ct_capture_get(c);
	if (!out || strlen(out) != size || memcmp(out, in, size) != 0)
		return (safefree(in), safefree(out), ct_capture_destroy(c), 3);
	return (safefree(in), safefree(out), ct_capture_destroy(c), EXIT_SUCCESS);
}

// expecting more than was written is a mismatch, never a wait and never a
// false success on the partial output
static int short_output(void)
{
	t_capture *c;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	write(STDOUT_FILENO, "abc", 3);
	if (ct_capture_compare(c, "abcdef") != -1)
		return (ct_capture_destroy(c), 2);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

// nothing written: get returns an allocated empty string and the capture can
// be reused afterwards
static int empty_output(void)
{
	t_capture *c;
	char	  *s;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	s = ct_capture_get(c);
	if (!s || s[0] != '\0')
		return (safefree(s), ct_capture_destroy(c), 2);
	safefree(s);
	if (ct_capture_compare(c, "") != 0)
		return (ct_capture_destroy(c), 3);
	write(STDOUT_FILENO, "later", 5);
	if (ct_capture_compare(c, "later") != 0)
		return (ct_capture_destroy(c), 4);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

// get must flush stdio buffers before reading the capture pipe
static int buffered_get(void)
{
	t_capture *c;
	char	  *s;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	printf("buffered");
	s = ct_capture_get(c);
	if (!s || strcmp(s, "buffered") != 0)
		return (safefree(s), ct_capture_destroy(c), 2);
	return (safefree(s), ct_capture_destroy(c), EXIT_SUCCESS);
}

// peekn inspects the output without consuming it: the same bytes can be
// peeked several times and are still returned by get afterwards.
static int peek_does_not_consume(void)
{
	t_capture *c;
	char	   buf[16];
	char	  *s;
	ssize_t	   n;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	write(STDOUT_FILENO, "hello", 5);
	n = ct_capture_peekn(c, sizeof(buf), buf);
	if (n != 5 || memcmp(buf, "hello", 5) != 0)
		return (ct_capture_destroy(c), 2);
	// a short peek only copies what fits
	n = ct_capture_peekn(c, 3, buf);
	if (n != 3 || memcmp(buf, "hel", 3) != 0)
		return (ct_capture_destroy(c), 3);
	// new output is appended to the pending data
	write(STDOUT_FILENO, " world", 6);
	n = ct_capture_peekn(c, sizeof(buf), buf);
	if (n != 11 || memcmp(buf, "hello world", 11) != 0)
		return (ct_capture_destroy(c), 4);
	s = ct_capture_get(c);
	if (!s || strcmp(s, "hello world") != 0)
		return (safefree(s), ct_capture_destroy(c), 5);
	safefree(s);
	// get consumed everything: nothing is left to peek
	if (ct_capture_peekn(c, sizeof(buf), buf) != 0)
		return (ct_capture_destroy(c), 6);
	// compare also consumes data that was already peeked
	write(STDOUT_FILENO, "bye", 3);
	n = ct_capture_peekn(c, sizeof(buf), buf);
	if (n != 3 || memcmp(buf, "bye", 3) != 0)
		return (ct_capture_destroy(c), 7);
	if (ct_capture_compare(c, "bye") != 0)
		return (ct_capture_destroy(c), 8);
	if (ct_capture_peekn(c, sizeof(buf), buf) != 0)
		return (ct_capture_destroy(c), 9);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

// peekn is binary safe: embedded NUL bytes are copied and counted, so output
// can be inspected as bytes instead of as a NUL terminated string.
static int peek_binary(void)
{
	static const char data[] = {'a', '\0', 'b', '\0', 'c'};
	t_capture		 *c;
	char			  buf[sizeof(data)];
	ssize_t			  n;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	write(STDOUT_FILENO, data, sizeof(data));
	n = ct_capture_peekn(c, sizeof(buf), buf);
	if (n != (ssize_t)sizeof(data) || memcmp(buf, data, sizeof(data)) != 0)
		return (ct_capture_destroy(c), 2);
	n = ct_capture_peekn(c, sizeof(buf), buf);
	if (n != (ssize_t)sizeof(data) || memcmp(buf, data, sizeof(data)) != 0)
		return (ct_capture_destroy(c), 3);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

static int peek_errors(void)
{
	t_capture *c;
	t_capture  bad;
	char	   buf[4];
	int		   pf[2];

	if (ct_capture_peekn(NULL, sizeof(buf), buf) != -1)
		return (1);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (2);
	if (ct_capture_peekn(c, sizeof(buf), NULL) != -1)
		return (ct_capture_destroy(c), 3);
	// a zero sized peek does not need a buffer
	if (ct_capture_peekn(c, 0, NULL) != 0)
		return (ct_capture_destroy(c), 4);
	ct_capture_destroy(c);
	if (pipe(pf) == -1)
		return (5);
	memset(&bad, 0, sizeof(bad));
	bad.pipe_read = pf[0];
	close(pf[0]);
	close(pf[1]);
	if (ct_capture_peekn(&bad, sizeof(buf), buf) != -1)
		return (6);
	// an input capture has nothing to peek
	c = ct_capture_stdin("x");
	if (!c)
		return (7);
	if (ct_capture_peekn(c, sizeof(buf), buf) != -1)
		return (ct_capture_destroy(c), 8);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

// peekn must flush stdio buffers before reading the capture pipe
static int peek_buffered(void)
{
	t_capture *c;
	char	   buf[16];
	ssize_t	   n;

	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	printf("buffered");
	n = ct_capture_peekn(c, sizeof(buf), buf);
	if (n != 8 || memcmp(buf, "buffered", 8) != 0)
		return (ct_capture_destroy(c), 2);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

// destroy must flush stdio while the descriptor is still captured: output
// that was never read must not leak to the descriptor restored afterwards.
// A second capture records whatever reaches the real descriptor.
static int destroy_discards_buffered(void)
{
	t_capture *outer;
	t_capture *inner;
	char	  *s;

	outer = ct_capture_create(STDOUT_FILENO);
	if (!outer)
		return (1);
	inner = ct_capture_create(STDOUT_FILENO);
	if (!inner)
		return (ct_capture_destroy(outer), 2);
	printf("discarded");
	ct_capture_destroy(inner);
	write(STDOUT_FILENO, "kept", 4);
	s = ct_capture_get(outer);
	if (!s)
		return (ct_capture_destroy(outer), 3);
	if (strcmp(s, "kept") != 0)
		return (safefree(s), ct_capture_destroy(outer), 4);
	return (safefree(s), ct_capture_destroy(outer), EXIT_SUCCESS);
}

static int stderr_capture(void)
{
	t_capture *c;
	char	  *s;

	c = ct_capture_create(STDERR_FILENO);
	if (!c)
		return (1);
	write(STDERR_FILENO, "err", 3);
	if (ct_capture_compare(c, "err") != 0)
		return (ct_capture_destroy(c), 2);
	ct_capture_destroy(c);
	c = ct_capture_create(STDERR_FILENO);
	if (!c)
		return (3);
	write(STDERR_FILENO, "world", 5);
	s = ct_capture_get(c);
	if (!s || memcmp(s, "world", 6) != 0)
		return (safefree(s), ct_capture_destroy(c), 4);
	return (safefree(s), ct_capture_destroy(c), EXIT_SUCCESS);
}

static int feed_errors(void)
{
	t_capture *c;

	if (ct_capture_stdin(NULL) != NULL)
		return (1);
	if (ct_capture_feed(NULL, "x") != -1)
		return (2);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (3);
	if (ct_capture_feed(c, "x") != -1)
		return (ct_capture_destroy(c), 4);
	if (ct_capture_feed(c, NULL) != -1)
		return (ct_capture_destroy(c), 5);
	ct_capture_destroy(c);
	return (EXIT_SUCCESS);
}

// stdin can be fed from a capture and reads reach EOF after the input, so a
// case never waits for input that will not come.
static int stdin_feed(void)
{
	t_capture *c;
	char	   buf[32];

	c = ct_capture_stdin("hello\n");
	if (!c)
		return (1);
	if (read(STDIN_FILENO, buf, sizeof(buf)) != 6 ||
		memcmp(buf, "hello\n", 6) != 0)
		return (ct_capture_destroy(c), 2);
	if (read(STDIN_FILENO, buf, sizeof(buf)) != 0)
		return (ct_capture_destroy(c), 3);
	// an input capture has nothing to read back
	if (ct_capture_get(c) != NULL)
		return (ct_capture_destroy(c), 4);
	if (ct_capture_compare(c, "") != -1)
		return (ct_capture_destroy(c), 5);
	ct_capture_destroy(c);
	if (fcntl(STDIN_FILENO, F_GETFD) == -1)
		return (6);

	// feeding an existing capture again discards the unread input of the
	// previous case
	c = ct_capture_create(STDIN_FILENO);
	if (!c)
		return (7);
	if (ct_capture_feed(c, "abc") != 0)
		return (ct_capture_destroy(c), 8);
	if (read(STDIN_FILENO, buf, sizeof(buf)) != 3 || memcmp(buf, "abc", 3) != 0)
		return (ct_capture_destroy(c), 9);
	if (ct_capture_feed(c, "defg") != 0)
		return (ct_capture_destroy(c), 10);
	if (read(STDIN_FILENO, buf, sizeof(buf)) != 4 ||
		memcmp(buf, "defg", 4) != 0)
		return (ct_capture_destroy(c), 11);
	if (read(STDIN_FILENO, buf, sizeof(buf)) != 0)
		return (ct_capture_destroy(c), 12);
	ct_capture_destroy(c);
	return (EXIT_SUCCESS);
}

// input larger than a pipe buffer must not block while it is being fed.
static int stdin_big(void)
{
	t_capture *c;
	char	  *input;
	char	   buf[CT_RBUFFSIZE];
	size_t	   total;
	ssize_t	   n;

	input = safealloc(STDIN_BIG_READ + 1);
	if (!input)
		return (1);
	memset(input, 'i', STDIN_BIG_READ);
	input[STDIN_BIG_READ] = '\0';
	c = ct_capture_stdin(input);
	if (!c)
		return (safefree(input), 2);
	total = 0;
	for (;;)
	{
		n = read(STDIN_FILENO, buf, sizeof(buf));
		if (n < 0)
			return (safefree(input), ct_capture_destroy(c), 3);
		if (n == 0)
			break;
		if (memcmp(buf, input + total, (size_t)n) != 0)
			return (safefree(input), ct_capture_destroy(c), 4);
		total += (size_t)n;
	}
	safefree(input);
	if (total != STDIN_BIG_READ)
		return (ct_capture_destroy(c), 5);
	return (ct_capture_destroy(c), EXIT_SUCCESS);
}

static int release_paths(void)
{
	t_capture cap;
	t_capture bad;
	int		  pf[2];
	t_capture other;

	if (pipe(pf) == -1)
		return (1);
	memset(&cap, 0, sizeof(cap));
	cap.original_fd = -1;
	cap.source_fd = -1;
	cap.pipe_read = -1;
	cap.pipe_write = pf[1];
	if (ct_capture_release(&cap) != true)
		return (close(pf[0]), 2);
	close(pf[0]);
	memset(&bad, 0, sizeof(bad));
	bad.source_fd = -1;
	bad.original_fd = dup(STDOUT_FILENO);
	bad.pipe_read = -1;
	bad.pipe_write = -1;
	if (ct_capture_release(&bad) != false)
		return (3);
	memset(&other, 0, sizeof(other));
	other.source_fd = 42;
	cti_fflush_step(&other);
	return (EXIT_SUCCESS);
}

int ctit_capture(void)
{
	int ret;

	ret = init_errors();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = compare_errors();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = big_compare();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = big_get();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = exact_buffer_get();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = short_output();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = empty_output();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = buffered_get();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = peek_buffered();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = peek_does_not_consume();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = peek_binary();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = peek_errors();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = destroy_discards_buffered();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = stderr_capture();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = feed_errors();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = stdin_feed();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = stdin_big();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = release_paths();
	return (ret);
}
