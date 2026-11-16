#include <stdlib.h>

#include "calculator.h"

int main(void)
{
	return (calc_repl(stdin) == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
