/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_fxtr.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:07:54 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:07:54 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_FXTR_H
# define TESTS_FXTR_H

# include "fcntl.h"
# include <unistd.h>

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

typedef struct s_test
{
	char	*name;
	int		(*test_func)(void);
	int		return_value;
	//
	pid_t	pid;
	// tests fork pid
}	t_test;

typedef struct s_module
{
	const char				*name;
	const char				*description;
	const struct s_module	*parent;
	int						depth;

	t_list					*tests_list;
	int						tests_count;
	int						tests_passed;

	t_list					*submodules_list;
	int						module_count;
	int						module_passed;
}	t_module;

typedef enum e_display_mode
{
	TM_INF_DESC = 1 << 0, // prints module desc
	TM_INF_NAME = 1 << 1, // prints module name
	TM_INF_ALL = TM_INF_NAME | TM_INF_DESC, // infos all
	//
	TM_SUM_PERCENT = 1 << 2, // summary percent
	TM_SUM_NB = 1 << 3, // summary number
	TM_SUM_ALL = TM_SUM_PERCENT | TM_SUM_NB, // summary all
	//
	TM_NONE = 1 << 5, // DONT prints module infos.
	TM_PRINT_LAST = (1 << 6) | TM_SUM_ALL,
	// only last module w all info
	TM_PRINT_SBM = 1 << 7,
	TM_SBM_SPLIT = 1 << 8 | TM_PRINT_SBM,

	_MODULE_FIELDS_MASK = TM_INF_ALL | TM_SUM_ALL | TM_PRINT_SBM
		| TM_PRINT_LAST | TM_NONE,
	//
	// T NAME : default: only test name is printed
	TESTS_SILENT = 1 << 10, // dont print the tests names when printing
	TESTS_FULLNAME = 1 << 11,
	// print the full chain "mod1:submod:testname ok|ko"
	TESTS_ALL = 1 << 12, // prints all tests even non-fail ones
	//
	// quick defaults
	SILENT = 1 << 13, // returns res of tests, 0=ok 1=fail.
	DEFAULT = TM_INF_ALL | TM_SUM_ALL,
	ONELINE = TM_NONE | TESTS_FULLNAME,
	DETAIL_ALL = DEFAULT | TESTS_ALL,
}	t_display_mode;

# ifndef FORK_TESTS
#  define FORK_TESTS 1
# endif

// internal tooling to run lib
# ifdef INTERNAL_TESTINGLIB

int			_cmp_alpha(const char *s1, const char *s2);
void		*safealloc(size_t size);
void		list_push(t_list **lst, void *data);
void		list_foreach(const t_list *list, void (*f)(void *data));
void		print_string(const char *str);
void		display_module_chain(const t_module *m);
void		print_padding(int depth);
void		_tests_display(t_module *m, t_display_mode md, int depth);
void		print_nb(int nb);
void		print_flt(float f);
int			_collect_sbm_t_success(t_module	*m);
int			_collect_sbm_t_total(t_module *m);

# endif

# ifdef INTERNAL_TESTINGLIB_TESTS

// dummy tests
int			dummy_fail_test(void);
int			dummy_success_test(void);
int			dummy_sigsegv(void);

// dummy modules creators
t_module	*create_string_tests(void);
t_module	*create_list_tests(void);

# endif

// semi-user facing functions (might be usefull)
void		module_display_bwrd(t_module *m, t_display_mode md, int depth);
void		module_display_fwrd(t_module *m, t_display_mode md, int depth);
void		module_sbm(t_list	*sbm, t_display_mode md);
void		module_sum_percent(int t_t, int t_p, t_display_mode md);
void		module_sum_nb(int t_t, int t_p, t_display_mode md);
void		sort_testslist(t_list *t_ls);
void		sort_moduleslist(t_list *m_ls);
void		sort_recursive(t_module	*root);

// user facing functions
void		init_module(t_module *module, const char *name, const char *dsc);
void		module_destroy(t_module *m);
void		add_submodule(t_module *module, t_module *sub_module);
void		add_test_f(t_module *m, int (*f)(void), const char *s);
void		add_test(t_module *module, const t_test *test);
int			run_module(t_module *module);
int			run_test(t_module *module);
void		display_results(t_module *module, t_display_mode mode);

#endif /* TESTS_FXTR_H */
