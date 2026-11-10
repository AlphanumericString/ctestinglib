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

#ifdef CTESTS_INTERNAL

// cmp / sorts
void sort_testslist(t_list *t_ls);
void sort_moduleslist(t_list *m_ls);

// collection
int	   cti_collect_sbm_t_success(const t_module *m);
int	   cti_collect_sbm_t_total(const t_module *m);
size_t cti_mget_longest_name(const t_module *m);
// module resolution
// resolves a ':' separated module path against root, the first name may be
// the root module itself. Prints a cli error to stderr and returns NULL when
// the path is empty, holds an empty segment, targets an unknown module or
// targets a test (single tests can not be run)
t_module *cti_module_resolve(t_module *root, const char *path);
// module totals
void cti_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md);
void cti_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md);
void cti_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md);
// run
int cti_tfork(t_test *t, t_module *m);

// printing
void cti_mdisplay_tests(const t_module *m, t_display_mode md, int depth);
void cti_mdisplay_sbm(const t_list *sbm, t_display_mode md);
void cti_display_results_json(const t_module *m, t_display_mode md);
void cti_display_results_json_list(t_module **modules, size_t count,
								   t_display_mode md);
void cti_display_results_txt(const t_module *m, t_display_mode md, int depth);

// -- tests
void cti_td_name(const t_test *t, const t_module *m, t_display_mode md,
				 size_t *pass);

#endif /* CTESTS_INTERNAL */

#ifdef CTESTS_INTERNAL_TESTS

// tests
// modules handling
// /base
int ctit_add(void);
int ctit_nb_passed(void);
int ctit_nb_tests(void);
int ctit_ordering(void);
int ctit_run(void);
int ctit_init(void);
// /display
int ctit_module_display(void);
int ctit_module_display_extra(void);

// /handling
int ctit_module_errors(void);

// /run
int ctit_module_hooks(void);
int ctit_run_args(void);

// capture
int ctit_capture(void);

// tests handling
// /base
int ctit_tadd(void);
int ctit_crash_catcher(void);
int ctit_tordering(void);
int ctit_return_check(void);
int ctit_timeout(void);

// dummy - used to check all func are called
// dummy tests
int dummy_fail_test(void);
int dummy_alloc(void);
int dummy_leak(void);
int dummy_success_test(void);
int dummy_sigsegv(void);
int dummy_capture_stdin(void);
int dummy_timeout(void);
// dummy modules creators
t_module *create_quiet_dummy(void);
t_module *create_dummy_module(void);

// self tests
t_module *create_modules_tests(void);
t_module *create_tests_tests(void);
t_module *create_utils_tests(void);
t_module *create_alloc_tests(void);
t_module *create_assert_tests(void);
t_module *create_capture_tests(void);

#endif /* CTESTS_INTERNAL_TESTS */

// user facing functions
// add tests / modules
bool ct_module_tadd(t_module *m, const t_test *test);
bool ct_module_taddf(t_module *m, int (*f)(void), const char *s);
bool ct_module_taddf_timeout(t_module *m, int (*f)(void), const char *s,
							 unsigned int seconds);
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
