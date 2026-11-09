#include "ct_structs.h"
#include <stdlib.h>

/* *********************** */
/*                         */
/* Functional tests main   */
/*                         */
/* Not compiled for lib    */
/*                         */
/* *********************** */

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"
#include "ct_utils.h"

#define FSETUP	"Failed to setup tests"
#define FSET_SBM	"Failed to setup tests"
#define RDESC	"The library for which the tests are running"

// static void	__attribute__((unused))	display_diff_formats(t_module *th)
// {
// 	write(STDOUT_FILENO, "=======default=\n", 16);
// 	ct_module_display(th, DEFAULT);
// 	write(STDOUT_FILENO, "========detail=\n", 16);
// 	ct_module_display(th, DETAIL_ALL);
// 	write(STDOUT_FILENO, "======dflt+sbm=\n", 16);
// 	ct_module_display(th, DEFAULT | TM_PRINT_SBM);
// 	write(STDOUT_FILENO, "===ol+sbm+last=\n", 16);
// 	ct_module_display(th, ONELINE | TM_PRINT_SBM | TM_PRINT_LAST);
// 	write(STDOUT_FILENO, "===========sbm=\n", 16);
// 	ct_module_display(th, TM_PRINT_SBM);
// 	write(STDOUT_FILENO, "===========inf=\n", 16);
// 	ct_module_display(th, TM_INF_ALL);
// 	write(STDOUT_FILENO, "=======oneline=\n", 16);
// 	ct_module_display(th, ONELINE);
// 	write(STDOUT_FILENO, "========ol+sbm=\n", 16);
// 	ct_module_display(th, ONELINE | TM_PRINT_SBM);
// 	write(STDOUT_FILENO, "========ol+lst=\n", 16);
// 	ct_module_display(th, ONELINE | TM_PRINT_LAST);
// 	write(STDOUT_FILENO, "====ol+lst+sbm=\n", 16);
// 	ct_module_display(th, ONELINE | TM_PRINT_LAST | TM_PRINT_SBM);
// }

static bool	add_modules(t_module *root, t_module *(**f)(void))
{
	size_t	i;

	if (!f || !root)
		return (false);
	i = 0;
	while (f[i])
		if (!ct_module_add_sub(root, f[i++]()))
			return (false);
	return (true);
}

int	main(void)
{
	t_module	*root;
	int			ret;

	ct_module_init(&root, "ctestinglib", RDESC);
	if (!add_modules(root, (t_module *(*[])(void)){create_modules_tests,
		create_alloc_tests, create_tests_tests, create_utils_tests,
		create_assert_tests, create_capture_tests,
		NULL}))
		return (ct_module_destroy(root), cti_print_string(FSET_SBM),
			EXIT_FAILURE);
	ret = ct_module_run(root);
	ct_module_sort(root);
	ct_module_display(root, 0);
	return (ct_module_destroy(root), ret);
}
