/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_capture.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 09:24:47 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 09:24:47 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_CAPTURE_H
# define TC_CAPTURE_H

# include <sys/cdefs.h>
# include <stdbool.h>

# ifdef CTESTS_INTERNAL

#  define TC_RBUFFSIZE 4096

// Structure to hold capture state for a single file descriptor
typedef struct s_capture
{
	int	original_fd;// Backup of the original file descriptor
	int	source_fd;	// The FD we're capturing
	// int mode; // Capture mode (CAPTURE_WRITE, CAPTURE_READ, or CAPTURE_BOTH)
	int	pipe_read;
	int	pipe_write;
}	t_capture;

void		tci_fflush_step(t_capture *c);

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
t_capture	*tc_capture_create(int source_fd) __attribute__((malloc));
void		tc_capture_destroy(t_capture *c);
bool		tc_capture_init(int source_fd, t_capture *capture);
int			tc_capture_compare(t_capture *capture, const char *expected);
char		*tc_capture_get(t_capture *c_fd) __attribute__((malloc));
bool		tc_capture_release(t_capture *capture);

#endif /* TC_CAPTURE_H */
