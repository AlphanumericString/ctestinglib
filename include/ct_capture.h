#ifndef CT_CAPTURE_H
#define CT_CAPTURE_H

#include <sys/cdefs.h>
#include <stdbool.h>

/**
 * @file ct_capture.h
 * @brief Capture everything written to a file descriptor.
 *
 * Redirects everything written to @c source_fd into an internal pipe so that
 * a test can assert on it:
 *
 * @code
 * t_capture *cap;
 *
 * cap = ct_capture_create(STDOUT_FILENO);
 * if (!cap)
 *     return (EXIT_FAILURE);
 * my_function_that_prints();
 * if (ct_capture_compare(cap, "expected output") != 0)
 *     return (ct_capture_destroy(cap), EXIT_FAILURE);
 * ct_capture_destroy(cap);
 * @endcode
 *
 * The original descriptor is restored by ct_capture_destroy(). Reading is
 * non blocking: ct_capture_get() and ct_capture_compare() consume the output
 * that is available when they are called, so call them once the code under
 * test is done writing.
 *
 * ct_capture_destroy() flushes stdio while the descriptor is still captured
 * and then restores it: output that was neither read nor compared is
 * discarded with the capture pipe instead of being written to the real
 * descriptor. It is therefore safe to destroy a capture with output still
 * buffered (for example from a function called by ct_auto_call()).
 *
 * Captured output is handled as a NUL terminated string: it is truncated at
 * the first NUL byte.
 */

/// @brief A capture on one file descriptor.
/// @details The type is opaque: create it with ct_capture_create() and release
/// it with ct_capture_destroy(). Destroying the handle restores the captured
/// descriptor and closes the internal pipe.
typedef struct s_capture t_capture;

t_capture *ct_capture_create(int source_fd) __attribute__((malloc));

void ct_capture_destroy(t_capture *capture);

char *ct_capture_get(t_capture *capture) __attribute__((malloc));

int ct_capture_compare(t_capture *capture, const char *expected);

#endif /* CT_CAPTURE_H */
