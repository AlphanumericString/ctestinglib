#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

#define CAPTURE_DSC "Check on fd capture functionalities"

t_module *create_capture_tests(void)
{
	t_module   *m_capture;
	const char *desc = CAPTURE_DSC;

	m_capture = NULL;
	ct_module_init(&m_capture, "capture", desc);
	if (!m_capture)
		return (NULL);
	ct_module_taddf(m_capture, ctit_capture, "fd capture");
	return (m_capture);
}

#undef CAPTURE_DSC
