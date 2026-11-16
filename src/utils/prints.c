#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

void cti_print_string(const char *str)
{
	write(STDOUT_FILENO, str, strlen(str));
}

void cti_print_charn(int depth, char c)
{
	if (depth <= 0 || !c)
		return;
	while (depth--)
		write(STDOUT_FILENO, &c, 1);
}

void cti_print_nb(int nb)
{
	char buffer[16];
	int	 len;

	len = snprintf(buffer, sizeof(buffer), "%d", nb);
	if (len < 0)
		return;
	if ((size_t)len >= sizeof(buffer))
		len = sizeof(buffer) - 1;
	write(STDOUT_FILENO, buffer, (size_t)len);
}

// prints float with 2 decimal points
void cti_print_flt(float f)
{
	char buffer[64];
	int	 len;

	len = snprintf(buffer, sizeof(buffer), "%.2f", (double)f);
	if (len < 0)
		return;
	if ((size_t)len >= sizeof(buffer))
		len = sizeof(buffer) - 1;
	write(STDOUT_FILENO, buffer, (size_t)len);
}
