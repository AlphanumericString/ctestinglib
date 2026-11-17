/**
 * @file ct_structs.h
 * @brief Public types used to build and describe a test tree.
 */

#ifndef CT_STRUCTS_H
#define CT_STRUCTS_H

#include <stddef.h>
#include <stdbool.h>

#ifdef CTESTS_INTERNAL_TESTS

typedef struct s_ftest
{
	const char *name;
	int (*test_func)(void);
} t_ftest;

#endif /* CTESTS_INTERNAL_TESTS */

#ifdef CTESTS_INTERNAL

typedef enum e_test_type
{
	CT_TPURE = 0,
	CT_PARAM,
} t_test_type;

typedef union u_test_func
{
	int (*pure)(void);
	int (*param)(void *);
} t_test_func;

typedef struct s_test
{
	const char *name;
	t_test_type type;
	t_test_func test_func;
	void	   *param;

	int return_value;
	int pid;

	unsigned int timeout;
	bool		 timed_out;
} t_test;

typedef struct s_list
{
	void		  *data;
	struct s_list *next;
} t_list;

typedef struct s_module
{
	const char			  *name;
	const char			  *description;
	const struct s_module *parent;
	int					   depth;

	t_list *tests_list;
	int		tests_count;
	int		tests_passed;

	t_list *submodules_list;
	int		module_count;
	int		module_passed;

	// module meta data
	bool auto_cleanup;
	void (*clean_up_function)(void *);
	bool (*setup_function)(void);
	bool (*teardown_function)(void);
	bool hooks_wrap_submodules;

	unsigned int timeout;

} t_module;

typedef enum e_display_mode
{
	// test prints
	CT_DT_KO = 0 << 0,
	CT_DT_OK = 1 << 0,
	// test names
	CT_DT_SHRTNAME = 0 << 1,
	CT_DT_FULLNAME = 1 << 1,
	// module summaries
	CT_DM_SUMNBS = 0 << 2,
	CT_DM_SUMPCT = 1 << 2,
	CT_DM_SUMSILENT = 2 << 2,
	// calc nb tests
	CT_DSBM_MRG = 0 << 4,
	CT_DSBM_SEP = 1 << 4,
	// submodule summaries
	CT_DSBM_SUMNBS = 0 << 5,
	CT_DSBM_SUMPCT = 1 << 5,
	// fmts
	CT_FMT_TXT = 0 << 6,
	CT_FMT_JSN = 1 << 6,
	CT_FMT_HTML = 2 << 6,
	// 3 << 6 to 6 << 6 are reserved for future formats (eg: xml)
	CT_FMT_SILENT = 7 << 6,
	CT_PRTY_COLOR = 0 << 9,
	CT_PRTY_NOCOLOR = 1 << 9,
	// default
	CT_DISPLAY_DEFAULT = 0,
	// masks
	CT_DT_RESMSK = 1 << 0,
	CT_DT_NAMEMSK = 1 << 1,
	CT_DM_SUMMSK = 3 << 2,
	CT_DSBM_GRPMSK = 1 << 4,
	CT_DSBM_SUMMSK = 1 << 5,
	CT_FMT_MSK = 7 << 6,
	CT_PRTY_MSK = 1 << 9,
} t_display_mode;

#else /* !CTESTS_INTERNAL */

// define types as anonymous structs to avoid compilation issues

/// @brief A module: a named group of tests and submodules.
/// @details Modules are created with ct_module_init(). Tests are attached
/// with ct_module_taddf() and friends, submodules with ct_module_add_sub().
/// The tree is run with ct_module_run() or ct_module_run_args() and released
/// with ct_module_destroy().
/// @note The type is opaque: its fields are private to the library.
typedef struct s_module t_module;

/// @brief A single test attached to a module.
/// @details Tests are created and attached by ct_module_taddf() (plain test),
/// ct_module_taddf_param() (parametrized test) or their timeout variants.
/// ct_module_tadd() attaches an already existing test object.
/// @note The type is opaque: its fields are private to the library.
typedef struct s_test t_test;

/// @brief Node of the internal singly linked list.
/// @note This type is exposed only because the module API refers to it; it is
/// not meant to be used directly by test code.
typedef struct s_list t_list;

/// @brief Options controlling how test results are displayed.
/// @details Each option is a bit field; several options can be combined with
/// a bitwise OR, for example:
/// @code
/// CT_DT_OK | CT_DM_SUMPCT | CT_FMT_JSN
/// @endcode
/// displays all test results (ok, ko and crashes) with module summaries in
/// percent, formatted as JSON.
///
/// Layout:
/// - bit 0: test result display (CT_DT_KO, CT_DT_OK);
/// - bit 1: test name display (CT_DT_SHRTNAME, CT_DT_FULLNAME);
/// - bits 2-3: module summary display (CT_DM_SUMNBS, CT_DM_SUMPCT,
///   CT_DM_SUMSILENT);
/// - bit 4: submodule result grouping (CT_DSBM_MRG, CT_DSBM_SEP);
/// - bit 5: submodule summary display (CT_DSBM_SUMNBS, CT_DSBM_SUMPCT);
/// - bits 6-8: output format (CT_FMT_TXT, CT_FMT_JSN, CT_FMT_HTML,
///   CT_FMT_SILENT, values 3 to 6 are reserved for future formats such as
///   XML);
/// - bit 9: color output (CT_PRTY_COLOR, CT_PRTY_NOCOLOR).
/// @note Some combinations are invalid: for example JSON output cannot be
/// combined with CT_DM_SUMSILENT.
/// @note The JSON output is compact; use a tool such as jq to make it human
/// readable.
/// @warning A percentage summary prints "NaN%" instead of "100%" when there
/// is no test to run (0/0), to avoid a division by zero.
typedef enum e_display_mode
{
	/// @brief Display only failed tests and crashes (default).
	CT_DT_KO = 0 << 0,
	/// @brief Display all test results, including successful tests.
	CT_DT_OK = 1 << 0,
	/// @brief Display only the test name (default).
	CT_DT_SHRTNAME = 0 << 1,
	/// @brief Display the full test path (module1:module2:testname).
	CT_DT_FULLNAME = 1 << 1,
	/// @brief Display module summaries as passed / total (default).
	CT_DM_SUMNBS = 0 << 2,
	/// @brief Display module summaries as a percentage.
	CT_DM_SUMPCT = 1 << 2,
	/// @brief Do not display any module information.
	CT_DM_SUMSILENT = 2 << 2,
	/// @brief Merge submodule results into their parent (default).
	CT_DSBM_MRG = 0 << 4,
	/// @brief Report each module's own results separately.
	CT_DSBM_SEP = 1 << 4,
	/// @brief Display submodule summaries as passed / total (default).
	CT_DSBM_SUMNBS = 0 << 5,
	/// @brief Display submodule summaries as a percentage.
	CT_DSBM_SUMPCT = 1 << 5,
	/// @brief Output the simple text format (default).
	CT_FMT_TXT = 0 << 6,
	/// @brief Output the compact JSON format.
	CT_FMT_JSN = 1 << 6,
	/// @brief Output a standalone HTML report.
	CT_FMT_HTML = 2 << 6,
	// 3 << 6 to 6 << 6 are reserved for future formats (eg: xml)
	/// @brief Do not output anything, only return the test status.
	CT_FMT_SILENT = 7 << 6,
	/// @brief Use colors in the human readable output (default).
	CT_PRTY_COLOR = 0 << 9,
	/// @brief Disable colors in the human readable output.
	CT_PRTY_NOCOLOR = 1 << 9,
	/// @brief Default display mode (all defaults, text format, colors).
	CT_DISPLAY_DEFAULT = 0,
} t_display_mode;

#endif /* CTESTS_INTERNAL */

#endif /* CT_STRUCTS_H */
