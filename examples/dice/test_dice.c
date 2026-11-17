#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ct_assert.h"
#include "ct_capture.h"
#include "ct_memory.h"
#include "ct_modules.h"
#include "dice.h"

static int check_die(const char *spec, unsigned int sides)
{
	t_die die;

	die.sides = 0;
	return (ct_assert_true(dice_parse(spec, &die) == 0 && die.sides == sides,
						   "dice_parse(\"%s\") did not return d%u\n", spec,
						   sides));
}

static int test_parse(void)
{
	t_die		   die;
	unsigned int   rolls;
	t_dice_options options;
	char *good[] = {"dice", "d20", "3", "--raw-rolls", "--stats", NULL};
	char *bad[] = {"dice", "d1", "3", NULL};

	die.sides = 0;
	rolls = 0;
	if (check_die("d2", 2) != 0 || check_die("d20", 20) != 0 ||
		check_die("D1000", 1000) != 0)
		return (1);
	if (ct_assert_true(
			dice_parse("d1", &die) != 0 && dice_parse("d0", &die) != 0 &&
				dice_parse("d1001", &die) != 0 && dice_parse("20", &die) != 0 &&
				dice_parse("d", &die) != 0,
			"dice_parse should reject invalid dice\n") != 0)
		return (2);
	if (ct_assert_true(dice_parse(NULL, &die) != 0 &&
						   dice_parse("d6", NULL) != 0,
					   "dice_parse should reject NULL arguments\n") != 0)
		return (3);
	if (ct_assert_true(dice_parse_rolls("1", &rolls) == 0 && rolls == 1 &&
						   dice_parse_rolls("1000000", &rolls) == 0 &&
						   rolls == 1000000,
					   "dice_parse_rolls should accept valid counts\n") != 0)
		return (4);
	if (ct_assert_true(dice_parse_rolls("0", &rolls) != 0 &&
						   dice_parse_rolls("1000001", &rolls) != 0 &&
						   dice_parse_rolls("x", &rolls) != 0 &&
						   dice_parse_rolls(NULL, &rolls) != 0 &&
						   dice_parse_rolls("2", NULL) != 0,
					   "dice_parse_rolls should reject invalid counts\n") != 0)
		return (5);
	if (ct_assert_true(dice_parse_args(5, good, &options) == 0 &&
						   options.die.sides == 20 && options.rolls == 3 &&
						   options.raw == true && options.stats == true,
					   "dice_parse_args rejected a valid command line\n") != 0)
		return (6);
	if (ct_assert_true(dice_parse_args(3, bad, &options) != 0 &&
						   dice_parse_args(0, good, &options) != 0 &&
						   dice_parse_args(5, good, NULL) != 0 &&
						   dice_parse_args(5, NULL, &options) != 0,
					   "dice_parse_args should reject invalid arguments\n") !=
		0)
		return (7);
	return (ct_assert_true(dice_roll(NULL) == 0,
						   "dice_roll should reject a NULL die\n"));
}

static int capture_stats(const t_dice_stats *stats, const char *expected)
{
	t_capture *out;
	int		   ret;

	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (1);
	dice_stats_print(stats);
	ret = ct_capture_compare(out, expected);
	ct_capture_destroy(out);
	return (ret == 0 ? EXIT_SUCCESS : 2);
}

static int test_stats_print(void)
{
	t_dice_stats stats;

	stats = (t_dice_stats){3, 2, 5, 12};
	if (capture_stats(&stats, "rolls: 3\ntotal: 12\nmin: 2\nmax: 5\n"
							  "average: 4.00\n") != 0)
		return (1);
	dice_stats_init(&stats);
	return (capture_stats(&stats, "rolls: 0\ntotal: 0\nmin: 0\nmax: 0\n"
								  "average: 0.00\n"));
}

static int parse_rolls(const char *text, unsigned int rolls, unsigned int sides,
					   unsigned long long *total, unsigned int *min,
					   unsigned int *max)
{
	const char		  *cursor;
	char			  *end;
	unsigned long	   value;
	unsigned int	   count;
	unsigned long long sum;
	unsigned int	   lo;
	unsigned int	   hi;

	cursor = text;
	count = 0;
	sum = 0;
	lo = sides;
	hi = 0;
	while (*cursor)
	{
		value = strtoul(cursor, &end, 10);
		if (end == cursor || value < 1 || value > sides)
			return (1);
		if (*end != '\n' && *end != '\0')
			return (1);
		if (value < lo)
			lo = (unsigned int)value;
		if (value > hi)
			hi = (unsigned int)value;
		sum += value;
		count++;
		cursor = (*end == '\n') ? end + 1 : end;
	}
	if (count != rolls)
		return (1);
	*total = sum;
	*min = lo;
	*max = hi;
	return (0);
}

static int capture_raw(unsigned int rolls, unsigned int sides,
					   unsigned long long *total, unsigned int *min,
					   unsigned int *max)
{
	t_dice_options options;
	t_capture	  *out;
	char		  *text;
	int			   ret;

	options = (t_dice_options){0};
	options.die.sides = sides;
	options.rolls = rolls;
	options.raw = true;
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (1);
	dice_seed(42);
	ret = dice_run(&options);
	text = ct_capture_get(out);
	if (ret != 0 || !text)
		ret = 2;
	else if (parse_rolls(text, rolls, sides, total, min, max) != 0)
		ret = 3;
	else
		ret = EXIT_SUCCESS;
	free(text);
	ct_capture_destroy(out);
	return (ret);
}

// The same seed makes the stats and default runs repeat the raw run: their
// total/min/max can be compared without relying on a specific rand() sequence.
static int test_run(void)
{
	t_dice_options	   options;
	t_capture		  *out;
	char			   expected[256];
	unsigned long long total;
	unsigned int	   min;
	unsigned int	   max;
	int				   ret;

	ret = capture_raw(5, 6, &total, &min, &max);
	if (ret != EXIT_SUCCESS)
		return (ret);
	options = (t_dice_options){0};
	options.die.sides = 6;
	options.rolls = 5;
	options.stats = true;
	snprintf(expected, sizeof(expected),
			 "rolls: 5\ntotal: %llu\nmin: %u\nmax: %u\naverage: %.2f\n", total,
			 min, max, (double)total / 5.0);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (4);
	dice_seed(42);
	ret = dice_run(&options);
	if (ret != 0 || ct_capture_compare(out, expected) != 0)
		ret = 5;
	ct_capture_destroy(out);
	if (ret != EXIT_SUCCESS)
		return (ret);
	options.stats = false;
	snprintf(expected, sizeof(expected), "total: %llu\n", total);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (6);
	dice_seed(42);
	ret = dice_run(&options);
	if (ret != 0 || ct_capture_compare(out, expected) != 0)
		ret = 7;
	ct_capture_destroy(out);
	return (ret);
}

static int test_errors(void)
{
	t_dice_options options;
	t_capture	  *err;
	char		  *bad[] = {"prog", "d0", "3", NULL};
	int			   ret;

	options = (t_dice_options){0};
	if (ct_assert_true(dice_run(NULL) != 0 && dice_run(&options) != 0,
					   "dice_run should reject missing/empty options\n") != 0)
		return (1);
	err = ct_capture_create(STDERR_FILENO);
	if (!err)
		return (2);
	dice_usage("dice");
	ret = ct_capture_compare(err, "usage: dice <dSIDES> <nb_rolls> "
								  "[--raw-rolls] [--stats]\n");
	ct_capture_destroy(err);
	if (ret != 0)
		return (3);
	err = ct_capture_create(STDERR_FILENO);
	if (!err)
		return (4);
	ret = dice_main(3, bad);
	if (ret != EXIT_FAILURE)
		ret = 5;
	else if (ct_capture_compare(err, "usage: prog <dSIDES> <nb_rolls> "
									 "[--raw-rolls] [--stats]\n") != 0)
		ret = 6;
	else
		ret = EXIT_SUCCESS;
	ct_capture_destroy(err);
	return (ret);
}

static void auto_dice_run(void)
{
	t_dice_options options;
	t_capture	  *out;

	options = (t_dice_options){0};
	options.die.sides = 6;
	options.rolls = 4;
	out = ct_capture_create(STDOUT_FILENO);
	dice_seed(42);
	(void)dice_run(&options);
	// Consume before destroying: release flushes pending output to the
	// restored descriptor.
	(void)ct_capture_compare(out, "");
	ct_capture_destroy(out);
}

// dice_run() keeps the rolls in a malloc'ed array. ct_alloc is used to count
// that allocation and to make it fail before anything is printed.
static int test_alloc(void)
{
	t_dice_options	   options;
	t_capture		  *out;
	char			   expected[256];
	unsigned long long total;
	unsigned int	   min;
	unsigned int	   max;
	size_t			   before;
	size_t			   after;
	int				   ret;

	options = (t_dice_options){0};
	options.die.sides = 6;
	options.rolls = 5;
	options.stats = true;
	ct_failloc_reset();
	ret = capture_raw(5, 6, &total, &min, &max);
	if (ret != EXIT_SUCCESS)
		return (ret);
	snprintf(expected, sizeof(expected),
			 "rolls: 5\ntotal: %llu\nmin: %u\nmax: %u\naverage: %.2f\n", total,
			 min, max, (double)total / 5.0);

	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (1);
	dice_seed(42);
	ret = dice_run(&options);
	if (ret != 0 || ct_capture_compare(out, expected) != 0)
		ret = 2;
	else
		ret = EXIT_SUCCESS;
	ct_capture_destroy(out);
	if (ret != EXIT_SUCCESS)
		return (ret);

	before = ct_alloc_getcount();
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (3);
	dice_seed(42);
	ret = dice_run(&options);
	after = ct_alloc_getcount();
	if (ret != 0 || ct_capture_compare(out, expected) != 0)
		ret = 4;
	else if (after != before + 1)
		ret = 5;
	else
		ret = EXIT_SUCCESS;
	ct_capture_destroy(out);
	if (ret != EXIT_SUCCESS)
		return (ret);

	ct_failloc_set(0);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (ct_failloc_reset(), 6);
	dice_seed(42);
	ret = dice_run(&options);
	ct_failloc_reset();
	if (ct_assert_true(ret != 0,
					   "dice_run should report a failed roll allocation\n") !=
			0 ||
		ct_assert_true(ct_capture_compare(out, "") == 0,
					   "a failed roll allocation should print nothing\n") != 0)
		return (ct_capture_destroy(out), 7);
	ct_capture_destroy(out);

	ct_auto_call(auto_dice_run);
	ct_failloc_reset();
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	t_module *root;
	int		  ret;

	ct_module_init(&root, "dice", "tests for the dice example");
	if (!root)
		return (EXIT_FAILURE);
	ct_module_taddf(root, test_parse, "parse dice and arguments");
	ct_module_taddf(root, test_stats_print, "capture stats output");
	ct_module_taddf(root, test_run, "capture rolls and totals");
	ct_module_taddf(root, test_errors, "capture usage errors");
	ct_module_taddf(root, test_alloc, "ct_alloc showcase");
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}
