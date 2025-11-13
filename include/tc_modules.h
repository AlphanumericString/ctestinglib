/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_modules.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:07:54 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 17:44:49 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_MODULES_H
# define TC_MODULES_H

# include <stdbool.h>
# include <unistd.h>

# include "tc_structs.h"

# ifndef FORK_TESTS
#  define FORK_TESTS 1
# endif

# ifdef CTESTS_INTERNAL

// cmp / sorts
void		sort_testslist(t_list *t_ls);
void		sort_moduleslist(t_list *m_ls);

// collection
int			tci_collect_sbm_t_success(const t_module *m);
int			tci_collect_sbm_t_total(const t_module *m);
size_t		tci_mget_longest_name(const t_module *m);
// module totals
void		tci_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md);
void		tci_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md);
void		tci_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md);
// run
int			tci_tfork(t_test *t, t_module *m);

// printing
void		tci_mdisplay_tests(const t_module *m, t_display_mode md, int depth);
void		tci_mdisplay_sbm(const t_list *sbm, t_display_mode md);
void		tci_display_results_json(const t_module *m, t_display_mode md);
void		tci_display_results_txt(const t_module *m, t_display_mode md,
				int depth);

// -- tests
void		tci_td_name(const t_test *t, const t_module *m, t_display_mode md,
				size_t *pass);

# endif /* CTESTS_INTERNAL */

# ifdef CTESTS_INTERNAL_TESTS

// tests functions for modules
int			tcit_add(void);
int			tcit_nb_passed(void);
int			tcit_nb_tests(void);
int			tcit_ordering(void);
int			tcit_run(void);
int			tcit_init(void);

// tests functions for tests
int			tcit_tadd(void);
int			tcit_crash_catcher(void);
int			tcit_tordering(void);
int			tcit_return_check(void);

// dummy tests
int			dummy_fail_test(void);
int			dummy_success_test(void);
int			dummy_sigsegv(void);
int			dummy_capture_stdin(void);

// dummy modules creators
t_module	*create_string_tests(void);
t_module	*create_list_tests(void);

// self tests
t_module	*create_modules_tests(void);
t_module	*create_tests_tests(void);
t_module	*create_utils_tests(void);

# endif

// user facing functions
// add tests / modules
bool		tc_module_tadd(t_module *m, const t_test *test);
bool		tc_module_taddf(t_module *m, int (*f)(void), const char *s);
void		tc_module_init(t_module **m_ptr, const char *name, const char *dsc);
bool		tc_module_add_sub(t_module *m, t_module *sub_module);

// run tests / modules
int			tc_module_run(t_module *module);
int			tc_module_run_self_tests(t_module *module);

// module utils
void		tc_module_sort(t_module	*root);
void		tc_module_display(const t_module *module, t_display_mode mode);

// destroy module tree
void		tc_module_destroy(t_module *m);

#endif /* TC_MODULES_H */
