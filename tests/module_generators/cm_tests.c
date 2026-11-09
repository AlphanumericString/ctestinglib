#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

#define TCMODDESCS "Check on handling tests functionalities"

t_module *create_tests_tests(void)
{
	t_module *tests_tests;

	ct_module_init(&tests_tests, "Tests", TCMODDESCS);
	ct_module_taddf(tests_tests, ctit_tadd, "add + addf");
	ct_module_taddf(tests_tests, ctit_crash_catcher, "crash catcher");
	ct_module_taddf(tests_tests, ctit_tordering, "ordering");
	ct_module_taddf(tests_tests, ctit_return_check, "return value checking");
	ct_module_taddf(tests_tests, ctit_timeout, "test timeout");
	return (tests_tests);
}

#undef TCMODDESCS
