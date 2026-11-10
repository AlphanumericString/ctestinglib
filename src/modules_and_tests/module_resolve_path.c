#define CTESTS_INTERNAL
#include "ct_internal.h"
#include "ct_structs.h"
#include "ct_utils.h"

#include <stdbool.h>

#define ERR_PRE "ctests: "
#define ERR_EMPTY_PATH ERR_PRE "empty module path\n"
#define ERR_EMPTY_SEG_PRE ERR_PRE "empty module name in path '"
#define ERR_NOT_FOUND_PRE ERR_PRE "module not found: '"
#define ERR_NOT_FOUND_POST "'\n"
#define ERR_IS_TEST_PRE ERR_PRE "test '"
#define ERR_IS_TEST_POST "' can not be run: only modules can be selected\n"

// cli errors are written to stderr to keep stdout (the reports) parsable
static void loc_err_bytes(const char *str, size_t n)
{
	write(STDERR_FILENO, str, n);
}

static void loc_err_string(const char *str)
{
	loc_err_bytes(str, ct_strlen(str));
}

// length of the segment between ':' separators
static size_t loc_seglen(const char *seg)
{
	size_t i;

	i = 0;
	while (seg[i] != '\0' && seg[i] != ':')
		i++;
	return (i);
}

// exact match of a ':' delimited segment (not null terminated) against name
static bool loc_name_eq(const char *name, const char *seg, size_t seglen)
{
	if (seglen == 0 || ct_strncmp(name, seg, seglen) != 0)
		return (false);
	return (name[seglen] == '\0');
}

static t_module *loc_find_sub(const t_module *m, const char *seg, size_t seglen)
{
	const t_list *sbm;

	sbm = m->submodules_list;
	while (sbm)
	{
		if (loc_name_eq(((const t_module *)sbm->data)->name, seg, seglen))
			return ((t_module *)sbm->data);
		sbm = sbm->next;
	}
	return (NULL);
}

static bool loc_is_test_name(const t_module *m, const char *seg, size_t seglen)
{
	const t_list *tls;

	tls = m->tests_list;
	while (tls)
	{
		if (loc_name_eq(((const t_test *)tls->data)->name, seg, seglen))
			return (true);
		tls = tls->next;
	}
	return (false);
}

static t_module *loc_resolve_err(const char *pre, const char *post,
								 const char *seg, size_t seglen,
								 const char *path)
{
	loc_err_string(pre);
	loc_err_bytes(seg, seglen);
	loc_err_string("' in path '");
	loc_err_string(path);
	loc_err_string(post);
	return (NULL);
}

static t_module *loc_resolve_empty(const char *path)
{
	loc_err_string(ERR_EMPTY_SEG_PRE);
	loc_err_string(path);
	loc_err_string("'\n");
	return (NULL);
}

// descends one segment into cur's submodules, NULL (+ printed error) on fail
static t_module *loc_descend(t_module *cur, const char *seg, size_t seglen,
							 const char *path)
{
	t_module *sub;

	sub = loc_find_sub(cur, seg, seglen);
	if (sub == NULL)
	{
		if (loc_is_test_name(cur, seg, seglen) == true)
			return (loc_resolve_err(ERR_IS_TEST_PRE, ERR_IS_TEST_POST, seg,
									seglen, path));
		return (loc_resolve_err(ERR_NOT_FOUND_PRE, ERR_NOT_FOUND_POST, seg,
								seglen, path));
	}
	return (sub);
}

// resolves a ':' separated module path from the cli against root
t_module *cti_module_resolve(t_module *root, const char *path)
{
	const char *seg;
	size_t		seglen;
	t_module   *cur;
	bool		first;

	if (!root || !path || *path == '\0')
		return (loc_err_string(ERR_EMPTY_PATH), NULL);
	cur = root;
	first = true;
	seg = path;
	while (*seg != '\0')
	{
		seglen = loc_seglen(seg);
		if (seglen == 0)
			return (loc_resolve_empty(path));
		if (first == false || loc_name_eq(cur->name, seg, seglen) == false)
			cur = loc_descend(cur, seg, seglen, path);
		if (cur == NULL)
			return (NULL);
		first = false;
		if (seg[seglen] == '\0')
			return (cur);
		seg += seglen + 1;
	}
	return (cur);
}
