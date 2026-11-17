#ifndef DICE_H
#define DICE_H

#include <stdbool.h>

#define DICE_MIN_SIDES 2u
#define DICE_MAX_SIDES 1000u
#define DICE_MAX_ROLLS 1000000u

// Toy dice roller used to demonstrate the ctesting library.
//
// The project is split into tiny functions so every part has a clear purpose:
//   - dice_parse()       parses a die such as "d20";
//   - dice_parse_rolls() parses the number of rolls;
//   - dice_parse_args()  parses the command line;
//   - dice_roll()        rolls one die;
//   - dice_stats_*()     accumulate and print statistics;
//   - dice_run()         performs all the requested rolls and prints them.
//
// dice_seed() makes the rolls reproducible.

typedef struct s_die
{
	unsigned int sides;
} t_die;

typedef struct s_dice_stats
{
	unsigned int	   rolls;
	unsigned int	   min;
	unsigned int	   max;
	unsigned long long total;
} t_dice_stats;

typedef struct s_dice_options
{
	t_die		 die;
	unsigned int rolls;
	bool		 raw;
	bool		 stats;
} t_dice_options;

void		 dice_seed(unsigned int seed);
int			 dice_parse(const char *spec, t_die *die);
int			 dice_parse_rolls(const char *text, unsigned int *rolls);
int			 dice_parse_args(int argc, char **argv, t_dice_options *options);
unsigned int dice_roll(const t_die *die);
void		 dice_stats_init(t_dice_stats *stats);
void		 dice_stats_add(t_dice_stats *stats, unsigned int roll);
void		 dice_stats_print(const t_dice_stats *stats);
int			 dice_run(const t_dice_options *options);
void		 dice_usage(const char *program);

int dice_main(int ac, char **av);

#endif /* DICE_H */
