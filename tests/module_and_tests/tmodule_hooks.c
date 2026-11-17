#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

static int	g_setup;
static int	g_teardown;
static bool g_fixture;

static bool hook_setup(void)
{
	g_setup++;
	g_fixture = true;
	return (true);
}

static bool hook_setup_fail(void)
{
	g_setup++;
	return (false);
}

static bool hook_teardown(void)
{
	g_teardown++;
	g_fixture = false;
	return (true);
}

static bool hook_teardown_fail(void)
{
	g_teardown++;
	return (false);
}

// succeeds only when the parent module's setup ran before the test
static int test_fixture_present(void)
{
	if (g_fixture == true)
		return (EXIT_SUCCESS);
	return (42);
}

// succeeds only when no parent setup runs before the test
static int test_fixture_absent(void)
{
	if (g_fixture == false)
		return (EXIT_SUCCESS);
	return (43);
}

static void reset_hooks(void)
{
	g_setup = 0;
	g_teardown = 0;
	g_fixture = false;
}

static int check_setters(void)
{
	t_module *m;

	ct_module_init(&m, "hooks", "hooks test");
	if (!m)
		return (1);
	ct_module_set_setup(NULL, hook_setup);
	ct_module_set_teardown(NULL, hook_teardown);
	ct_module_set_hooks_wrap_submodules(NULL, true);
	if (m->setup_function != NULL || m->teardown_function != NULL ||
		ct_module_get_hooks_wrap_submodules(NULL) != false)
		return (ct_module_destroy(m), 2);
	ct_module_set_setup(m, hook_setup);
	ct_module_set_teardown(m, hook_teardown);
	if (m->setup_function != hook_setup ||
		m->teardown_function != hook_teardown ||
		ct_module_get_hooks_wrap_submodules(m) != false)
		return (ct_module_destroy(m), 3);
	ct_module_set_hooks_wrap_submodules(m, true);
	if (ct_module_get_hooks_wrap_submodules(m) != true)
		return (ct_module_destroy(m), 4);
	ct_module_set_hooks_wrap_submodules(m, false);
	if (ct_module_get_hooks_wrap_submodules(m) != false)
		return (ct_module_destroy(m), 5);
	ct_module_destroy(m);
	return (EXIT_SUCCESS);
}

static int check_normal_run(void)
{
	t_module *m;
	int		  ret;

	reset_hooks();
	ct_module_init(&m, "hooks", "hooks test");
	if (!m)
		return (1);
	ct_module_set_setup(m, hook_setup);
	ct_module_set_teardown(m, hook_teardown);
	ct_module_taddf(m, test_fixture_present, "present");
	ret = ct_module_run(m);
	if (ret != EXIT_SUCCESS || g_setup != 1 || g_teardown != 1 ||
		m->tests_passed != 1 || g_fixture != false)
		return (ct_module_destroy(m), 2);
	ct_module_destroy(m);
	return (EXIT_SUCCESS);
}

static int check_setup_failure(void)
{
	t_module *m;
	t_test	 *t;
	int		  ret;

	reset_hooks();
	ct_module_init(&m, "hooks", "hooks test");
	if (!m)
		return (1);
	ct_module_set_setup(m, hook_setup_fail);
	ct_module_set_teardown(m, hook_teardown);
	ct_module_taddf(m, test_fixture_present, "present");
	ret = ct_module_run(m);
	t = m->tests_list->data;
	if (ret != EXIT_FAILURE || g_setup != 1 || g_teardown != 1 ||
		m->tests_passed != 0 || t->return_value != 0)
		return (ct_module_destroy(m), 2);
	ct_module_destroy(m);
	return (EXIT_SUCCESS);
}

static int check_teardown_failure(void)
{
	t_module *m;
	int		  ret;

	reset_hooks();
	ct_module_init(&m, "hooks", "hooks test");
	if (!m)
		return (1);
	ct_module_set_setup(m, hook_setup);
	ct_module_set_teardown(m, hook_teardown_fail);
	ct_module_taddf(m, test_fixture_present, "present");
	ret = ct_module_run(m);
	if (ret != EXIT_FAILURE || g_setup != 1 || g_teardown != 1 ||
		m->tests_passed != 1)
		return (ct_module_destroy(m), 2);
	ct_module_destroy(m);
	return (EXIT_SUCCESS);
}

static int check_default_order(void)
{
	t_module *p;
	t_module *s;
	int		  ret;

	reset_hooks();
	ct_module_init(&p, "parent", "parent module");
	ct_module_init(&s, "sub", "submodule");
	if (!p || !s)
		return (ct_module_destroy(p), ct_module_destroy(s), 1);
	ct_module_set_setup(p, hook_setup);
	ct_module_set_teardown(p, hook_teardown);
	ct_module_taddf(p, test_fixture_present, "parent test");
	ct_module_taddf(s, test_fixture_absent, "sub test");
	ct_module_add_sub(p, s);
	ret = ct_module_run(p);
	if (ret != EXIT_SUCCESS || g_setup != 1 || g_teardown != 1 ||
		p->tests_passed != 1 || s->tests_passed != 1 || p->module_passed != 1 ||
		g_fixture != false)
		return (ct_module_destroy(p), 2);
	ct_module_destroy(p);
	return (EXIT_SUCCESS);
}

static int check_wrap_submodules(void)
{
	t_module *p;
	t_module *s;
	t_test	 *t;
	int		  ret;

	reset_hooks();
	ct_module_init(&p, "parent", "parent module");
	ct_module_init(&s, "sub", "submodule");
	if (!p || !s)
		return (ct_module_destroy(p), ct_module_destroy(s), 1);
	ct_module_set_setup(p, hook_setup);
	ct_module_set_teardown(p, hook_teardown);
	ct_module_set_hooks_wrap_submodules(p, true);
	ct_module_taddf(s, test_fixture_present, "sub test");
	ct_module_add_sub(p, s);
	ret = ct_module_run(p);
	t = s->tests_list->data;
	if (ret != EXIT_SUCCESS || g_setup != 1 || g_teardown != 1 ||
		s->tests_passed != 1 || p->module_passed != 1 ||
		t->return_value != EXIT_SUCCESS || g_fixture != false)
		return (ct_module_destroy(p), 2);
	ct_module_destroy(p);
	return (EXIT_SUCCESS);
}

static int check_wrap_setup_failure(void)
{
	t_module *p;
	t_module *s;
	t_test	 *pt;
	t_test	 *st;
	int		  ret;

	reset_hooks();
	ct_module_init(&p, "parent", "parent module");
	ct_module_init(&s, "sub", "submodule");
	if (!p || !s)
		return (ct_module_destroy(p), ct_module_destroy(s), 1);
	ct_module_set_setup(p, hook_setup_fail);
	ct_module_set_teardown(p, hook_teardown);
	ct_module_set_hooks_wrap_submodules(p, true);
	ct_module_taddf(p, test_fixture_present, "parent test");
	ct_module_taddf(s, test_fixture_present, "sub test");
	ct_module_add_sub(p, s);
	ret = ct_module_run(p);
	pt = p->tests_list->data;
	st = s->tests_list->data;
	if (ret != EXIT_FAILURE || g_setup != 1 || g_teardown != 1 ||
		p->tests_passed != 0 || s->tests_passed != 0 || p->module_passed != 0 ||
		pt->return_value != 0 || st->return_value != 0)
		return (ct_module_destroy(p), 2);
	ct_module_destroy(p);
	return (EXIT_SUCCESS);
}

int ctit_module_hooks(void)
{
	int ret;

	ret = check_setters();
	if (ret == EXIT_SUCCESS)
		ret = check_normal_run();
	if (ret == EXIT_SUCCESS)
		ret = check_setup_failure();
	if (ret == EXIT_SUCCESS)
		ret = check_teardown_failure();
	if (ret == EXIT_SUCCESS)
		ret = check_default_order();
	if (ret == EXIT_SUCCESS)
		ret = check_wrap_submodules();
	if (ret == EXIT_SUCCESS)
		ret = check_wrap_setup_failure();
	return (ret);
}
