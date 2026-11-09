#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int ctit_module_errors(void)
{
	t_module *m;
	t_module *sub;
	t_test	  t;

	ct_module_sort(NULL);
	ct_module_destroy(NULL);
	ct_module_init(NULL, "x", "y");
	m = NULL;
	ct_module_init(&m, NULL, "y");
	if (m != NULL)
		return (1);
	ct_module_init(&m, "m", "d");
	t = (t_test){0};
	sub = create_quiet_dummy();
	if (ct_module_add_sub(NULL, sub) != false)
		return (ct_module_destroy(sub), ct_module_destroy(m), 2);
	ct_module_destroy(sub);
	if (ct_module_add_sub(m, NULL) != false)
		return (ct_module_destroy(m), 3);
	if (ct_module_taddf(NULL, dummy_success_test, "x") != false)
		return (ct_module_destroy(m), 4);
	if (ct_module_taddf(m, NULL, "x") != false)
		return (ct_module_destroy(m), 5);
	if (ct_module_taddf(m, dummy_success_test, NULL) != false)
		return (ct_module_destroy(m), 6);
	if (ct_module_tadd(NULL, &t) != false)
		return (ct_module_destroy(m), 7);
	if (ct_module_tadd(m, NULL) != false)
		return (ct_module_destroy(m), 8);
	ct_module_destroy(m);
	return (EXIT_SUCCESS);
}
