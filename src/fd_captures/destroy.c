#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Stop capturing and free the capture handle.
/// @details Restores the captured descriptor, closes the internal pipe and
/// frees the handle.
/// @param capture Capture handle, NULL is accepted.
void ct_capture_destroy(t_capture *capture)
{
	if (!capture)
		return;
	ct_capture_release(capture);
	safefree(capture);
}
