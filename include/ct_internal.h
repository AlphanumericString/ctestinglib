#ifndef CT_INTERNAL_H
#define CT_INTERNAL_H

/*
 * Internal declarations of the ctesting library.
 *
 * This header is only meant to be included by implementation files after
 * defining CTESTS_INTERNAL (and CTESTS_INTERNAL_TESTS for the library's own
 * self-tests). It is never part of the public API.
 */
#if !defined(CTESTS_INTERNAL) && !defined(CTESTS_INTERNAL_TESTS)
#error "ct_internal.h requires CTESTS_INTERNAL or CTESTS_INTERNAL_TESTS"
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "ct_assert.h"
#include "ct_capture.h"
#include "ct_memory.h"
#include "ct_modules.h"
#include "ct_structs.h"

#ifdef CTESTS_INTERNAL

// module resolution
// resolves a ':' separated module path against root, the first name may be
// the root module itself. Prints a cli error to stderr and returns NULL when
// the path is empty, holds an empty segment, targets an unknown module or
// targets a test (single tests can not be run)
t_module *cti_module_resolve(t_module *root, const char *path);

// cmp / sorts
void sort_testslist(t_list *t_ls);
void sort_moduleslist(t_list *m_ls);

// collection
int	   cti_collect_sbm_t_success(const t_module *m);
int	   cti_collect_sbm_t_total(const t_module *m);
size_t cti_mget_longest_name(const t_module *m);

// display format hooks
typedef void (*t_display_hook)(const t_module *m, t_display_mode md, int depth);
typedef void (*t_display_sum_hook)(int total, int passed, t_display_mode md);
typedef void (*t_display_sbm_hook)(const t_list *sbm, t_display_mode md);
typedef void (*t_display_list)(t_module **m, size_t count, t_display_mode md);

typedef struct s_display_fmt
{
	t_display_hook	   first_open;
	t_display_hook	   open;
	t_display_hook	   tests;
	t_display_hook	   modules;
	t_display_hook	   summary;
	t_display_hook	   close;
	t_display_hook	   last_close;
	t_display_sum_hook sum_nb;
	t_display_sum_hook sum_percent;
	t_display_sbm_hook sbm_recap;
	t_display_list	   md_list_displayer;
	// legacy text output prints a module's submodules before its own tests.
	bool submodules_first;
} t_display_fmt;

extern const t_display_fmt g_cti_fmt_txt;
extern const t_display_fmt g_cti_fmt_json;
extern const t_display_fmt g_cti_fmt_html;

// main hooks
const t_display_fmt *cti_display_fmt_get(t_display_mode md);
void				 cti_display_run(const t_module *m, t_display_mode md);
void cti_display_module(const t_module *m, t_display_mode md, int depth);
void cti_selection_display(t_module **selection, int count,
						   t_display_mode mode);

// module summaries
void cti_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md);
void cti_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md);

// module display
void cti_mdisplay_module_summary(const t_module *m, t_display_mode md);
void cti_mdisplay_sbm_recap(const t_list *sbm, t_display_mode md);

// tests display
bool cti_test_succeeded(const t_test *t);
void cti_mdisplay_chain(const t_module *m);
void cti_mdisplay_test_name(const t_test *t, const t_module *m,
							t_display_mode md);

// run
int cti_test_call(const t_test *t);
int cti_tfork(t_test *t, t_module *m);

// utils
void  cti_list_push(t_list **lst, void *data);
void *cti_list_pop(t_list **lst);
void  cti_list_foreach(const t_list *list, void (*f)(void *data));

void cti_print_nb(int nb);
void cti_print_string(const char *str);
void cti_print_flt(float f);
void cti_print_charn(int depth, char c);

// memory
int64_t *cti_failloc_get(void);
size_t	*cti_alloc_get(void);
void	 cti_alloc_reset(void);

void *safecalloc(size_t nmemb, size_t size);
void *saferealloc(void *ptr, size_t size);

void push_to_cleanup(void *ptr);
bool is_in_cleanup(void *ptr);
bool remove_from_cleanup(void *ptr);
void do_cleanup(void (*f)(void *));

void cti_set_memory_cleanup(bool val);
bool cti_get_memory_cleanup(void);

// asserts
int ct_assert_false_v(bool condition, const char *err_msg, va_list args);
int ct_assert_true_v(bool condition, const char *err_msg, va_list args);

// captures
#define CT_RBUFFSIZE 4096

// State of a capture on a single file descriptor.
// The file descriptor table is changed by init and restored by release:
// - original_fd is a backup of the captured descriptor (-1 when unused)
// - source_fd is the descriptor being captured (-1 once released)
// - pipe_read reads what was written to source_fd
// - pipe_write only exists until the redirection is complete (-1 afterwards)
// - pending keeps the bytes read from the pipe until they are consumed, so
//   that peekn can inspect them without losing them
struct s_capture
{
	int	   original_fd;
	int	   source_fd;
	int	   pipe_read;
	int	   pipe_write;
	char  *pending;
	size_t pending_len;
	size_t pending_cap;
};

bool ct_capture_init(int source_fd, t_capture *capture);
bool ct_capture_release(t_capture *capture);
void cti_fflush_step(t_capture *c);

// deprecated - used by test-suite
void cti_display_results_txt(const t_module *m, t_display_mode md, int depth);
void cti_display_results_json(const t_module *m, t_display_mode md);

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
int ctit_module_display_html(void);

// /handling
int ctit_module_errors(void);

// /run
int ctit_module_hooks(void);

// capture
int ctit_capture(void);

// tests handling
// /base
int ctit_tadd(void);
int ctit_crash_catcher(void);
int ctit_tordering(void);
int ctit_return_check(void);
int ctit_timeout(void);
int ctit_tparam(void);

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

// memory
int ctit_cleanup(void);
int ctit_count(void);
int ctit_count_reset(void);
int ctit_fail(void);
int ctit_alloc_variants(void);
int ctit_free(void);

// asserts
int ctit_true(void);
int ctit_false(void);
int ctit_streq(void);
int ctit_strne(void);

// utils
int ctit_listpush(void);
int ctit_listpop(void);
int ctit_listforeach(void);

int ctit_print_cn(void);
int ctit_print_nb(void);
int ctit_print_flt(void);
int ctit_print_string(void);

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
int ctit_module_display_html(void);

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
int ctit_tparam(void);

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

#endif /* CT_INTERNAL_H */
