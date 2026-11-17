#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dice.h"

static int parse_uint(const char *text, unsigned long max, unsigned long *out)
{
	char		 *end;
	unsigned long value;

	if (!text || !out || text[0] < '0' || text[0] > '9')
		return (1);
	value = strtoul(text, &end, 10);
	if (*end != '\0' || value > max)
		return (1);
	*out = value;
	return (0);
}

void dice_seed(unsigned int seed)
{
	srand(seed);
}

int dice_parse(const char *spec, t_die *die)
{
	unsigned long sides;

	if (!die || !spec || (spec[0] != 'd' && spec[0] != 'D'))
		return (1);
	if (parse_uint(spec + 1, DICE_MAX_SIDES, &sides) != 0 ||
		sides < DICE_MIN_SIDES)
		return (1);
	die->sides = (unsigned int)sides;
	return (0);
}

int dice_parse_rolls(const char *text, unsigned int *rolls)
{
	unsigned long value;

	if (!rolls || parse_uint(text, DICE_MAX_ROLLS, &value) != 0 || value == 0)
		return (1);
	*rolls = (unsigned int)value;
	return (0);
}

int dice_parse_args(int argc, char **argv, t_dice_options *options)
{
	bool has_die;
	bool has_rolls;
	int	 i;

	if (!options || argc < 1 || !argv)
		return (1);
	*options = (t_dice_options){0};
	has_die = false;
	has_rolls = false;
	i = 1;
	while (i < argc)
	{
		if (strcmp(argv[i], "--raw-rolls") == 0)
			options->raw = true;
		else if (strcmp(argv[i], "--stats") == 0)
			options->stats = true;
		else if (!has_die)
		{
			if (dice_parse(argv[i], &options->die) != 0)
				return (1);
			has_die = true;
		}
		else if (!has_rolls)
		{
			if (dice_parse_rolls(argv[i], &options->rolls) != 0)
				return (1);
			has_rolls = true;
		}
		else
			return (1);
		i++;
	}
	return ((has_die && has_rolls) ? 0 : 1);
}

unsigned int dice_roll(const t_die *die)
{
	if (!die || die->sides < DICE_MIN_SIDES || die->sides > DICE_MAX_SIDES)
		return (0);
	return ((unsigned int)(rand() % (int)die->sides) + 1u);
}

void dice_stats_init(t_dice_stats *stats)
{
	if (!stats)
		return;
	*stats = (t_dice_stats){0};
}

void dice_stats_add(t_dice_stats *stats, unsigned int roll)
{
	if (!stats)
		return;
	if (stats->rolls == 0)
	{
		stats->min = roll;
		stats->max = roll;
	}
	else
	{
		if (roll < stats->min)
			stats->min = roll;
		if (roll > stats->max)
			stats->max = roll;
	}
	stats->rolls++;
	stats->total += roll;
}

void dice_stats_print(const t_dice_stats *stats)
{
	if (!stats)
		return;
	printf("rolls: %u\n", stats->rolls);
	printf("total: %llu\n", stats->total);
	printf("min: %u\n", stats->min);
	printf("max: %u\n", stats->max);
	if (stats->rolls == 0)
		printf("average: 0.00\n");
	else
		printf("average: %.2f\n", (double)stats->total / (double)stats->rolls);
}

int dice_run(const t_dice_options *options)
{
	t_dice_stats  stats;
	unsigned int *rolls;
	unsigned int  i;

	if (!options || options->rolls == 0)
		return (1);
	// Keep every roll so the stats and the raw output come from the same
	// sequence. This allocation is what the ct_alloc tests make fail.
	rolls = malloc(options->rolls * sizeof(*rolls));
	if (!rolls)
		return (1);
	dice_stats_init(&stats);
	i = 0;
	while (i < options->rolls)
	{
		rolls[i] = dice_roll(&options->die);
		if (rolls[i] == 0)
			return (free(rolls), 1);
		i++;
	}
	i = 0;
	while (i < options->rolls)
	{
		dice_stats_add(&stats, rolls[i]);
		if (options->raw)
			printf("%u\n", rolls[i]);
		i++;
	}
	free(rolls);
	if (options->stats)
		dice_stats_print(&stats);
	else if (!options->raw)
		printf("total: %llu\n", stats.total);
	return (0);
}

void dice_usage(const char *program)
{
	fprintf(stderr, "usage: %s <dSIDES> <nb_rolls> [--raw-rolls] [--stats]\n",
			program ? program : "dice");
}

int dice_main(int ac, char **av)
{
	t_dice_options options;

	if (dice_parse_args(ac, av, &options) != 0)
		return (dice_usage(av[0]), EXIT_FAILURE);
	return (dice_run(&options) == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
