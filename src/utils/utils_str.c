#include "ct_utils.h"

int	ct_strncmp(const char *s, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s[i] == s2[i] && s[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s[i] - (unsigned char)s2[i]);
}

size_t	ct_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
