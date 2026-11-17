#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stddef.h>
#include <stdio.h>

#define CALC_PROMPT "calc> "
#define CALC_LINE_MAX 128

// Toy integer calculator used to demonstrate the ctesting library.
//
// The project is split into tiny functions so every part has a clear purpose:
//   - calc_parse_op()  turns a one character operator into a t_calc_op;
//   - calc_apply()     applies an operator to two integers;
//   - calc_eval()      parses "<int> <op> <int>" and evaluates it;
//   - calc_repl()      runs the read-eval-print loop.
//
// calc_eval() and calc_apply() return:
//   0 on success;
//   1 for a malformed expression or an unknown operator;
//   2 for a division by zero.
//
// The REPL keeps a dynamically sized history of successful results. Typing
// "history" prints it. The history grows with malloc() so the ct_alloc tests
// can make its allocations fail without breaking the calculator.

typedef enum e_calc_op
{
	CALC_ADD,
	CALC_SUB,
	CALC_MUL,
	CALC_DIV
} t_calc_op;

typedef struct s_calc_history
{
	int	  *values;
	size_t len;
	size_t cap;
} t_calc_history;

int calc_parse_op(const char *token, t_calc_op *op);
int calc_apply(int lhs, t_calc_op op, int rhs, int *result);
int calc_eval(const char *line, int *result);
int calc_repl(FILE *input);

void calc_history_init(t_calc_history *history);
int	 calc_history_push(t_calc_history *history, int value);
void calc_history_print(const t_calc_history *history);
void calc_history_destroy(t_calc_history *history);

#endif /* CALCULATOR_H */
