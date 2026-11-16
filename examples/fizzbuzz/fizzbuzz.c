#ifndef _POSIX_C_SOURCE

#define _POSIX_C_SOURCE 200809L

#endif	// _POSIX_C_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fizzbuzz.h"

int fizzbuzz_scan(t_fizzbuzz *fb)
{
	if (!fb)
		return (1);
	printf(PROMPT);
	fflush(stdout);
	if (scanf("%d", &fb->value) != 1)
		return (1);
	return (0);
}

void fizzbuzz_response(const t_fizzbuzz *fb)
{
	if (!fb)
		return;
	if (fb->value % 3 == 0 && fb->value % 5 == 0)
		printf("bazz\n");
	else if (fb->value % 3 == 0)
		printf("fizz\n");
	else if (fb->value % 5 == 0)
		printf("buzz\n");
	else
		printf("%d\n", fb->value);
}

int fizzbuzz_run(void)
{
	t_fizzbuzz fb = {0};

	if (fizzbuzz_scan(&fb) != 0)
	{
		dprintf(STDERR_FILENO, ERRMSG);
		return (EXIT_FAILURE);
	}
	fizzbuzz_response(&fb);
	return (EXIT_SUCCESS);
}
