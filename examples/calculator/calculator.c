#include <stdio.h>

#include "calculator.h"

int calc_parse_op(const char *token, t_calc_op *op)
{
	if (!token || !op || token[0] == '\0' || token[1] != '\0')
		return (1);
	if (token[0] == '+')
		*op = CALC_ADD;
	else if (token[0] == '-')
		*op = CALC_SUB;
	else if (token[0] == '*')
		*op = CALC_MUL;
	else if (token[0] == '/')
		*op = CALC_DIV;
	else
		return (1);
	return (0);
}

int calc_apply(int lhs, t_calc_op op, int rhs, int *result)
{
	int value;

	if (!result)
		return (1);
	if (op == CALC_ADD)
		value = lhs + rhs;
	else if (op == CALC_SUB)
		value = lhs - rhs;
	else if (op == CALC_MUL)
		value = lhs * rhs;
	else if (op == CALC_DIV)
	{
		if (rhs == 0)
			return (2);
		value = lhs / rhs;
	}
	else
		return (1);
	*result = value;
	return (0);
}

int calc_eval(const char *line, int *result)
{
	char	  token[2];
	int		  lhs;
	int		  rhs;
	t_calc_op op;

	if (!line || !result)
		return (1);
	if (sscanf(line, " %d %1s %d", &lhs, token, &rhs) != 3)
		return (1);
	if (calc_parse_op(token, &op) != 0)
		return (1);
	return (calc_apply(lhs, op, rhs, result));
}

int calc_repl(FILE *input)
{
	char line[128];
	int	 status;
	int	 value;

	if (!input)
		return (1);
	for (;;)
	{
		fputs(CALC_PROMPT, stdout);
		fflush(stdout);
		if (!fgets(line, sizeof(line), input))
		{
			fputc('\n', stdout);
			break;
		}
		status = calc_eval(line, &value);
		if (status == 0)
			printf("%d\n", value);
		else if (status == 2)
			fputs("error: division by zero\n", stderr);
		else
			fputs("error: expected '<int> <op> <int>'\n", stderr);
	}
	return (0);
}
