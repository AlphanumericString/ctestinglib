#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Stop capturing and free the capture handle.
/// @details Flushes stdio while the descriptor is still captured, restores
/// the captured descriptor, closes the internal pipe and frees the handle.
/// Output that was not consumed by ct_capture_get() or ct_capture_compare()
/// is discarded instead of leaking to the restored descriptor.
/// @param capture Capture handle, NULL is accepted.
void ct_capture_destroy(t_capture *capture)
{
	if (!capture)
		return;
	ct_capture_release(capture);
	safefree(capture);
}
