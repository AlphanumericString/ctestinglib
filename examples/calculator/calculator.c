#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void calc_history_init(t_calc_history *history)
{
	if (!history)
		return;
	*history = (t_calc_history){0};
}

static int calc_history_reserve(t_calc_history *history, size_t min_cap)
{
	int	  *values;
	size_t new_cap;

	if (!history)
		return (1);
	if (history->cap >= min_cap)
		return (0);
	new_cap = history->cap ? history->cap : 8;
	while (new_cap < min_cap)
	{
		if (new_cap > SIZE_MAX / 2)
		{
			new_cap = min_cap;
			break;
		}
		new_cap *= 2;
	}
	values = malloc(new_cap * sizeof(*values));
	if (!values)
		return (1);
	if (history->values)
	{
		memcpy(values, history->values, history->len * sizeof(*values));
		free(history->values);
	}
	history->values = values;
	history->cap = new_cap;
	return (0);
}

int calc_history_push(t_calc_history *history, int value)
{
	if (!history || calc_history_reserve(history, history->len + 1) != 0)
		return (1);
	history->values[history->len] = value;
	history->len++;
	return (0);
}

void calc_history_print(const t_calc_history *history)
{
	size_t i;

	if (!history)
		return;
	i = 0;
	while (i < history->len)
	{
		printf("%d\n", history->values[i]);
		i++;
	}
}

void calc_history_destroy(t_calc_history *history)
{
	if (!history)
		return;
	free(history->values);
	*history = (t_calc_history){0};
}

int calc_repl(FILE *input)
{
	t_calc_history history;
	char		  *line;
	int			   status;
	int			   value;

	if (!input)
		return (1);
	line = malloc(CALC_LINE_MAX);
	if (!line)
		return (1);
	calc_history_init(&history);
	for (;;)
	{
		fputs(CALC_PROMPT, stdout);
		fflush(stdout);
		if (!fgets(line, CALC_LINE_MAX, input))
		{
			fputc('\n', stdout);
			break;
		}
		if (strncmp(line, "history", 7) == 0 &&
			(line[7] == '\0' || line[7] == '\n'))
		{
			calc_history_print(&history);
			continue;
		}
		status = calc_eval(line, &value);
		if (status == 0)
		{
			printf("%d\n", value);
			// history is a convenience: losing an entry is not fatal.
			(void)calc_history_push(&history, value);
		}
		else if (status == 2)
			fputs("error: division by zero\n", stderr);
		else
			fputs("error: expected '<int> <op> <int>'\n", stderr);
	}
	calc_history_destroy(&history);
	free(line);
	return (0);
}
