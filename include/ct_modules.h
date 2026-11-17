#ifndef CT_MODULES_H
#define CT_MODULES_H

#include <stdbool.h>
#include <unistd.h>

#include "ct_structs.h"

/**
 * @file ct_modules.h
 * @brief Build, run and report a tree of test modules.
 *
 * A test binary creates a root module with ct_module_init(), attaches tests
 * with ct_module_taddf() and submodules with ct_module_add_sub(), then runs
 * the tree with ct_module_run() or ct_module_run_args(). The tree is released
 * with ct_module_destroy().
 */

#ifndef CT_DEFAULT_TIMEOUT
/// @brief Default timeout, in seconds, applied to every test when neither the
/// test nor its module defines one. 0 disables the timeout (plain waitpid).
#define CT_DEFAULT_TIMEOUT 0
#endif

// user facing functions
// add tests / modules
bool ct_module_tadd(t_module *m, const t_test *test);
bool ct_module_taddf(t_module *m, int (*f)(void), const char *s);
bool ct_module_taddf_timeout(t_module *m, int (*f)(void), const char *s,
							 unsigned int seconds);
// parametrized test: f is called with param when the test runs
bool ct_module_taddf_param(t_module *m, int (*f)(void *), void *param,
						   const char *s);
bool ct_module_taddf_param_timeout(t_module *m, int (*f)(void *), void *param,
								   const char *s, unsigned int seconds);
void ct_module_init(t_module **m_ptr, const char *name, const char *dsc);
bool ct_module_add_sub(t_module *m, t_module *sub_module);

// module fixtures
// The setup hook runs once before the module's tests, the teardown hook once
// after the last test (also when a test failed). Both return void, so they are
// meant to prepare/release a fixture, not to assert on the test results.
// Tests run in forked children, so the hooks run in the parent process before
// the tests are forked: a test starts from the state left by setup, but its
// own changes are never visible to teardown.
void ct_module_set_setup(t_module *module, void (*setup)(void));
void ct_module_set_teardown(t_module *module, void (*teardown)(void));

// timeouts
// per module default timeout (0: fallback on CT_DEFAULT_TIMEOUT)
void		 ct_module_set_timeout(t_module *module, unsigned int seconds);
unsigned int ct_module_get_timeout(const t_module *module);
// per test timeout (0: fallback on module then CT_DEFAULT_TIMEOUT)
void		 ct_test_set_timeout(t_test *test, unsigned int seconds);
unsigned int ct_test_get_timeout(const t_test *test);

// run tests / modules
int ct_module_run(t_module *module);
int ct_module_run_self_tests(t_module *module);

// cli entry point, see the definition for the supported arguments
int ct_module_run_args(t_module *root, int argc, char **argv,
					   t_display_mode mode);

// module utils
void ct_module_sort(t_module *root);
void ct_module_display(const t_module *module, t_display_mode mode);

// destroy module tree
void ct_module_destroy(t_module *m);

#endif /* CT_MODULES_H */
