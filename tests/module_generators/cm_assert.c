#include "ct_structs.h"

#define CTESTS_INTERNAL_TESTS
#include "ct_assert.h"
#include "ct_modules.h"

#define ASRT_DSC	"Assertion module to check conditions and print on fail"

t_module	*create_assert_tests(void)
{
	t_module	*m_assert;
	const char	*desc = ASRT_DSC;

	m_assert = NULL;
	ct_module_init(&m_assert, "Module assert", desc);
	ct_module_taddf(m_assert, ctit_true, "true");
	ct_module_taddf(m_assert, ctit_false, "false");
	ct_module_taddf(m_assert, ctit_streq, "str equality checks");
	ct_module_taddf(m_assert, ctit_strne, "str differ checks");
	return (m_assert);
}

#undef ASRT_DSC
