#ifndef CT_CAPTURE_H
#define CT_CAPTURE_H

#include <sys/cdefs.h>
#include <stdbool.h>

/*
 * fd capture
 *
 * Redirects everything written to source_fd into an internal pipe so that a
 * test can assert on it:
 *
 *     t_capture *cap;
 *
 *     cap = ct_capture_create(STDOUT_FILENO);
 *     if (!cap)
 *         return (EXIT_FAILURE);
 *     my_function_that_prints();
 *     if (ct_capture_compare(cap, "expected output") != 0)
 *         return (ct_capture_destroy(cap), EXIT_FAILURE);
 *     ct_capture_destroy(cap);
 *
 * The original descriptor is restored by ct_capture_destroy(). Reading is
 * non blocking: ct_capture_get() and ct_capture_compare() consume the output
 * that is available when they are called, so call them once the code under
 * test is done writing.
 *
 * Captured output is handled as a NUL terminated string: it is truncated at
 * the first NUL byte.
 */

typedef struct s_capture t_capture;

/// @brief	start capturing everything written to source_fd
/// @param	source_fd	the descriptor to capture (usually STDOUT_FILENO or
///						STDERR_FILENO)
/// @return	the capture handle, or NULL on failure
t_capture *ct_capture_create(int source_fd) __attribute__((malloc));

/// @brief	stop capturing, restore source_fd and free the handle
/// @param	capture		the capture handle, NULL is accepted
void ct_capture_destroy(t_capture *capture);

/// @brief	consume everything written to the captured descriptor since the
///			last call
/// @param	capture		the capture handle
/// @return	a malloc'ed string (free it with free or safefree), an empty
///			string when nothing was written, or NULL on error
char *ct_capture_get(t_capture *capture) __attribute__((malloc));

/// @brief	consume the captured output and compare it to expected
/// @param	capture		the capture handle
/// @param	expected	the exact output the code under test should produce
/// @return	0 when the output matches, -1 on mismatch or error
int ct_capture_compare(t_capture *capture, const char *expected);

#endif /* CT_CAPTURE_H */
