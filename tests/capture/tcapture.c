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
	ret = destroy_discards_buffered();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = stderr_capture();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = release_paths();
	return (ret);
}
