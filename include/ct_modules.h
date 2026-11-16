#ifndef CT_MODULES_H
#define CT_MODULES_H

#include <stdbool.h>
#include <unistd.h>

#include "ct_structs.h"

#ifndef FORK_TESTS
#define FORK_TESTS 1
#endif

// default timeout (in seconds) applied to every test when neither the test
// nor its module defines one. 0 disables the timeout (plain waitpid).
#ifndef CT_DEFAULT_TIMEOUT
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

// run the modules selected from the cli:
// - module paths are given either as positional arguments ("m1 m2 ...") or
//   as '--run-module=m1 --run-module=m2' flags, both forms can be mixed
// - a path is a ':' separated chain of module names resolved from root,
//   the first name may be the root module itself
// - every selected module runs with its own submodules, then the selection
//   is sorted and reported (json mode wraps several modules in an array)
// - overlapping selections are deduplicated: the broadest module wins
// - single tests can not be selected, only whole modules
// - when no module is provided root and its submodules are run
// - '-h' or '--help' prints the usage on stdout and returns EXIT_SUCCESS
//   without running anything (help wins over any other argument)
int ct_module_run_args(t_module *root, int argc, char **argv,
					   t_display_mode mode);

// module utils
void ct_module_sort(t_module *root);
void ct_module_display(const t_module *module, t_display_mode mode);

// destroy module tree
void ct_module_destroy(t_module *m);

#endif /* CT_MODULES_H */
