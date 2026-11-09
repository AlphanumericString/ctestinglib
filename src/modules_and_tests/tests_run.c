#include "sys/wait.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CTESTS_INTERNAL
#include "ct_modules.h"

static t_module	*climb_to_root(const t_module *module)
{
	const t_module	*root;

	if (!module)
		return (NULL);
	root = module;
	while (root->parent)
		root = root->parent;
	return ((t_module *)root);
}

#if (defined (FORK_TESTS) && (FORK_TESTS == 1))

int	cti_tfork(t_test *t, t_module *m)
{
	int	(*f)(void);

	t->pid = fork();
	if (t->pid < 0)
		return (EXIT_FAILURE);
	else if (!t->pid)
	{
		f = t->test_func;
		ct_module_destroy(climb_to_root(m));
		exit(f());
	}
	waitpid(t->pid, &t->return_value, 0);
	if (WIFEXITED(t->return_value))
		t->return_value = WEXITSTATUS(t->return_value);
	if (t->return_value == EXIT_SUCCESS)
		m->tests_passed++;
	return (EXIT_SUCCESS);
}

#else

// defining symbol to avoid compilation issues
int	fork_tests(t_test *t, t_module *m)
{
	(void)t;
	return (EXIT_FAILURE);
}

#endif
