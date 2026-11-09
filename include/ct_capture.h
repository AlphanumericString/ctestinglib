#ifndef CT_CAPTURE_H
# define CT_CAPTURE_H

# include <sys/cdefs.h>
# include <stdbool.h>

# ifdef CTESTS_INTERNAL

#  define CT_RBUFFSIZE 4096

// Structure to hold capture state for a single file descriptor
typedef struct s_capture
{
	int	original_fd;// Backup of the original file descriptor
	int	source_fd;	// The FD we're capturing
	// int mode; // Capture mode (CAPTURE_WRITE, CAPTURE_READ, or CAPTURE_BOTH)
	int	pipe_read;
	int	pipe_write;
}	t_capture;

void		cti_fflush_step(t_capture *c);

# else

typedef struct s_capture	t_capture;

# endif /* CTESTS_INTERNAL */

// Enum for capture modes
// typedef enum {
//  	CAPTURE_WRITE = 1 << 0,  // Capture write operations (0x01)
//  	CAPTURE_READ  = 1 << 1,  // Capture read operations (0x02)
//  	CAPTURE_RW = CAPTURE_WRITE | CAPTURE_READ // (0x03)
// }	capture_mode_t;

// capture functions
t_capture	*ct_capture_create(int source_fd) __attribute__((malloc));
void		ct_capture_destroy(t_capture *c);
bool		ct_capture_init(int source_fd, t_capture *capture);
int			ct_capture_compare(t_capture *capture, const char *expected);
char		*ct_capture_get(t_capture *c_fd) __attribute__((malloc));
bool		ct_capture_release(t_capture *capture);

#endif /* CT_CAPTURE_H */
