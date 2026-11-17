#ifndef FIZZBUZZ_H
#define FIZZBUZZ_H

#define PROMPT "Enter a number: "
#define ERRMSG "Can't init number\n"
#define ERRMSG_ALLOC "Can't allocate fizzbuzz state\n"

// Toy fizz-buzz project used to demonstrate the ctesting library.
//
// fizzbuzz_run() prompts on stdout, reads one number from stdin into the
// t_fizzbuzz value field, then prints the matching response on stdout:
//   - fizz when the value is a multiple of 3
//   - buzz when the value is a multiple of 5
//   - bazz when the value is a multiple of both 3 and 5
//   - the value itself otherwise
//
// The prompt is written on stdout together with the response. It is flushed
// right away, so it is visible immediately even while scanf() waits for the
// user's input.

typedef struct s_fizzbuzz
{
	int value;
} t_fizzbuzz;

int	 fizzbuzz_scan(t_fizzbuzz *fb);
void fizzbuzz_response(const t_fizzbuzz *fb);
int	 fizzbuzz_run(void);

#endif /* FIZZBUZZ_H */
