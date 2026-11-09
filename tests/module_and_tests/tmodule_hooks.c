#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

static int	g_setup;
static int	g_teardown;

static void	hook_setup(void)
{
	g_setup++;
}

static void	hook_teardown(void)
{
	g_teardown++;
}

int	ctit_module_hooks(void)
{
	t_module	*m;

	g_setup = 0;
	g_teardown = 0;
	ct_module_init(&m, "hooks", "hooks test");
	if (!m)
		return (1);
	m->setup_function = hook_setup;
	m->teardown_function = hook_teardown;
	ct_module_taddf(m, dummy_success_test, "s1");
	ct_module_run(m);
	if (g_setup != 1 || g_teardown != 1)
		return (ct_module_destroy(m), 2);
	ct_module_destroy(m);
	return (EXIT_SUCCESS);
}
