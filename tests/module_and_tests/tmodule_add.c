#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int ctit_add(void)
{
	t_module *test_root;

	ct_module_init(&test_root, "root", "root for tests");
	ct_module_add_sub(test_root, create_dummy_module());
	if (test_root->module_count != 1 || test_root->submodules_list == NULL)
		return (ct_module_destroy(test_root), 1);
	ct_module_add_sub(test_root, create_quiet_dummy());
	if (test_root->module_count != 2 || test_root->submodules_list == NULL)
		return (ct_module_destroy(test_root), 2);
	ct_module_add_sub(test_root, NULL);
	if (test_root->module_count != 2 || test_root->submodules_list == NULL)
		return (ct_module_destroy(test_root), 3);
	return (ct_module_destroy(test_root), EXIT_SUCCESS);
}
