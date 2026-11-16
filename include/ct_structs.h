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

typedef struct s_test
{
	const char *name;
	int (*test_func)(void);
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
	void (*setup_function)(void);
	void (*teardown_function)(void);

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
typedef struct s_module t_module;
typedef struct s_test	t_test;
typedef struct s_list	t_list;

/// @brief Display mode for test results
/// @details Bits layout:
///	- bits 0-1: test display mode
///			- 0: display ko and crashes only (default)
///			- 1: display ok, ko and crashes
///			- 2: display test name as "short" (only test name) (default)
///			- 3: display test name as "full" (module1:module2:testname)
/// - bits 2-3: sub-module display summary mode
///			- 0: display summary (default)
///			- 1: display summary in percent
///			- 2: silent (no module infos)
///	- bits 4-5: submodule display mode
///			- 0: merged results (default) (self + childrend modules)
///			- 1: split results per submodule (self only)
///			- 0: display nb tests passed/total (default)
///			- 1: display in percent
///	- bits 6-8: output format
///			- 0: simple text format (cli humman readable format) (default)
///			- 1: json format (compact)
///			- 2: html format (standalone report document)
///			- 3 to 6: reserved for future formats (eg: xml)
///			- 7: silent (no output, only return value)
///	- bit 9 : color or no color
///			- 0: color / no color toggle (default: color = yes)
///	@note: multiple options can be combined using bitwise OR eg:
///		CT_DT_OK | CT_DM_SUMPERCENT | CT_FMT_JSON
///		will display all tests results (ok, ko, crashes) with module
///		summary in percent format, outputted as json.
///	    Some combinations are invalid, for example using JSON format
///		with silent module display mode will result in an error message.
/// @note: about the JSON format: the JSON format is "compacted" to turn it
///		into humman readable text use 'jq' or another json parsing tool of the
///		cli.
///	@Warning: the percent display will output "NaN%" and not "100%" if there
///		is no tests to run (0/0) to avoid division by zero.
typedef enum e_display_mode
{
	CT_DT_KO = 0 << 0,
	CT_DT_OK = 1 << 0,
	CT_DT_SHRTNAME = 0 << 1,
	CT_DT_FULLNAME = 1 << 1,
	CT_DM_SUMNBS = 0 << 2,
	CT_DM_SUMPCT = 1 << 2,
	CT_DM_SUMSILENT = 2 << 2,
	CT_DSBM_MRG = 0 << 4,
	CT_DSBM_SEP = 1 << 4,
	CT_DSBM_SUMNBS = 0 << 5,
	CT_DSBM_SUMPCT = 1 << 5,
	CT_FMT_TXT = 0 << 6,
	CT_FMT_JSN = 1 << 6,
	CT_FMT_HTML = 2 << 6,
	// 3 << 6 to 6 << 6 are reserved for future formats (eg: xml)
	CT_FMT_SILENT = 7 << 6,
	CT_PRTY_COLOR = 0 << 9,
	CT_PRTY_NOCOLOR = 1 << 9,
	CT_DISPLAY_DEFAULT = 0,
} t_display_mode;

#endif /* CTESTS_INTERNAL */

#endif /* CT_STRUCTS_H */
