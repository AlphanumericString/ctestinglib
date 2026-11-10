#include <unistd.h>
#include <limits.h>

#define CTESTS_INTERNAL
#include "ct_utils.h"

void cti_print_string(const char *str)
{
	write(STDOUT_FILENO, str, ct_strlen(str));
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
	char buffer[sizeof(int) * 8];
	int	 i;

	if (nb == 0)
		return (write(STDOUT_FILENO, "0", 1), (void)0);
	if (nb == INT_MIN)
		return (write(STDOUT_FILENO, "-2147483648", 11), (void)0);
	if (nb < 0)
		nb = (write(STDOUT_FILENO, "-", 1), -nb);
	i = 0;
	while (nb != 0)
	{
		buffer[i++] = '0' + (nb % 10);
		nb /= 10;
	}
	while (--i >= 0)
		write(STDOUT_FILENO, &buffer[i], 1);
}

// prints float with 2 decimal points
void cti_print_flt(float f)
{
	int int_part;

	if (f < 0)
	{
		write(STDOUT_FILENO, "-", 1);
		f = -f;
	}
	int_part = (int)f;
	cti_print_nb(int_part);
	write(STDOUT_FILENO, ".", 1);
	f -= int_part;
	f *= 100;
	cti_print_nb((int)f / 10);
	if (f - (int)f >= 0.5f)
		f += 1.0f;
	cti_print_nb((int)f % 10);
}
