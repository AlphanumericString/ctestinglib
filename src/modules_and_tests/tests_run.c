#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "sys/wait.h"
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

static t_module *climb_to_root(const t_module *module)
{
	const t_module *root;

	if (!module)
		return (NULL);
	root = module;
	while (root->parent)
		root = root->parent;
	return ((t_module *)root);
}

int cti_test_call(const t_test *t)
{
	if (t->type == CT_PARAM)
		return (t->test_func.param(t->param));
	return (t->test_func.pure());
}

static unsigned int resolve_timeout(const t_test *t, const t_module *m)
{
	if (t->timeout != 0)
		return (t->timeout);
	if (m->timeout != 0)
		return (m->timeout);
	return (CT_DEFAULT_TIMEOUT);
}

static void wait_with_timeout(t_test *t, unsigned int timeout)
{
	const struct timespec delay = {0, 1000000L};
	unsigned long		  elapsed;

	if (timeout == 0)
	{
		waitpid(t->pid, &t->return_value, 0);
		return;
	}
	elapsed = 0;
	while (elapsed < (unsigned long)timeout * 1000)
	{
		if (waitpid(t->pid, &t->return_value, WNOHANG) == t->pid)
			return;
		nanosleep(&delay, NULL);
		elapsed++;
	}
	t->timed_out = true;
	kill(t->pid, SIGKILL);
	waitpid(t->pid, &t->return_value, 0);
}

int cti_tfork(t_test *t, t_module *m)
{
	t->pid = fork();
	if (t->pid < 0)
		return (EXIT_FAILURE);
	else if (!t->pid)
	{
		t_test snapshot = *t;

		ct_module_destroy(climb_to_root(m));
		exit(cti_test_call(&snapshot));
	}
	wait_with_timeout(t, resolve_timeout(t, m));
	if (WIFEXITED(t->return_value))
		t->return_value = WEXITSTATUS(t->return_value);
	if (t->return_value == EXIT_SUCCESS)
		m->tests_passed++;
	return (EXIT_SUCCESS);
}
