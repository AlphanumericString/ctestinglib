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

# include <stdbool.h>
# include <unistd.h>

// Enum for capture modes
// typedef enum {
// 	CAPTURE_WRITE = 1 << 0,  // Capture write operations (0x01)
// 	CAPTURE_READ  = 1 << 1,  // Capture read operations (0x02)
// 	CAPTURE_RW = CAPTURE_WRITE | CAPTURE_READ // (0x03)
// } capture_mode_t;

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
	TM_SBM_MASK = 15 << 5,
	TM_MASK = TM_INF_ALL | TM_SUM_ALL | TM_SBM_MASK,
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

# ifdef INTERNAL_TESTINGLIB

typedef struct s_test
{
	const char	*name;
	int			(*test_func)(void);
	int			return_value;
	int			pid;
}	t_test;

// Structure to hold capture state for a single file descriptor
typedef struct t_capture {
	int original_fd;	  // Backup of the original file descriptor
	int source_fd;		// The FD we're capturing
	// int mode;			 // Capture mode (CAPTURE_WRITE, CAPTURE_READ, or CAPTURE_BOTH)

	// For write capture
	int pipe_read;
	int pipe_write;
} t_capture;

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

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

// lists
void		list_push(t_list **lst, void *data);
void		list_foreach(const t_list *list, void (*f)(void *data));

// cmp / sorts
int			_cmp_alpha(const char *s1, const char *s2);
void		sort_testslist(t_list *t_ls);
void		sort_moduleslist(t_list *m_ls);

// memory utils
void		*safealloc(size_t size);

// collection
int			_collect_sbm_t_success(t_module	*m);
int			_collect_sbm_t_total(t_module *m);

// printing utils
void		print_string(const char *str);
void		print_nb(int nb);
void		print_flt(float f);
void		print_padding(int depth);
// module totals
void		module_sum_nb(int t_t, int t_p, t_display_mode md);
void		module_sum_percent(int t_t, int t_p, t_display_mode md);
void		module_display_bwrd(t_module *m, t_display_mode md, int depth);
void		module_display_fwrd(t_module *m, t_display_mode md, int depth);

// printing
void		display_module_chain(const t_module *m);
void		_tests_display(t_module *m, t_display_mode md, int depth);
void		module_sbm(t_list	*sbm, t_display_mode md);

# else

// define types as anonymous structs to avoid compilation issues

typedef struct s_module		t_module;
typedef struct s_test		t_test;
typedef struct s_capture	t_capture;

# endif

# ifdef INTERNAL_TESTINGLIB_TESTS

// tests functions for modules
int	tmodule_add(void);
int	tmodule_nb_passed(void);
int	tmodule_nb_tests(void);
int	tmodule_ordering(void);
int	tmodule_run(void);
int	tmodule_init(void);

// tests functions for tests
int	ttest_add(void);
int	ttest_crash_catcher(void);
int	ttest_ordering(void);
int	ttest_return_check(void);

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

# endif

// user facing functions
// add tests / modules
void		add_test(t_module *module, const t_test *test);
void		add_test_f(t_module *m, int (*f)(void), const char *s);
void		init_module(t_module *module, const char *name, const char *dsc);
bool		add_submodule(t_module *module, t_module *sub_module);

// run tests / modules
int			run_module(t_module *module);
int			run_test(t_module *module);

// module utils
void		sort_recursive(t_module	*root);
void		display_results(t_module *module, t_display_mode mode);

// destroy module tree
void		module_destroy(t_module *m);

// capture functions
t_capture	*create_capture(int source_fd);
void		capture_destroy(t_capture *c);
bool		capture_init(int source_fd, t_capture *capture);
int			capture_compare(t_capture *capture, const char *expected);
bool		capture_release(t_capture *capture);

#endif /* TESTS_FXTR_H */
