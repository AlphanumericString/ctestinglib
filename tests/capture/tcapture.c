/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcapture.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:20:00 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 15:20:00 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_capture.h"
#include "tc_memory.h"
#include "tc_modules.h"
#include "tc_utils.h"

#define BIG_READ	8192
#define MID_READ	5000

static int	init_errors(void)
{
	t_capture	cap;

	tc_memset(&cap, 0xFF, sizeof(cap));
	if (tc_capture_init(-1, &cap) != false)
		return (1);
	if (tc_capture_init(STDOUT_FILENO, NULL) != false)
		return (2);
	if (tc_capture_create(-1) != NULL)
		return (3);
	tc_capture_destroy(NULL);
	if (tc_capture_release(NULL) != false)
		return (4);
	if (tc_capture_get(NULL) != NULL)
		return (5);
	tc_memset(&cap, 0xFF, sizeof(cap));
	if (tc_capture_init(999999, &cap) != false)
		return (6);
	return (EXIT_SUCCESS);
}

static int	compare_errors(void)
{
	t_capture	*c;
	int			pf[2];
	t_capture	bad;
	char		*s;

	c = tc_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	if (tc_capture_compare(c, "") != -1)
		return (tc_capture_destroy(c), 2);
	if (tc_capture_compare(NULL, "x") != -1)
		return (tc_capture_destroy(c), 3);
	write(STDOUT_FILENO, "0123456789", 10);
	if (tc_capture_compare(c, "012") != -1)
		return (tc_capture_destroy(c), 4);
	tc_capture_destroy(c);
	if (pipe(pf) == -1)
		return (5);
	tc_memset(&bad, 0, sizeof(bad));
	bad.pipe_read = pf[0];
	close(pf[0]);
	close(pf[1]);
	s = tc_capture_get(&bad);
	if (s != NULL)
		return (safefree(s), 6);
	if (tc_capture_compare(&bad, "x") != -1)
		return (7);
	return (EXIT_SUCCESS);
}

static int	big_compare(void)
{
	t_capture	*c;
	char		*exp;
	char		*written;

	c = tc_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	exp = safealloc(BIG_READ + 1);
	written = safealloc(BIG_READ);
	if (!exp || !written)
		return (safefree(exp), safefree(written), tc_capture_destroy(c), 2);
	tc_memset(exp, 'a', BIG_READ);
	exp[BIG_READ] = 0;
	tc_memset(written, 'a', BIG_READ);
	write(STDOUT_FILENO, written, BIG_READ);
	if (tc_capture_compare(c, exp) != 0)
		return (safefree(exp), safefree(written), tc_capture_destroy(c), 3);
	return (safefree(exp), safefree(written), tc_capture_destroy(c),
		EXIT_SUCCESS);
}

static int	big_get(void)
{
	t_capture	*c;
	char		*in;
	char		*out;

	c = tc_capture_create(STDOUT_FILENO);
	if (!c)
		return (1);
	in = safealloc(MID_READ);
	if (!in)
		return (tc_capture_destroy(c), 2);
	tc_memset(in, 'b', MID_READ);
	write(STDOUT_FILENO, in, MID_READ);
	out = tc_capture_get(c);
	if (!out || tc_strlen(out) != MID_READ)
		return (safefree(in), safefree(out), tc_capture_destroy(c), 3);
	safefree(in);
	safefree(out);
	tc_capture_destroy(c);
	return (EXIT_SUCCESS);
}

static int	stderr_capture(void)
{
	t_capture	*c;
	char		*s;

	c = tc_capture_create(STDERR_FILENO);
	if (!c)
		return (1);
	write(STDERR_FILENO, "err", 3);
	if (tc_capture_compare(c, "err") != 0)
		return (tc_capture_destroy(c), 2);
	tc_capture_destroy(c);
	c = tc_capture_create(STDERR_FILENO);
	if (!c)
		return (3);
	write(STDERR_FILENO, "world", 5);
	s = tc_capture_get(c);
	if (!s || tc_memcmp(s, "world", 6) != 0)
		return (safefree(s), tc_capture_destroy(c), 4);
	return (safefree(s), tc_capture_destroy(c), EXIT_SUCCESS);
}

static int	release_paths(void)
{
	t_capture	cap;
	t_capture	bad;
	int			pf[2];
	t_capture	other;

	if (pipe(pf) == -1)
		return (1);
	tc_memset(&cap, 0, sizeof(cap));
	cap.original_fd = -1;
	cap.source_fd = -1;
	cap.pipe_read = -1;
	cap.pipe_write = pf[1];
	if (tc_capture_release(&cap) != true)
		return (close(pf[0]), 2);
	close(pf[0]);
	tc_memset(&bad, 0, sizeof(bad));
	bad.source_fd = -1;
	bad.original_fd = dup(STDOUT_FILENO);
	bad.pipe_read = -1;
	bad.pipe_write = -1;
	if (tc_capture_release(&bad) != false)
		return (3);
	tc_memset(&other, 0, sizeof(other));
	other.source_fd = 42;
	tci_fflush_step(&other);
	return (EXIT_SUCCESS);
}

int	tcit_capture(void)
{
	int	ret;

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
	ret = stderr_capture();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = release_paths();
	return (ret);
}
