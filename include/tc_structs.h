/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_structs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 09:29:28 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 09:29:28 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_STRUCTS_H
# define TC_STRUCTS_H

# include <stddef.h>
# include <stdbool.h>

# ifdef CTESTS_INTERNAL

typedef struct s_test
{
	const char	*name;
	int			(*test_func)(void);
	int			return_value;
	int			pid;
}	t_test;

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

	// module meta data
	bool					auto_cleanup;
	void					(*clean_up_function)(void *);
	void					(*setup_function)(void);
	void					(*teardown_function)(void);

}	t_module;

# else /* !CTESTS_INTERNAL */

// define types as anonymous structs to avoid compilation issues
typedef struct s_module		t_module;
typedef struct s_test		t_test;
typedef struct s_list		t_list;

# endif /* CTESTS_INTERNAL */

// TODO:!!! // TC_FMT_JSON = 1 << 6, // outputs as json
// TODO:!	// TC_FMT_XML = 2 << 6, // outputs as xml

typedef enum e_display_mode
{
	// tests
	// test display mode
	TC_DT_KO	= 0 << 0, // displays ko and crashes (default)
	TC_DT_OK	= 1 << 0, // displays ko, crashes, ok
	// test name display mode
	TC_DT_SHORTNAME = 0 << 1, // print the test name (e.g.: "foo  KO") (default)
	TC_DT_FULLNAME = 1 << 1, // print the test name (e.g.: "bar::foo KO")

	// modules
	// module sumary display mode
	TC_DM_SUMARY = 0 << 2, // display sumary (e.g.: "foo 1/1") (default)
	TC_DM_SUMPERCENT = 1 << 2, // display in percent (e.g.: "foo 100%")
	TC_DM_SILENT = 2 << 2, // displays no module infos

	// sub modules
	// merged or split
	TC_DSBM_MERGED = 0 << 4, // display submodules results merged aka foo 1/3 [baz:0/2] (default)
	TC_DSBM_SPLIT = 1 << 4, // display submodules results split "foo 1/1 [baz: 1/1]"
	// nb or percent
	TC_DSBM_SUMNB = 0 << 5, // displays "foo 1/1 [submod:1/1]..." (default)
	TC_DSBM_SUMPRECENT = 1 << 5, // display "foo 100% [submod:100%]..."

	// format
	// output format
	TC_FMT_TEXT = 0 << 6, // simple text format (default)
	TC_FMT_SILENT = 3 << 6, // no output only return value

# ifdef CTESTS_INTERNAL
	TC_DT_RESMASK = 1 << 0,
	TC_DT_NAMMASK = 1 << 1,
	TC_DM_SUMMARY_MASK = 3 << 2,
	TC_DSBM_GROUPING_MASK = 1 << 4,
	TC_DSBM_SUMMARY_MASK = 1 << 5,
	TC_FMT_MASK = 3 << 6,
# endif /* CTESTS_INTERNAL */

	TC_DISPLAY_DEFAULT = 0,
}	t_display_mode;

#endif /* TC_STRUCTS_H */
